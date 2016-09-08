//
// Created by ������ on 16/9/7.
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

    // ��ms_rtt/olt_instance�в���һ���µ�real-time-trans/offline-trans����PreProcReq�¼�
    static void PostRTTProcess(TransText * p_trans_text, const CallID cid, EventEngine * p_caller);
    static void PostOLTProcess(TransText * p_trans_text, const CallID cid, EventEngine * p_caller);

private:
    static PreProcessor ms_rtt_instance;
    static PreProcessor ms_olt_instance;

    void on_event(Event & e);   // ִ��on_pre_process,��caller���ؽ��

    EventData * on_pre_process(EventData * p_edata);    // ����preprocres����transtextִ��pre_process,����olttrans����Ϣ

    bool pre_process(TransText * trans_text);
};

#endif //TRANSMASTER_PREPROCESSOR_H
