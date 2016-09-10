//
// Created by 刘舒曼 on 16/9/7.
//

#include "PreProcessor.h"

PreProcessor PreProcessor::ms_rtt_instance;
PreProcessor PreProcessor::ms_olt_instance;


// public functions
PreProcessor::PreProcessor(void)
{
}

PreProcessor::~PreProcessor(void)
{
}

bool PreProcessor::StartGroup(const size_t rtt_num, const size_t olt_num)
{
    if(!ms_rtt_instance.Start(rtt_num))
    {
        lerr << "Start rtt preprocess failed." << endl;
        return false;
    }

    if(!ms_olt_instance.Start(olt_num))
    {
        lerr << "Start rtt preprocess failed." << endl;
        return false;
    }

    return true;
}

void PreProcessor::PostRTTProcess(TransText * p_trans_text, const CallID cid, EventEngine * p_caller)
{
    assert(p_trans_text && p_caller);

    PreProcReq *p_proc_req = new PreProcReq(cid, p_trans_text);
    Event e(p_proc_req, p_caller);

    ms_rtt_instance.PostEvent(e);
}

void PreProcessor::PostOLTProcess(TransText * p_trans_text, const CallID cid, EventEngine * p_caller)
{
    assert(p_trans_text && p_caller);

    PreProcReq *p_proc_req = new PreProcReq(cid, p_trans_text);
    Event e(p_proc_req, p_caller);

    ms_olt_instance.PostEvent(e);
}


// private functions
void PreProcessor::on_event(Event & e)
{
    if(!e._p_edata)
    {
        lerr << "PreProcessor::on_event() e._data is null." << endl;
        return;
    }

    if(!e._p_caller)
    {
        lerr << "PreProcessor::on_event() e.caller is null." << endl;
        delete e._p_edata;
        return;
    }

    const EDType e_type = e._p_edata->GetType();
    EventData * p_edata_re = NULL;

    switch(e_type)
    {
        case EDTYPE_PREPROC_REQ:
            p_edata_re = on_pre_process(e._p_edata);
            break;
        default:
            lerr << "PreProcessor can't handle this event , type = " << e_type << endl;
    }

    if(p_edata_re)
    {
        //返回结果
        Event re(p_edata_re, this);
        e._p_caller->PostEvent(re);
    }else
    {
        lerr << "PreProcessor handle faield. set p_edata_re is NULL." << endl;
    }

    //销毁Req
    delete e._p_edata;
}

EventData * PreProcessor::on_pre_process(EventData * p_edata)
{
    if(!p_edata)
    {
        lerr << "Input EventData is null." << endl;
        return NULL;
    }

    //转换为PreProcReq
    PreProcReq * p_proc_req = dynamic_cast<PreProcReq*>(p_edata);

    if(!p_proc_req)
    {
        lerr << "Convert to PreProcReq failed." << endl;
        return NULL;
    }

    //生成res
    PreProcRes * p_proc_res = new PreProcRes(p_proc_req->GetCallID(), p_proc_req->GetTransText());

    if(!p_proc_res->GetTransText())
    {
        lerr << "PreProcessor::pre_process() input PreProcRes's TransText is null." << endl;
        return NULL;
    }

    //开始预处理
    TransText & trans_text = *(p_proc_res->GetTransText());

    int proc_res = SUCCESS;

    if( false == pre_process(trans_text) )
    {
        lerr << "PreProcess Failed, text id = " << trans_text.GetID() << endl;
        proc_res = ERR_USR_PRE_PROC;
    }

    //更新数据库中的领域 语言信息
    if(TRANS_TYPE_OLT == trans_text.GetTransType())
    {
        lout << "src = " << trans_text.GetSrcLanguage() << endl;
        lout << "tgt = " << trans_text.GetTgtLanguage() << endl;
        DBOperation::UpdateTransInfo(trans_text.GetID(), trans_text.GetDomainName(), trans_text.GetSrcLanguage(), trans_text.GetTgtLanguage() , trans_text.CountCharacter());
    }

#ifdef USR_CHARGING
    //权限获取或更新
    proc_res = DBOperation::GetPermission(trans_text.GetUsrID(), trans_text.CountCharacter());
#endif //USR_CHARGING

    //处理完毕
    p_proc_res->SetResult(proc_res);

    return p_proc_res;
}


bool PreProcessor::pre_process(TransText & trans_text)
{
    // 原步骤:1. 识别语种   2. 根据语种进行文本预处理
    // 当前直接预处理即可
    bool res = true;
    try{
        trans_text.SetTgtLanguage(LANGUAGE_CHINESE);

        res = trans_text.PreProcess( boost::bind(&LanguageProcess::StandardPreTextProcess, _1),
                                     boost::bind(&LanguageProcess::StandardSplitSent, _1, _2),
                                     boost::bind(&LanguageProcess::StandardPreSentProcess, _1));
    }
    catch(exception e){
        lerr << "Catch Error In PreProcessor :" << e.what() << endl;
        res = false;
    }
    return res;
}
