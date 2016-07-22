//
// Created by ¡ı Ê¬¸ on 16/7/22.
// ª•≥‚À¯
//

#ifndef TRANSMASTER_BASEMUTEX_H
#define TRANSMASTER_BASEMUTEX_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

class MutexLock
        : public boost::noncopyable
{
public:
    MutexLock() {}
    ~MutexLock() {}

    void Lock()
    {
        m_mutex.lock();
    }

    void Unlock()
    {
        m_mutex.unlock();
    }

    boost::mutex & GetMutexRef()
    {
        return m_mutex;
    }

private:
    boost::mutex m_mutex;
};

class MutexLockGuard
        : public boost::noncopyable
{
public:
    explicit MutexLockGuard(MutexLock & mutex) : m_lock_guard(mutex.GetMutexRef()) {}

    ~MutexLockGuard() {}

private:
    boost::lock_guard<boost::mutex> m_lock_guard;
};

#endif //TRANSMASTER_BASEMUTEX_H
