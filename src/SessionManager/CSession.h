//
// Created by ¡ı Ê¬¸ on 16/7/23.
//

#ifndef TRANSMASTER_CSESSION_H
#define TRANSMASTER_CSESSION_H

#include "EventEngine/EventEngine.h"
#include "Log/LogStream.h"
#include "NetDriver/NetPacket.h"
#include "CProcess/CProcess.h"

typedef enum
{
    SSTATE_LISTEN,    //0
    SSTATE_RECVING,   //1
    SSTATE_SENDING,   //2
    SSTATE_CLOSING,   //3
    SSTATE_PROCESS,   //4
    SSTATE_CONNECT    //5
} SessionState;

class CSession
{
public:
    CSession(const CallID & cid, const int sockfd);
    virtual ~CSession(void);

    const CallID & GetCallID() const
    {
        return m_cid;
    }

    int GetSock() const
    {
        return m_sockfd;
    }

    const SessionState & GetState() const
    {
        return m_state;
    }

    void SetState(const SessionState & state)
    {
        m_state = state;
    }

public:
    NetPacket _recv_packet;
    NetPacket _send_packet;
    CProcess * _p_process;
private:
    CallID m_cid;
    int m_sockfd;
    SessionState m_state;
};

#endif //TRANSMASTER_CSESSION_H
