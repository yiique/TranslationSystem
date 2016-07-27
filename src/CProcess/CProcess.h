//
// Created by 刘舒曼 on 16/7/22.
// 事务处理基类
//

#ifndef TRANSMASTER_CPROCESS_H
#define TRANSMASTER_CPROCESS_H

#include "EventEngine/EventData.h"
#include "EventEngine/EventEngine.h"
#include "Log/LogStream.h"
#include "NetDriver/NetPacket.h"
#include "Common/f_utility.h"
#include "Common/ErrorCode.h"
#include "TinyXml/tinyxml.h"

#include <iostream>
#include <string>
using namespace std;

typedef enum
{
    PROCESS_KEEP,
    PROCESS_FINISH
} ProcessRes;

typedef enum
{
    PROCESS_TYPE_NORMAL,
    PROCESS_TYPE_RTT_SEG,
    PROCESS_TYPE_RTT
} ProcessType;

class CProcess
{
public:
    CProcess(EventEngine * p_owner, const CallID & cid, NetPacket & inpacket, NetPacket & outpacket);
    virtual ~CProcess(void);

    virtual ProcessType GetType() const;

    virtual ProcessRes Begin();
    virtual ProcessRes Work(EventData * p_edata);

protected:

    EventEngine * mp_owner;
    CallID m_cid;

    NetPacket & m_input_packet;
    NetPacket & m_output_packet;

    virtual ProcessRes on_error(int err_code);
};

#endif //TRANSMASTER_CPROCESS_H
