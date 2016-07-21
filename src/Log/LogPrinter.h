//
// Created by 刘舒曼 on 16/7/20.
//

#ifndef PROJECT_LOGPRINTER_H
#define PROJECT_LOGPRINTER_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.cpp>

#include "EventEngine/EventEngine.h"
#include "LogData.h"
#include "Common/f_utility.h"

#include <fstream>
#include <string>
#include <map>
#include <iostream>
using namespace std;

class LogPrinter
        : public EventEngine
{
public:
    ~LogPrinter(void);

    static LogPrinter * GetInstance();

    bool Start(bool is_std_io = true);

private:
    static LogPrinter ms_log_instance;  //唯一实例
    bool m_is_start;                    //启动标识,避免重复启动
    bool m_is_std_io;                   //是否同时输出到std
    ofstream m_log_file;                //输出流
    unsigned long m_log_cnt;            //日志行数

    //构造函数私有化,单例模式
    LogPrinter(void);
    //事件处理函数
    void on_event(Event & e);
    //打印日志
    void print_data(const LogData & data);

};

#endif //PROJECT_LOGPRINTER_H
