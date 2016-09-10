//
// Created by ¡ı Ê¬¸ on 16/8/31.
//

#ifndef TRANSMASTER_PREPROCREQ_H
#define TRANSMASTER_PREPROCREQ_H

#include "EventEngine/EventData.h"
#include "TransText/TransText.h"

class PreProcReq
        :public EventData
{
public:
    PreProcReq(const CallID & cid, TransText * p_trans_text);

    ~PreProcReq(void);

    TransText * GetTransText()
    {
        return mp_trans_text;
    }

    const EDType GetType() const;

private:
    TransText * mp_trans_text;
};

#endif //TRANSMASTER_PREPROCREQ_H
