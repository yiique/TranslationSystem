//
// Created by 刘舒曼 on 16/7/21.
// Response数据类定义
// 包括ListenNetRes, ConnNetRes, CloseNetRes, RecvNetRes, SendNetRes,
// 返回不同的event_type,见EventEngine/EventData.h
//

#ifndef TRANSMASTER_NETRESPONSE_H
#define TRANSMASTER_NETRESPONSE_H

#include "NetType.h"
#include "NetPacket.h"
#include "EventEngine/EventData.h"

class NetResponse
        : public EventData
{
public:

    virtual ~NetResponse(void);

    virtual const EDType GetType() const = 0;

    sock_t GetSrcSock() const
    {
        return m_src_sockfd;
    };

    void SetResult(bool res, int sock_errno = 0, int protl_errno = 0)
    {
        m_res = res;
        m_sock_errno = ( (true == res) ? 0 : sock_errno );
        m_protl_errno = ( (true == res) ? 0 : protl_errno );
    };

    bool GetRes() const
    {
        return m_res;
    };

    int GetSockErrno() const
    {
        return m_sock_errno;
    };

    int GetProtlErrno() const
    {
        return m_protl_errno;
    };

protected:
    NetResponse(void);
    NetResponse(sock_t sockfd);

    const sock_t m_src_sockfd;
    bool m_res;             //由响应方返回,标识是否接收成功
    int  m_sock_errno;
    int  m_protl_errno;

};


class ListenNetRes
        : public NetResponse
{
public:
    ListenNetRes(sock_t sockfd, sock_t clifd);
    ~ListenNetRes(void);

    const EDType GetType() const;

    sock_t GetClientSock() const
    {
        return m_client_sock;
    };

private:
    const sock_t m_client_sock;

};

class ConnNetRes
        : public NetResponse
{
public:
    ConnNetRes(sock_t sockfd);
    ~ConnNetRes(void);

    const EDType GetType() const;
};

class CloseNetRes
        : public NetResponse
{
public:
    CloseNetRes(sock_t sockfd, NetPacket * p_recv_packet = NULL, NetPacket * p_send_packet = NULL);
    ~CloseNetRes(void);

    const EDType GetType() const;

private:
    NetPacket * mp_recv_packet;
    NetPacket * mp_send_packet;
};

class RecvNetRes
        : public NetResponse
{
public:
    RecvNetRes(sock_t sockfd, NetPacket * p_recv_packet);
    ~RecvNetRes(void);

    const EDType GetType() const;

    NetPacket * GetRecvPakcet()
    {
        return mp_recv_packet;
    };

private:
    int m_protl_errno;
    NetPacket * mp_recv_packet;
};

class SendNetRes
        : public NetResponse
{
public:
    SendNetRes(sock_t sockfd, NetPacket * p_send_packet);
    ~SendNetRes(void);

    const EDType GetType() const;

    NetPacket * GetSendPacket()
    {
        return mp_send_packet;
    };

private:
    int m_protl_errno;
    NetPacket * mp_send_packet;
};

#endif //TRANSMASTER_NETRESPONSE_H
