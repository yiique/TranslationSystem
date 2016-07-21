//
// Created by 刘舒曼 on 16/7/20.
//

#ifndef PROJECT_LOGDATA_H
#define PROJECT_LOGDATA_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include "EventEngine/EventData.h"

#include <string>
using namespace std;

class LogData
        : public EventData
{
public:
    LogData(boost::thread::id tid, const string & log_content, const boost::posix_time::ptime & time_stamp );
    ~LogData(void);

    virtual const EDType GetType() const;   //实现自EventData

    const boost::thread::id & GetThreadID() const
    {
        return m_log_tid;
    };

    const string & GetContent() const
    {
        return m_log_content;
    };

    const boost::posix_time::ptime & GetTimeStamp() const
    {
        return m_time_stamp;
    };

private:
    boost::thread::id m_log_tid;    //输出日志的来源线程id
    string m_log_content;           //日志内容
    boost::posix_time::ptime m_time_stamp;  //输出时间戳
};

#endif //PROJECT_LOGDATA_H
