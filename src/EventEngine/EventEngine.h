//
// Created by 刘舒曼 on 16/7/20.
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


// Event优先级
typedef enum
{
    EVENT_PRIORITY_HIGH,
    EVENT_PRIORITY_NORMAL
} EventPriority;


class Event
{
public:
    EventData * _p_edata;       // 事件数据
    EventEngine * _p_caller;    // 事件发送方
    EventPriority _priority;    // 事件优先级

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
    //事件状态类
    typedef enum {STATE_STOP, STATE_RUN} state_t;
    //默认事件读取缓冲区大小,0为不限制大小,全部取空(BlockingDeque中决定)
    static const size_t DEF_INPUT_SIZE = 1;

    BlockingDeque<Event> m_input_blkque;    //事件阻塞队列
    state_t m_state;                        //当前运行状态
    size_t m_input_size;                    //从队列里读取的最大事件数
    size_t m_thread_num;                    //工作线程数
    boost::thread_group m_thread_group;

    //工作线程的循环函数,循环从事件队列里读取事件,正常情况不会返回
    virtual int run();
    //事件处理函数,由子类实现
    virtual void on_event(Event &e) = 0;

    //CallID产生器,非线程安全,当m_thread_num大于1时禁用
    CallID m_next_cid;
    set<CallID> m_using_cid_set;

    CallID generate_call_id();
    void release_call_id(const CallID & cid);

public:
    EventEngine(void);
    virtual ~EventEngine(void);

    //启动EventEngine,Start内部会产生一个工作线程,然后返回
    virtual bool Start(const size_t num = 1);

    //向EventEngine插入一个待处理事件e,返回0表示出错
    int PostEvent(const Event &e);
};

#endif //PROJECT_EVENTENGINE_H
