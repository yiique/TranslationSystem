//
// Created by ¡ı Ê¬¸ on 16/8/1.
//

#ifndef TRANSMASTER_TEXTPROCREQ_H
#define TRANSMASTER_TEXTPROCREQ_H

#include "EventEngine/EventData.h"
#include "TransText.h"

class TextProcReq
        : public EventData
{
public:
    TextProcReq(const CallID & cid,
                TransText * p_trans_text,
                const TransType trans_type,
                bool need_language): mp_trans_text(p_trans_text),
                                     m_trans_type(trans_type),
                                     m_need_language(need_language)
    {
        m_call_id = cid;
    }

    ~TextProcReq(void) {};

    const EDType GetType() const
    {
        return EDTYPE_TEXT_PROCESS_REQ;
    }

    TransText * GetTransText()
    {
        return mp_trans_text;
    };

    TransType GetTransType() const
    {
        return m_trans_type;
    }

    bool NeedLanguage() const
    {
        return m_need_language;
    };

private:

    TransText * mp_trans_text;
    TransType m_trans_type;
    bool m_need_language;
};

#endif //TRANSMASTER_TEXTPROCREQ_H
