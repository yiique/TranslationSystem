//
// Created by ¡ı Ê¬¸ on 16/7/29.
//

#ifndef TRANSMASTER_TIMELOG_H
#define TRANSMASTER_TIMELOG_H

#include <boost/timer.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "Common/f_utility.h"

#include <utility>
#include <string>
#include <map>
#include <vector>
#include <sstream>
using namespace std;

class Timer
{
public:
    Timer(void) {}
    ~Timer(void) {}

    void Start()
    {
        m_timer.restart();
    }

    double Stop()
    {
        return m_timer.elapsed();
    }

private:
    boost::timer m_timer;
};


class TimeLog;

typedef boost::shared_ptr<TimeLog> time_log_share_ptr;

class TimeLog
        : public boost::enable_shared_from_this<TimeLog>,
          private boost::noncopyable
{

public:
    TimeLog(const string & name = "TimeLog", const double value = -1);
    ~TimeLog();

    TimeLog & operator [] (const string & name);
    TimeLog & operator [] (const size_t i);

    void Start();
    void Stop();

    string Serialize();

public:
    double time_value;

private:
    Timer m_timer;
    string m_name;
    map<string, time_log_share_ptr> m_time_map;
    vector<time_log_share_ptr> m_seq_vec;

    void re_count_time();

    string serialize(const size_t deep);
};

#endif //TRANSMASTER_TIMELOG_H
