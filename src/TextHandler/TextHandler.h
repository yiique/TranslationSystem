//
// Created by 刘舒曼 on 16/8/24.
//

#ifndef TRANSMASTER_TEXTHANDLER_H
#define TRANSMASTER_TEXTHANDLER_H


#include "EventEngine/EventEngine.h"
#include "Log/LogStream.h"
#include "Common/ByteBuffer.h"
#include "DataBase/DBOperation.h"
#include "TextParser/TxtParser.h"
#include "TransText/TransText.h"
#include "TextHandler/TextHandlerEvent.h"

#include "LanguageTools/CLanguage.h"
#include "Common/ICUConverter.h"
#include "TinyXml/tinyxml.h"

#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")


class TextHandler
        : public EventEngine
{
public:
    ~TextHandler(void) {}

    static TextHandler * GetInstance() {return &ms_instance;}

    bool Start(const size_t thread_num = 1, const string & default_file_path = "./");

    // 分别向队列中插入TextLoadReq,TextCreateReq和TextSubmitReq
    void LoadTransText(TransText * p_trans_text, const CallID cid, EventEngine * p_caller);
    void CreateResult(const TextID & tid, const CallID cid, const string & download_type, const bool is_bilingual, EventEngine * p_caller);
    void SubmitResult(TransText * p_text, const CallID cid, EventEngine * p_caller);

private:
    TextHandler(): m_thread_num(0) {}

    //禁用基类start函数，本类重新实现，用于启动多个线程
    bool Start(void) {return false;}

    //实现基类事件处理函数,根据提交的请求分别执行on_load/create/submit_result,并向发送方事件队列返回结果
    void on_event(Event & e);

    // 1 读取text信息; 2 转换text格式(txt,word,html,pdf); 3 解析内容; 4 提交摘要
    EventData * on_load_text(EventData * p_edata);
    // 1 生成结果文件
    EventData * on_create_result(EventData * p_edata);
    // 1 提交结果文件
    EventData * on_submit_result(EventData * p_edata);

    // on_load_text_2
    int convert_txt_to_text(const string & filepath, string & text);
    int convert_word_to_text(const string & filepath, const TextID & tid, const string & file_type, string & text);
    int convert_html_to_text(const string & filepath, const TextID & tid, string & text);
    int convert_pdf_to_text(const string & filepath, const TextID & tid, string & text);
    // on_load_text_3   1 编码格式转换 2 文本格式解析,分段 3 保存base pos/source
    int build_base_source(const string & text, TransText & trans_text);

    // on_create_result_1
    int build_tmx_result_file(const TextID & tid, string & result_file_name);   // 生成tmx格式
    int build_result_file(const TextID & tid, const bool is_bilingual, string & result_file_name);  // 根据TEXT_TYPE选择类型
    int build_text_result_file(const TextID & tid,
                               const bool is_bilingual,
                               string & result_file_name); //调用build_base_result, 生成txt类型的结果
    int build_detail_result_file(const TextID & tid, string & result_file_name);    // 生成详细的tmx格式

    // tool functions for build result
    int build_base_result_file(const TextID & tid,
                               const string & build_id,
                               const vector<boost::tuple<string, string, string, size_t>> & result_vec,
                                const bool is_blank_between_sent,
                                const bool is_bilingual,
                                string & base_result_file_path,
                                string & result_text);

    // 调用jar wordDocumentExtractor生成wordresult
    int build_txt_result_file(const TextID & tid,
                              const string & build_id,
                              const string & result_text,
                              string & result_file_name);

    int build_word_result_file(const TextID & tid,
                               const string & build_id,
                               const string & file_type,
                               const string & result_text,
                               string & result_file_name);

    // 调用jar htmlAnalysis生成html
    int build_html_result_file(const TextID & tid,
                               const string & build_id,
                               const string & base_result_file_path,
                               string & result_file_name);

    int build_pdf_result_file(const TextID & tid,
                              const string & build_id,
                              const string & result_text,
                              string & result_file_name);

    // tool functions
    // resize data的大小,将file读入data
    bool read_file(const string & file_path, ByteBuffer & data);
    bool write_file(const string & file_path, const char * p_data, const size_t len);

    string & del_utf8_bom(string & src);    // 去掉utf-8 bom头
    void charset_handle(string & text);


private:
    static TextHandler ms_instance;

    size_t m_thread_num;
    string m_default_file_path;
    string m_word_workspace;
};

#endif //TRANSMASTER_TEXTHANDLER_H