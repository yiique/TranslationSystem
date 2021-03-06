//
// Created by ������ on 16/7/21.
//

#include "NetResponse.h"

//NetResponse
NetResponse::NetResponse(void): m_src_sockfd(0)
{
}

NetResponse::NetResponse(sock_t sockfd): m_src_sockfd(sockfd),
                                         m_res(true),
                                         m_sock_errno(0),
                                         m_protl_errno(0)
{
}

NetResponse::~NetResponse(void)
{
}

//ListenNetRes
ListenNetRes::ListenNetRes(sock_t sockfd, sock_t clifd): NetResponse(sockfd),
                                                         m_client_sock(clifd)
{
}

ListenNetRes::~ListenNetRes(void)
{
}

const EDType ListenNetRes::GetType() const
{
    return EDTYPE_LISTEN_NET_RES;
}

//ConnNetRes
ConnNetRes::ConnNetRes(sock_t sockfd): NetResponse(sockfd)
{
}

ConnNetRes::~ConnNetRes(void)
{
}

const EDType ConnNetRes::GetType() const
{
    return EDTYPE_CONN_NET_RES;
}

//CloseNetRes
CloseNetRes::CloseNetRes(sock_t sockfd, NetPacket * p_recv_packet, NetPacket * p_send_packet): NetResponse(sockfd),
                                                                                               mp_recv_packet(p_recv_packet),
                                                                                               mp_send_packet(p_send_packet)
{
};

CloseNetRes::~CloseNetRes(void)
{
}

const EDType CloseNetRes::GetType() const
{
    return EDTYPE_CLOSE_NET_RES;
}

//RecvNetRes
RecvNetRes::RecvNetRes(sock_t sockfd, NetPacket * p_recv_packet): NetResponse(sockfd),
                                                                  mp_recv_packet(p_recv_packet)
{
}

RecvNetRes::~RecvNetRes(void)
{
}

const EDType RecvNetRes::GetType() const
{
    return EDTYPE_RECV_NET_RES;
}

//SendNetRes
SendNetRes::SendNetRes(sock_t sockfd, NetPacket * p_send_packet): NetResponse(sockfd),
                                                                  mp_send_packet(p_send_packet)
{
}

SendNetRes::~SendNetRes(void)
{
}

const EDType SendNetRes::GetType() const
{
    return EDTYPE_SEND_NET_RES;
}