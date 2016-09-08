//
// Created by ¡ı Ê¬¸ on 16/8/31.
//

#ifndef TRANSMASTER_PREPROCRES_H
#define TRANSMASTER_PREPROCRES_H

#include "EventEngine/EventData.h"
#include "TransText/TransText.h"

class PreProcRes
        :public EventData
{
public:
    PreProcRes(const CallID & cid, TransText * p_trans_text, const int result = 0);

    ~PreProcRes(void);

    TransText * GetTransText()
    {
        return mp_trans_text;
    }

    int GetResult() const
    {
        return m_result;
    }

    void SetResult(const int result)
    {
        m_result = result;
    }

    const EDType GetType() const;

private:
    TransText * mp_trans_text;
    int m_result;
};

#endif //TRANSMASTER_PREPROCRES_H
