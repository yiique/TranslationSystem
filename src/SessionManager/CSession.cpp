//
// Created by ¡ı Ê¬¸ on 16/7/23.
//

#include "CSession.h"

CSession::CSession(const CallID &cid, const int sockfd) : m_cid(cid),
                                                          m_socket(sockfd),
                                                          _recv_packet(sockfd),
                                                          _send_packet(sockfd),
                                                          _p_process(NULL)
{
}

CSession::~CSession(void)
{
    if (_p_process)
        lerr << "_process is not null when CSession Destructed" << endl;
}