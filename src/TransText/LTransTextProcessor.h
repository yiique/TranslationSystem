//
// Created by ������ on 16/8/1.
//

#ifndef TRANSMASTER_LTRANSTEXTPROCESSOR_H
#define TRANSMASTER_LTRANSTEXTPROCESSOR_H

#include "Common/ErrorCode.h"
#include "LanguageTools/CLanguage.h"

#include <map>
#include <utility>
#include <string>
#include <deque>
#include <assert.h"
using namespace std;

class LTransTextProcessor
        : public EventEngine
{
public:
    LTransTextProcessor(void);
    ~LTransTextProcessor(void);

    static LTransTextProcessor * GetInstance();

    // ��д����start����
    bool Start();

    bool LocalProcess(TransText * p_text,
                        const TransType trans_type,
                        bool need_language,
                        const CallID & cid,
                        EventEngine * p_caller);

private:
    static LTransTextProcessor ms_instance;

    // ��д������¼�������
    void on_event(Event & e);

    // �¼�����
    void on_submit_process(EventData * p_edata, EventEngine * p_caller);
};

#endif //TRANSMASTER_LTRANSTEXTPROCESSOR_H
