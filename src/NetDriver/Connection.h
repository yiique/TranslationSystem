//
// Created by 刘舒曼 on 16/7/22.
//

#ifndef TRANSMASTER_CONNECTION_H
#define TRANSMASTER_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/function.hpp>

#include "NetType.h"
#include "NetPacket.h"
#include "NetResponse.h"
#include "EventEngine/EventEngine.h"
#include "Log/LogStream.h"

#include <map>
#include <string>
#include <iostream>
using namespace std;

class Connection
        : public boost::enable_shared_from_this<Connection>,
          private boost::noncopyable
{
public:
    explicit Connection(const sock_t sockfd,
                        EventEngine * p_owner,
                        boost::asio::io_service & io_service);
    ~Connection();

    sock_t Sockfd() const {return m_sockfd;}
    boost::asio::ip::tcp::socket & Socket() {return m_socket;}

    void Connect(const string & ip, const string & port, EventEngine * p_caller, CallID cid);
    void Recv(NetPacket * p_packet, EventEngine * p_caller, CallID cid);
    void Send(NetPacket * p_packet, EventEngine * p_caller, CallID cid);
    void Stop();

private:
    boost::asio::io_service & m_io_service;

    sock_t m_sockfd;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::resolver m_resolver;

    boost::array<char, 18192> m_buffer;

    CallID m_callid;
    EventEngine * mp_caller;    //发送消息给该connection的发送方,调用者
    EventEngine * mp_owner;     //connection持有者,构造时决定

    NetPacket * mp_recv_packet;
    NetPacket * mp_send_packet;

    string m_endpoint;

private:
    void handle_resolve(const boost::system::error_code & err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const boost::system::error_code & err);
    void handle_read(const boost::system::error_code & e, size_t bytes_transferred);
    void handle_write(const boost::system::error_code & e);
    boost::logic::tribool parse_recv_data(const char * data, const size_t len);

    void handle_connect_success();
    void handle_recv_success();
    void handle_send_success();

    void handle_connect_error();
    void handle_recv_error();
    void handle_send_error();
};

typedef boost::shared_ptr<Connection> connection_ptr;

//多线程connection管理器
class ConnectionManager
        : private boost::noncopyable
{
public:
    bool Start(connection_ptr sp_conn);
    bool Stop(const sock_t sockfd);

    connection_ptr Find(const sock_t sockfd);

private:
    map<sock_t, connection_ptr> m_connection_map;
};

#endif //TRANSMASTER_CONNECTION_H
