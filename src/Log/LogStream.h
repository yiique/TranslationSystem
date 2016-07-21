//
// Created by 刘舒曼 on 16/7/21.
// 日志输出格式
//

#ifndef PROJECT_LOGSTREAM_H
#define PROJECT_LOGSTREAM_H

#include "Common/BasicType.h"
#include "LogPrinter.h"
#include "LogData.h"

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class LogStream
{
public:
    ~LogStream(void);

    static LogStream CreateLogStream();

    typedef basic_ostream<char, char_traits<char> >& (*EndlType)(basic_ostream<char, char_traits<char> >&);

    LogStream & operator << (EndlType endl_obj);
    LogStream & operator << (const DomainType & domain);
    LogStream & operator << (const ServerAddrType & addr);
    template <class Type>
    LogStream & operator << (const Type & content)
    {
        m_sstream << content;
        return *this;
    }

private:
    stringstream m_sstream; //输出缓存,遇到endl则输出

    //私有构造,拷贝和赋值
    LogStream(void);
    LogStream(const LogStream & rsh);
    LogStream & operator = (const LogStream &rsh);

    //将现有数据发送给LogPrinter
    void output();
};

class NullLogStream
{
public:
    NullLogStream(void) {};
    ~NullLogStream(void) {};

    static NullLogStream CreateNullLogStream()
    {
        NullLogStream nulllog;
        return nulllog;
    }

    typedef basic_ostream<char, char_traits<char> >& (*EndlType)(basic_ostream<char, char_traits<char> >&);

    NullLogStream & operator << (EndlType endl_obj)
    {
        //DO NOTHING
        return *this;
    }

    template <class Type>
    NullLogStream & operator << (const Type & content)
    {
        //DO NOTHING
        return *this;
    }
};

//=========================================//
//定义日志输出级别
//调试1 ldbg1  输出调试信息，如程序执行的上下文，程序中间输出结果等
//调试2 ldbg2  输出调试信息中的计划内错误，如插入删除失败，词典ID未找到，报文格式解析错误等
//普通  lout   输出常规信息，如资源读取的路径，配置文件信息等
//警告  lwrn   输出警告信息，即可以忽略的错误信息（此错误可丢弃，仅对本次业务有影响），如：收到错误的报文，错误的事件类型等
//错误  lerr   输出致命错误，即造成程序功能无法继续执行的错误，如：读取资源文件失败，程序逻辑错误等


//1. 调试1 ldbg1
#ifdef LOG_DEBUG1_OFF
#define ldbg1 ( NullLogStream::CreateNullLogStream() )
#else
#ifdef LOG_CODE_DETAIL
#define ldbg1 ( LogStream::CreateLogStream() << " [ " << (__FUNCTION__) << "() , " << (__FILE__) << " , " << (__LINE__) << " ]" << " [DEBUG_1] " )
#else
#define ldbg1 ( LogStream::CreateLogStream() << " [DEBUG_1] " )
#endif
#endif

//2. 调试 ldbg2
#ifdef LOG_DEBUG2_OFF
#define ldbg2 ( NullLogStream::CreateNullLogStream() )
#else
#ifdef LOG_CODE_DETAIL
#define ldbg2 ( LogStream::CreateLogStream() << " [ " << (__FUNCTION__) << "() , " << (__FILE__) << " , " << (__LINE__) << " ]" << " [DEBUG_2] " )
#else
#define ldbg2 ( LogStream::CreateLogStream() << " [DEBUG_2] " )
#endif
#endif

//3. 普通 lout
#ifdef LOG_INFO_OFF
#define lout ( NullLogStream::CreateNullLogStream() )
#else
#ifdef LOG_CODE_DETAIL
#define lout ( LogStream::CreateLogStream() << " [ " << (__FUNCTION__) << "() , " << (__FILE__) << " , " << (__LINE__) << " ] " )
#else
#define lout ( LogStream::CreateLogStream() )
#endif
#endif

//4. 警告 lwrn
#ifdef LOG_WARNNING_OFF
#define lwrn ( NullLogStream::CreateNullLogStream() )
#else
#ifdef LOG_CODE_DETAIL
#define lwrn ( LogStream::CreateLogStream() << " [ " << (__FUNCTION__) << "() , " << (__FILE__) << " , " << (__LINE__) << " ]" << " [WARNNING] ")
#else
#define lwrn ( LogStream::CreateLogStream() << " [WARNNING] " )
#endif
#endif


//5. 错误 lerr
#ifdef LOG_ERROR_OFF
#define lerr ( NullLogStream::CreateNullLogStream() )
#else
#ifdef LOG_CODE_DETAIL
#define lerr ( LogStream::CreateLogStream() << " [ " << (__FUNCTION__) << "() , " << (__FILE__) << " , " << (__LINE__) << " ]" << " [ERROR] ")
#else
#define lerr ( LogStream::CreateLogStream() << " [ERROR] " )
#endif
#endif

//END
//==========================================//

#endif //PROJECT_LOGSTREAM_H
