//
// Created by ������ on 16/7/20.
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
    static LogPrinter ms_log_instance;  //Ψһʵ��
    bool m_is_start;                    //������ʶ,�����ظ�����
    bool m_is_std_io;                   //�Ƿ�ͬʱ�����std
    ofstream m_log_file;                //�����
    unsigned long m_log_cnt;            //��־����

    //���캯��˽�л�,����ģʽ
    LogPrinter(void);
    //�¼�������
    void on_event(Event & e);
    //��ӡ��־
    void print_data(const LogData & data);

};

#endif //PROJECT_LOGPRINTER_H
