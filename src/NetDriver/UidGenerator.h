//
// Created by ¡ı Ê¬¸ on 16/7/22.
//

#ifndef TRANSMASTER_UIDGENERATOR_H
#define TRANSMASTER_UIDGENERATOR_H

#include <boost/noncopyable.hpp>

#include "Common/BaseMutex.h"

#include <set>
using namespace std;

class UidGenerator
        : private boost::noncopyable
{
public:
    UidGenerator(void): m_last_id(0) {}
    ~UidGenerator(void) {}

    int Generate()
    {
        MutexLockGuard guard(m_lock);

        int id = 0;
        do
        {
            id = ++m_last_id;
        }while(!m_used_id_set.insert(id).second || id < 0);

        return id;
    }

    void Recycle(const int id)
    {
        MutexLockGuard guard(m_lock);
        m_used_id_set.erase(id);
    }

private:
    MutexLock m_lock;

    int m_last_id;
    set<int> m_used_id_set;
};


#endif //TRANSMASTER_UIDGENERATOR_H
