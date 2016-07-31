//
// Created by ¡ı Ê¬¸ on 16/8/1.
//

#ifndef TRANSMASTER_TEXTPROCRES_H
#define TRANSMASTER_TEXTPROCRES_H

#include "EventEngine/EventData.h"
#include "TransText.h"


class TextProcRes
        : public EventData
{
public:
    TextProcRes(const CallID & cid,
                TransText * p_trans_text,
                int result): mp_trans_text(p_trans_text), m_result(result)
    {
        m_call_id = cid;
    }

    ~TextProcRes(void) {};

    const EDType GetType() const
    {
        return EDTYPE_TEXT_PROCESS_RES;
    }

    TransText * GetTransText()
    {
        return mp_trans_text;
    }

    int GetResult() const
    {
        return m_result;
    }

private:
    TransText * mp_trans_text;
    int m_result;
};

#endif //TRANSMASTER_TEXTPROCRES_H
