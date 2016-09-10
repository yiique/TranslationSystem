//
// Created by ������ on 16/8/31.
//

#include "PreProcReq.h"

PreProcReq::PreProcReq(const CallID & cid, TransText * p_trans_text):mp_trans_text(p_trans_text)
{
    m_call_id = cid;
}

PreProcReq::~PreProcReq()
{
}

const EDType PreProcReq::GetType() const
{
    return EDTYPE_PREPROC_REQ;
}