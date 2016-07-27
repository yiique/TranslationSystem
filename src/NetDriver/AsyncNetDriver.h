//
// Created by ������ on 16/7/22.
//

#ifndef TRANSMASTER_ASYNCNETDRIVER_H
#define TRANSMASTER_ASYNCNETDRIVER_H

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "NetType.h"
#include "NetPacket.h"
#include "UidGenerator.h"
#include "NetRequest.h"
#include "NetResponse.h"
#include "Connection.h"
#include "Acceptor.h"
#include "EventEngine/EventEngine.h"
#include "Log/LogStream.h"

#include <string>
#include <queue>
#include <map>
#include <utility>
#include <iostream>
#include <vector>
using namespace std;

typedef enum
{
    SOCKET_LISTEN,  //0
    SOCKET_CONNECT, //1
    SOCKET_RECV,    //2
    SOCKET_SEND,    //3
    SOCKET_SUSPEND, //4
    SOCKET_CLOSE    //5
} SocketState;

typedef enum
{
    PROTL_SUC,
    PROTL_ERR_NOSOCK,   //��socketδע��
    PROTL_ERR_STATE,    //�����뵱ǰ״̬��ƥ��
    PROTL_ERR_RECV,     //�������ݰ���ʽ��
    PROTL_ERR_TIMEOUT   //��ʱ
} ProtlErr;

#define DEF_MAX_EPOLL_SIZE  1000
#define RECV_BUF_SIZE 		2048000
#define PIPE_BUF_SIZE 		1024
#define TCP_STACK_SIZE      100
#define SOCK_IDLE_TIMEOUT   300


class AsyncNetDriver
        : public EventEngine,
          private boost::noncopyable
{
public:
    AsyncNetDriver(void);
    ~AsyncNetDriver(void);

    //Function: �������ض˿�
    //Return:  ��������socket�ɹ������ش�socket��������򷵻�-1����ͨ��errno��ô������
    //Event :  1�� �յ������ӣ� �¼����ݣ������Ӿ��
    //         2�� socket�����¼����ݣ��������
    sock_t Listen(const char * ip,
                  const int port,
                  EventEngine *p_caller,
                  const CallID & cid);

    //Function: ����Զ�˷�����
    //Return:  ��������socket�ɹ������ش�socket��������򷵻�-1����ͨ��errno��ô������
    //Event :  1�� ����Զ�̷������ɹ��� �¼����ݣ�NULL
    //         2�� socket�����¼����ݣ��������
    sock_t Connect(const char * ip,
                   const int port,
                   const char * cliip,
                   EventEngine *p_caller,
                   const CallID & cid);

    //Function: ��������
    //Return:  ֪ͨAsyncNetDriver�ɹ�������0���������������󣬷���-1��
    //Event :  1�� �������ݳɹ��� �¼����ݣ�NetPacket���
    //         2�� socket�����¼����ݣ��������
    //         3�� Э�������¼����ݣ��������
    int Receive(sock_t sockfd,
                NetPacket * p_packet,
                EventEngine *p_caller,
                const CallID & cid);

    //Function: ��������
    //Return:  ֪ͨAsyncNetDriver�ɹ�������0���������������󣬷���-1��
    //Event :  1�� �������ݳɹ��� �¼����ݣ�NetPacket���
    //         2�� socket�����¼����ݣ��������
    //         3�� Э�������¼����ݣ��������
    int Send(sock_t sockfd,
             NetPacket * p_packet,
             EventEngine *p_caller,
             const CallID & cid);

    //Function: ��epoll��ȥ��socket
    //Return:  ֪ͨAsyncNetDriver�ɹ�������0���������������󣬷���-1��
    //Event :  1�� �رճɹ��� �¼����ݣ�NULL
    //         2�� socket�����¼����ݣ��������
    //         3�� Э�������¼����ݣ��������
    int Shutdown(sock_t sockfd,
                 EventEngine *p_caller,
                 const CallID & cid);

    //��д����PostEvent��������Ҫ��������Ҫ����notify()����epoll
    int PostEvent(const Event & e);
    void post_handler();

private:

    //��д����run����
    int run();
    //����ԭ����on_event()����Ϊ�Ѿ���run���ء�
    void on_event(Event & e) {}

    void req_listen(EventData * p_edata, EventEngine * p_caller);
    void req_connect(EventData * p_edata, EventEngine * p_caller);
    void req_close(EventData * p_edata, EventEngine * p_caller);
    void req_recv(EventData * p_edata, EventEngine* p_caller);
    void req_send(EventData * p_edata, EventEngine* p_caller);

    void on_timer(const boost::system::error_code& error);

public:
    boost::asio::io_service m_io_service;
    boost::shared_ptr<boost::asio::io_service::work > msp_io_service_worker;
    boost::asio::deadline_timer m_timer;
    UidGenerator m_sockfd_generator;

    ConnectionManager m_connection_manager;
    AcceptorManager m_acceptor_manager;

};

#endif //TRANSMASTER_ASYNCNETDRIVER_H
