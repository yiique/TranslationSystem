//
// Created by ������ on 16/7/20.
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

    virtual const EDType GetType() const;   //ʵ����EventData

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
    boost::thread::id m_log_tid;    //�����־����Դ�߳�id
    string m_log_content;           //��־����
    boost::posix_time::ptime m_time_stamp;  //���ʱ���
};

#endif //PROJECT_LOGDATA_H
