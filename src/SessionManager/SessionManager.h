//
// Created by ������ on 16/7/26.
//

#ifndef TRANSMASTER_SESSIONMANAGER_H
#define TRANSMASTER_SESSIONMANAGER_H

#include "EventEngine/EventEngine.h"
#include "Log/LogStream.h"
#include "NetDriver/AsyncNetDriver.h"
#include "CSession.h"

#include <iostream>
#include <string>
#include <map>
using namespace std;

class SessionManager
        : public EventEngine
{
public:
    SessionManager(AsyncNetDriver * p_net_driver);
    virtual ~SessionManager(void);

    //�����û������˿�
    bool SetListen(const char *ip, const int port);

    //��д����Start����
    bool Start(void);

protected:
    //�����¼�������
    void on_event(Event & e);
    //TODO �ڴ�й©���� ����Է�Ͷ���˵�event�а���һ��NEW�ĳ�Աָ�룬���eventû���ҵ�session�����޷�����Ҳ���޷����ٴ�ָ��
    virtual void on_no_session_event(Event & e); //����û���ҵ�session���¼�

    //�����¼�
    virtual int on_listen_newsock(CSession & session, EventData * p_edata); //�����µ����ӵ�socket fd�����󷵻�-1
    void on_recv_packet(CSession & session, EventData * p_edata);
    void on_send_packet(CSession & session, EventData * p_edata);
    void on_close_sock(CSession & session, EventData * p_edata);

    //���ݽ���packet�����ͷ��ض�Ӧ�Ĵ������̣���������ڴ����ͣ��򷵻�NULL
    virtual CProcess * create_process(CSession & session);
    void start_process(CSession & session);
    //ҵ�����̴���
    void on_proc_event(CSession & session, EventData * p_edata);
    virtual void end_process(CSession & session);

protected:
    string m_listen_ip;             //����ip
    int m_listen_port;              //�����˿�

    AsyncNetDriver & m_net_driver;  //����ӿ�

    typedef map<CallID, CSession> session_map_t;
    session_map_t m_session_map;
};

#endif //TRANSMASTER_SESSIONMANAGER_H
