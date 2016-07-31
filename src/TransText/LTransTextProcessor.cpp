//
// Created by 刘舒曼 on 16/8/1.
//

#include "LTransTextProcessor.h"


LTransTextProcessor LTransTextProcessor::ms_instance;


LTransTextProcessor::LTransTextProcessor(void)
{
}

LTransTextProcessor::~LTransTextProcessor(void)
{
}

LTransTextProcessor * LTransTextProcessor::GetInstance()
{
    return &ms_instance;
}

bool LTransTextProcessor::Start()
{

    return EventEngine::Start();
}

bool LTransTextProcessor::LocalProcess(TransText * p_text,
                                       const TransType trans_type,
                                       bool need_language,
                                       const CallID & cid,
                                       EventEngine * p_caller)
{
    assert(p_text && p_caller);

    TextProcReq * p_req = new TextProcReq(cid, p_text, trans_type, need_language);

    Event e(p_req, p_caller);
    this->PostEvent(e);
    return true;
}


// private
void LTransTextProcessor::on_event(Event & e)
{
    if(!e._p_edata || !e._p_caller)
    {
        lerr << "Event's data is null. Ignore this event." << endl;
        return ;
    }

    const EDType e_type = e._p_edata->GetType();

    if(EDTYPE_TEXT_PROCESS_REQ == e_type)
    {
        on_submit_process(e._p_edata, e._p_caller);
    }else
    {
        lerr << "Recv a unknow type event , type = " << e_type << endl;
    }

    //销毁EventData
    delete e._p_edata;
}

void LTransTextProcessor::on_submit_process(EventData * p_edata, EventEngine * p_caller)
{
    cout << "test2 in on_submit_process..." << endl;
    assert(p_edata && p_caller);

    //转换请求
    TextProcReq * p_req = dynamic_cast<TextProcReq*>(p_edata);

    if(!p_req)
    {
        lerr << "Convert to TextProcReq Failed." << endl;
        return;
    }

    if(p_req->GetTransType() != TRANS_TYPE_RTT)
    {
        lerr << "Local process can't accept a un rtt trans type." << endl;
        return;
    }

    TransText * p_text = p_req->GetTransText();

    assert(p_text);

    int result = SUCCESS;

    try
    {
        //Step 1 语种识别
        //DO NOTHING

        //Step 2 txt文本格式解析
        TxtParser txt;
        vector<string> para_vec;
        vector<pair<size_t,size_t> > pos_vec;

        if(false == txt.Parse(p_text->GetSrcData(), para_vec, pos_vec) )
        {
            lerr << "Txt Format parse Failed." << endl;
            result = ERR_PROC_TXT_FROMAT;
            throw -1;
        }

        p_text->SetFormatSrc(para_vec, pos_vec);

    }catch(...)
    {
    }

    //返回结果事件
    TextProcRes *p_proc_res = new TextProcRes(p_req->GetCallID(), p_text, result);
    Event re(p_proc_res, this);

    p_caller->PostEvent(re);

    return;
}