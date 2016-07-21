//
// Created by ������ on 16/7/20.
//

#ifndef PROJECT_EVENTENGINE_H
#define PROJECT_EVENTENGINE_H

#include <boost/thread/thread.hpp>

#include "BlockingDeque.h"
#include "EventData.h"

#include <vector>
#include <utility>
#include <iostream>
#include <set>
using namespace std;


class EventEngine;


// Event���ȼ�
typedef enum
{
    EVENT_PRIORITY_HIGH,
    EVENT_PRIORITY_NORMAL
} EventPriority;


class Event
{
public:
    EventData * _p_edata;       // �¼�����
    EventEngine * _p_caller;    // �¼����ͷ�
    EventPriority _priority;    // �¼����ȼ�

    Event(EventData * p_edata = NULL,
            EventEngine * p_caller = NULL,
            const EventPriority priority = EVENT_PRIORITY_NORMAL): _p_edata(p_edata),
                                                                   _p_caller(p_caller),
                                                                   _priority(priority)
    {
    };

    Event(const Event & rsh)
    {
        if (&rsh != this)
        {
            _p_edata = rsh._p_edata;
            _p_caller = rsh._p_caller;
            _priority = rsh._priority;
        }
    };

    ~Event(void) {};
};


class EventEngine
{
protected:
    //�¼�״̬��
    typedef enum {STATE_STOP, STATE_RUN} state_t;
    //Ĭ���¼���ȡ��������С,0Ϊ�����ƴ�С,ȫ��ȡ��(BlockingDeque�о���)
    static const size_t DEF_INPUT_SIZE = 1;

    BlockingDeque<Event> m_input_blkque;    //�¼���������
    state_t m_state;                        //��ǰ����״̬
    size_t m_input_size;                    //�Ӷ������ȡ������¼���
    size_t m_thread_num;                    //�����߳���
    boost::thread_group m_thread_group;

    //�����̵߳�ѭ������,ѭ�����¼��������ȡ�¼�,����������᷵��
    virtual int run();
    //�¼�������,������ʵ��
    virtual void on_event(Event &e) = 0;

    //CallID������,���̰߳�ȫ,��m_thread_num����1ʱ����
    CallID m_next_cid;
    set<CallID> m_using_cid_set;

    CallID generate_call_id();
    void release_call_id(const CallID & cid);

public:
    EventEngine(void);
    virtual ~EventEngine(void);

    //����EventEngine,Start�ڲ������һ�������߳�,Ȼ�󷵻�
    virtual bool Start(const size_t num = 1);

    //��EventEngine����һ���������¼�e,����0��ʾ����
    int PostEvent(const Event &e);
};

#endif //PROJECT_EVENTENGINE_H
