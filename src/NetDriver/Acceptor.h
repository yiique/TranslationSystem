//
// Created by ¡ı Ê¬¸ on 16/7/22.
//

#ifndef TRANSMASTER_ACCEPTOR_H
#define TRANSMASTER_ACCEPTOR_H

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "NetType.h"
#include "UidGenerator.h"
#include "NetResponse.h"
#include "Connection.h"
#include "EventEngine/EventEngine.h"

#include <string>
#include <iostream>
#include <set>
#incluee <map>
using namespace std;

class Acceptor
        : private boost::noncopyable,
          public boost::enable_shared_from_this<Acceptor>
{
public:
    explicit Acceptor(const sock_t sockfd,
                      const string & ip,
                      const string & port,
                      EventEngine * p_owner,
                      ConnectionManager & connection_manager,
                      UidGenerator & uid_generator,
                      boost::asio::io_service & io_service);

    void Listen(EventEngine * p_caller, CallID cid);
    void Stop() {m_acceptor.close();}

    sock_t Sockfd() const { return m_sockfd; }

private:
    boost::asio::io_service & m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;
    sock_t m_sockfd;
    UidGenerator & m_uid_generator;

    const string & m_listen_ip;
    const string & m_listen_port;

    connection_ptr m_in_connection;
    ConnectionManager & m_connection_manager;

    CallID m_callid;
    EventEngine * mp_caller;
    EventEngine * mp_owner;

private:
    void start_accept();
    void handle_accept(const boost::system::error_code & err);

};

typedef boost::shared_ptr<Acceptor> acceptor_ptr;

class AcceptorManager
{
public:
    bool Start(acceptor_ptr sp_acceptor);
    bool Stop(const sock_t sockfd);

    acceptor_ptr Find(const sock_t sockfd);

private:
    map<sock_t, acceptor_ptr> m_acceptor_map;
};

#endif //TRANSMASTER_ACCEPTOR_H
