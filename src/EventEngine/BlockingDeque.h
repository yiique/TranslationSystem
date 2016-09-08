//
// Created by 刘舒曼 on 16/7/20.
// 线程安全阻塞队列
//

#ifndef PROJECT_BLOCKINGDEQUE_H
#define PROJECT_BLOCKINGDEQUE_H

//BOOST
#include <boost/thread/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

//STL
#include <vector>
#include <deque>
using namespace std;


template <typename elem_t>
class BlockingDeque
    : public boost::noncopyable
{
private:
    deque<elem_t> m_elem_deque;
    boost::mutex m_mutex;
    boost::condition_variable m_cond;
    size_t m_wait_count;

public:
    BlockingDeque(void) : m_wait_count(0) {};
    ~BlockingDeque(void) {};

    size_t Size();
    size_t WaitCount();

    elem_t PopFront(void);
    void PopFront(vector<elem_t> & elem_vec, const size_t max_num = 0);
    bool TryPopFront(elem_t & element);
    bool TryPopFront(vector<elem_t> & elem_vec, const size_t max_num = 0);

    elem_t PopBack(void);
    void PopBack(vector<elem_t> & elem_vec, const size_t max_num = 0);
    bool TryPopBack(elem_t & element);
    bool TryPopBack(vector<elem_t> & elem_vec, const size_t max_num = 0);

    void PushFront(const elem_t & element);
    void PushBack(const elem_t & element);

private:
    // 若队列里无elem,则等待直到取出
    void pop_element(vector<elem_t> & elem_vec, const size_t max_num, const bool is_front);
    // 若队列里有elem则取出
    void try_pop_element(vector<elem_t> & elem_vec, const size_t max_num, const bool is_front);

};


template <typename elem_t>
size_t BlockingDeque<elem_t>::Size()
{
    boost::mutex::scoped_lock lock(m_mutex);
    return m_elem_deque.size();
}

template <typename elem_t>
size_t BlockingDeque<elem_t>::WaitCount()
{
    boost::mutex::scoped_lock lock(m_mutex);
    return m_wait_count;
}

template <typename elem_t>
elem_t BlockingDeque<elem_t>::PopFront(void)
{
    vector<elem_t> elem_vec;
    this->pop_element(elem_vec, 1, true);

    return elem_vec[0];
}

template <typename elem_t>
void BlockingDeque<elem_t>::PopFront(vector <elem_t> &elem_vec, const size_t max_num)
{
    this->pop_element(elem_vec, max_num, true);
}

template <typename elem_t>
bool BlockingDeque<elem_t>::TryPopFront(elem_t &element)
{
    vector<elem_t> elem_vec;
    this->try_pop_element(elem_vec, 1, true);

    if (elem_vec.size() > 0)
    {
        element = elem_vec[0];
        return true;
    }

    return false;
}

template <typename elem_t>
bool BlockingDeque<elem_t>::TryPopFront(vector <elem_t> &elem_vec, const size_t max_num)
{
    this->try_pop_element(elem_vec, max_num, true);

    if (elem_vec.size() > 0)
        return true;
    else
        return false;
}

template <typename elem_t>
elem_t BlockingDeque<elem_t>::PopBack(void)
{
    vector<elem_t> elem_vec;
    this->pop_element(elem_vec, 1, false);

    return elem_vec[0];
}

template <typename elem_t>
void BlockingDeque<elem_t>::PopBack(vector <elem_t> &elem_vec, const size_t max_num)
{
    this->pop_element(elem_vec, max_num, false);
}

template <typename elem_t>
bool BlockingDeque<elem_t>::TryPopBack(elem_t &element)
{
    vector<elem_t> elem_vec;
    this->try_pop_element(elem_vec, 1, false);

    if(elem_vec.size() > 0)
    {
        element = elem_vec[0];
        return true;
    }

    return false;
}

template <typename elem_t>
bool BlockingDeque<elem_t>::TryPopBack(vector <elem_t> &elem_vec, const size_t max_num)
{
    this->try_pop_element(elem_vec, max_num, false);

    if (elem_vec.size() > 0)
        return true;
    else
        return false;
}

template <typename elem_t>
void BlockingDeque<elem_t>::PushFront(const elem_t &element)
{
    bool need_notify = false;

    {
        boost::mutex::scoped_lock lock(m_mutex);
        m_elem_deque.push_front(element);

        if (m_wait_count > 0)
            need_notify = true;
    }

    if (true == need_notify)
        m_cond.notify_one();
}

template <typename elem_t>
void BlockingDeque<elem_t>::PushBack(const elem_t &element)
{
    bool need_notify = false;

    {
        boost::mutex::scoped_lock lock(m_mutex);
        m_elem_deque.push_back(element);

        if (m_wait_count > 0)
            need_notify = true;
    }

    if (true == need_notify)
        m_cond.notify_one();
}


template <typename elem_t>
void BlockingDeque<elem_t>::pop_element(vector <elem_t> &elem_vec, const size_t max_num, const bool is_front)
{
    boost::mutex::scoped_lock lock(m_mutex);

    while(true)
    {
        const size_t deq_size = m_elem_deque.size();

        if (0 == deq_size)
        {
            ++m_wait_count;
            m_cond.wait(lock);
            --m_wait_count;
            continue;
        }else
        {
            size_t pop_num = 0;

            if (0 == max_num)
            {
                pop_num = deq_size;
            }else
            {
                pop_num = (deq_size < max_num) ? deq_size : max_num;
            }

            elem_vec.clear();
            elem_vec.resize(pop_num);

            if (true == is_front)
            {
                for (size_t i = 0; i < pop_num; ++i)
                {
                    elem_vec[i] = m_elem_deque.front();
                    m_elem_deque.pop_front();
                }
            }else
            {
                for (size_t i = 0; i < pop_num; ++i)
                {
                    elem_vec[i] = m_elem_deque.back();
                    m_elem_deque.pop_back();
                }
            }

            break;
        }

    }

    if (m_wait_count > 0 && m_elem_deque.size() > 0)
    {
        m_cond.notify_one();
    }
}

template <typename elem_t>
void BlockingDeque<elem_t>::try_pop_element(vector <elem_t> &elem_vec, const size_t max_num, const bool is_front)
{
    boost::mutex::scoped_lock lock(m_mutex);

    const size_t deq_size = m_elem_deque.size();
    size_t pop_num = 0;

    if ( 0 == max_num )
    {
        pop_num = deq_size;
    }else
    {
        pop_num = ( deq_size < max_num ) ? deq_size : max_num;
    }

    elem_vec.clear();
    elem_vec.resize(pop_num);

    if (true == is_front)
    {
        for (size_t i = 0; i < pop_num; ++i){
            elem_vec[i] = m_elem_deque.front();
            m_elem_deque.pop_front();
        }
    }else
    {
        for (size_t i = 0; i < pop_num; ++i){
            elem_vec[i] = m_elem_deque.back();
            m_elem_deque.pop_back();
        }
    }

    if (m_wait_count > 0 && m_elem_deque.size() > 0)
    {
        m_cond.notify_one();
    }
}

#endif //PROJECT_BLOCKINGDEQUE_H