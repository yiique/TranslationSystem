//
// Created by ¡ı Ê¬¸ on 16/7/20.
//

#include "EventEngine.h"s
#include "assert.h"

EventEngine::EventEngine(void): m_state(STATE_STOP),
                                m_input_size(DEF_INPUT_SIZE),
                                m_next_cid(NULL_CALL_ID+1)
{
}

EventEngine::~EventEngine(void)
{
}

bool EventEngine::Start(const size_t num)
{
    m_thread_num = num;
    m_state = STATE_RUN;

    for (size_t i = 0; i < m_thread_num; i ++)
    {
        m_thread_group.create_thread(boost::bind(&EventEngine::run, this));
    }
    return true;
}

int EventEngine::PostEvent(const Event &e)
{
    if (e._priority == EVENT_PRIORITY_HIGH)
    {
        m_input_blkque.PushFront(e);
    }else
    {
        m_input_blkque.PushBack(e);
    }

    return 0;
}


int EventEngine::run()
{
    vector<Event> event_vec;

    while(true)
    {
        event_vec.clear();
        m_input_blkque.PopFront(event_vec, m_input_size);

        for (size_t i = 0; i < event_vec.size(); ++i)
        {
            on_event(event_vec[i]);
        }
    }

    return 0;
}


CallID EventEngine::generate_call_id()
{
    assert(m_thread_num <= 1);

    unsigned long setin_cnt = 0;

    while(true)
    {
        CallID next_cid = ++m_next_cid;

        const pair<set<CallID>::iterator, bool> res = m_using_cid_set.insert(next_cid);

        if (true == res.second)
        {
            return next_cid;
        }else
        {
            cerr << "Generate CallID find a same cid, loop and do it again. setin_cnt = " << setin_cnt << endl;
            ++setin_cnt;
            continue;
        }
    }
}

void EventEngine::release_call_id(const CallID &cid)
{
    assert(m_thread_num <= 1);
    m_using_cid_set.erase(cid);
}