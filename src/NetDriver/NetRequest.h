//
// Created by 刘舒曼 on 16/7/21.
// Request数据类定义
// 包括ListenNetReq, ConnNetReq, CloseNetReq, RecvNetReq, SendNetReq
// 返回不同的event_type,见EventEngine/EventData.h
//

#ifndef TRANSMASTER_NETREQUEST_H
#define TRANSMASTER_NETREQUEST_H

#include "NetType.h"
#include "NetPacket.h"
#include "EventEngine/EventData.h"

class NetRequest
        : public EventData
{
public:
    virtual ~NetRequest(void);

    virtual const EDType GetType() const = 0;

    sork_t GetSrcSock() const
    {
        return m_src_sockfd;
    }

protected:
    NetRequest(void);
    NetRequest(sock_t sockfd);

    const sock_t m_src_sockfd;
};


class ListenNetReq
        : public NetRequest
{
public:
    ListenNetReq(sock_t sockfd, const string & ip, const string & port);
    ~ListenNetReq(void);

    const EDType GetType() const;

public:
    const string _ip;
    const string _port;
};

class ConnNetReq
        : public NetRequest
{
public:
    ConnNetReq(sock_t sockfd, const string & ip, const string & port);
    ~ConnNetReq(void);

    const EDType GetType() const;

public:
    const string _ip;
    const string _port;
};


class CloseNetReq
        : public NetRequest
{
public:
    CloseNetReq(int sockfd);
    ~CloseNetReq(void);

    const EDType GetType() const;
};


class RecvNetReq
        : public NetRequest
{
public:
    RecvNetReq(sock_t sockfd, NetPacket * p_recv_packet);
    ~RecvNetReq(void);

    const EDType GetType() const;

    NetPacket * GetRecvPakcet()
    {
        return mp_recv_packet;
    };

private:
    NetPacket * mp_recv_packet;
};

class SendNetReq
        : public NetRequest
{
public:
    SendNetReq(sock_t sockfd, NetPacket * p_send_packet);
    ~SendNetReq(void);

    const EDType GetType() const;

    NetPacket * GetSendPacket()
    {
        return mp_send_packet;
    };

private:
    NetPacket * mp_send_packet;
};


#endif //TRANSMASTER_NETREQUEST_H
