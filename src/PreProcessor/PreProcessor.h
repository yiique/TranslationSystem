//
// Created by 刘舒曼 on 16/9/7.
//

#ifndef TRANSMASTER_PREPROCESSOR_H
#define TRANSMASTER_PREPROCESSOR_H

#include <boost/bind.hpp>

#include "EventEngine/EventEngine.h"
#include "PreProcReq.h"
#include "PreProcRes.h"
#include "Log/LogStream.h"
#include "Common/ErrorCode.h"
#include "DataBase/DBOperation.h"
#include "LanguageProcess.h"

class PreProcessor
        : public EventEngine
{
public:
    PreProcessor(void);
    ~PreProcessor(void);

    static bool StartGroup(const size_t rtt_num, const size_t olt_num);

    // 向ms_rtt/olt_instance中插入一个新的real-time-trans/offline-trans请求PreProcReq事件
    static void PostRTTProcess(TransText * p_trans_text, const CallID cid, EventEngine * p_caller);
    static void PostOLTProcess(TransText * p_trans_text, const CallID cid, EventEngine * p_caller);

private:
    static PreProcessor ms_rtt_instance;
    static PreProcessor ms_olt_instance;

    void on_event(Event & e);   // 执行on_pre_process,向caller返回结果

    EventData * on_pre_process(EventData * p_edata);    // 生成preprocres并对transtext执行pre_process,更新olttrans的信息

    bool pre_process(TransText * trans_text);
};

#endif //TRANSMASTER_PREPROCESSOR_H
