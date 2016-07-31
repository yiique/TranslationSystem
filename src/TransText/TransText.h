//
// Created by 刘舒曼 on 16/7/29.
//

#ifndef TRANSMASTER_TRANSTEXT_H
#define TRANSMASTER_TRANSTEXT_H

#include "Log/LogStream.h"
#include "Common/BasicType.h"
#include "Common/BaseMutex.h"
#include "Common/f_utility.h"
#include "Common/CSequence.h"
#include "Common/Utf8Stream.h"
#include "TextFormat.h"
#include "TimeLog.h"
#include "TinyXml/tinyxml.h"
#include "LanguageTools/CLanguage.h"
#include "LanguageTools/SentSpliter.h"

#include <boost/function.hpp>

#include <vector>
#include <iostream>
#include <map>
#include <utility>
#include <string>
using namespace std;

#define TEXT_TYPE_TXT "txt"
#define TEXT_TYPE_DOC "doc"
#define TEXT_TYPE_DOCX "docx"
#define TEXT_TYPE_HTML "html"
#define TEXT_TYPE_PDF  "pdf"

//对齐关系
class BaseAlign
{
public:
    size_t src_offset;
    size_t src_size;

    size_t tgt_offset;
    size_t tgt_size;


    BaseAlign(const size_t _src_offset = 0,
              const size_t _src_size = 0,
              const size_t _tgt_offset = 0,
              const size_t _tgt_size = 0): src_offset(_src_offset), src_size(_src_size), tgt_offset(_tgt_offset), tgt_size(_tgt_size)
    {}

    static string & Serialize(const vector<BaseAlign> & align_vec, string & tgt)
    {
        stringstream ss;

        for(size_t i=0; i<align_vec.size(); ++i)
        {
            const BaseAlign & align = align_vec[i];

            ss << align.src_offset << " " << align.src_size << " " << align.tgt_offset << " " << align.tgt_size << "\t";
        }

        tgt = ss.str();
        return tgt;
    }

    static void UnSerialize(const string & tgt, vector<BaseAlign> & align_vec)
    {
        align_vec.clear();

        vector<string> line_vec;
        split_string_by_tag(tgt.c_str(), line_vec, '\t');

        for(size_t i=0; i<line_vec.size(); ++i)
        {
            vector<string> tmp_vec;
            split_string_by_tag(line_vec[i].c_str(), tmp_vec, ' ');

            if(tmp_vec.size() == 4)
            {
                align_vec.push_back(BaseAlign(str_2_num(tmp_vec[0]), str_2_num(tmp_vec[1]), str_2_num(tmp_vec[2]), str_2_num(tmp_vec[3])));
            }
        }
    }
};


class TransAlignment
{
public:

    //原文
    string src;

    //原文 - a2b结果
    vector<BaseAlign> src_a2b_vec;      //长度为字符数

    //a2b结果 - 分词结果
    vector<BaseAlign> a2b_seg_vec;      //长度为词数

    //分词结果 - 断句结果
    vector<BaseAlign> seg_splite_vec;   //长度为句子数

    class SentAlignment
    {
    public:
        string src;
        string tgt;
        vector<BaseAlign> align_vec;        //原文到词的对齐
        vector<BaseAlign> final_align_vec;
    };

    vector<SentAlignment> sent_vec;
};


//  翻译src & tgt
class TransSrc
{
public:
    TextID _text_id;
    size_t _cell_id;

    string _src_str;                    //源文本
    string _pre_proc_result;            //前处理结果, Segment结果

    vector<string> _dict_result_vec;    //rules

    mutable size_t _trans_times;        //被翻译的次数
public:
    TransSrc(const TextID & text_id = "NULL",
             const size_t cell_id = 0): _text_id(text_id),
                                        _cell_id(cell_id),
                                        _trans_times(0){} ;
    ~TransSrc(void) {};

    string Rules() const
    {
        string rules;

        for(size_t i=0; i<_dict_result_vec.size(); ++i)
        {
            rules += _dict_result_vec[i] + " ; ";
        }

        return rules;
    }
} ;

class TransTgt
{
public:
    TextID _text_id;            //全局唯一的文本id
    size_t _cell_id;            //在cell_vec中的下标

    string _decode_result;      //解码后的结果
    string _tgt_str;            //翻译结果

    string _assist_tgt;
    string _assist_str;
    string _assist2_str;

    vector<BaseAlign> _decode_align_vec;

    vector<string> _oov_vec;
    string _oov_sent;
#ifdef OUTPUT_TRANS_TIME
    double _trans_time;
#endif
public:
    TransTgt(const TextID & text_id,
             const size_t cell_id): _text_id(text_id),
                                    _cell_id(cell_id) {} ;
    ~TransTgt(void) {};
} ;


class SentProcCell
{
public:
    const string & usr_id;
    const DomainType & domain;
    TransSrc & trans_src;
    TransTgt & trans_tgt;

public:
    SentProcCell(const string & _usr_id,
                 const DomainType & _domain,
                 TransSrc & _trans_src,
                 TransTgt & _trans_tgt): usr_id(_usr_id),
                                         domain(_domain),
                                         trans_src(_trans_src),
                                         trans_tgt(_trans_tgt) {}
    ~SentProcCell() {}
};


typedef boost::function<bool (string &, TransAlignment &)> PreTextHandler;
typedef boost::function<bool (const string & , vector<string> &, TransAlignment &)> SpliteSentHandler;
typedef boost::function<bool (SentProcCell &)> SentHandler;


class TransInfo
{
public:
    TransInfo(const TextID & tid): m_text_id(tid) {}
    ~TransInfo(void) {}

    void SetSrc(const string & src) { m_src = src; }
    void SetTransResult(const string & trans_res) { m_tgt = trans_res; }
    void SetDomain(const string & domain) { m_domain = domain; }
    void SetSrcLanguage(const string & src_language) { m_src_language = src_language; }
    void SetTgtLanguage(const string & tgt_language) { m_tgt_language = tgt_language; }

    bool SetSubSrc(const size_t para_idx, const size_t sent_idx, const string & sub_src)
    {
        if(para_idx >= m_para_vec.size())
            return false;

        if(sent_idx >= m_para_vec[para_idx]._sub_sent_vec.size())
            return false;

        m_para_vec[para_idx]._sub_sent_vec[sent_idx]._sent_src = sub_src;
        return true;
    }

    bool SetTransResult(const size_t para_idx, const size_t sent_idx, const string & trans_res)
    {
        if(para_idx >= m_para_vec.size())
            return false;

        if(sent_idx >= m_para_vec[para_idx]._sub_sent_vec.size())
            return false;

        m_para_vec[para_idx]._sub_sent_vec[sent_idx]._trans_res = trans_res;
        return true;
    }


    void BuildPara(const vector<string> & para_str_vec)
    {
        m_para_vec.clear();

        for(size_t i = 0; i<para_str_vec.size(); ++i)
        {
            ParaLog para;
            m_para_vec.push_back(para);
            m_para_vec.rbegin()->_para_src = para_str_vec[i];
        }
    }

    bool BuildSubSent(const size_t para_idx, const vector<string> & sub_sent_vec)
    {
        if(para_idx >= m_para_vec.size())
            return false;

        m_para_vec[para_idx]._sub_sent_vec.clear();

        for(size_t i=0; i<sub_sent_vec.size(); ++i)
        {
            SentLog sent;
            m_para_vec[para_idx]._sub_sent_vec.push_back(sent);
            m_para_vec[para_idx]._sub_sent_vec.rbegin()->_sent_src = sub_sent_vec[i];
        }

        return true;
    }

    void SetTextTransTime(const double trans_time)
    {
#ifdef OUTPUT_TRANS_TIME
        m_trans_time = trans_time;
#endif
    }

    void SetSentTransTime(const size_t para_idx, const size_t sent_idx, const double trans_time)
    {
#ifdef OUTPUT_TRANS_TIME
        if(para_idx >= m_para_vec.size())
			return ;

		if(sent_idx >= m_para_vec[para_idx]._sub_sent_vec.size())
			return ;

		m_para_vec[para_idx]._sub_sent_vec[sent_idx].m_trans_time = trans_time;
#endif
    }

private:
    const TextID m_text_id;
    string m_src;
    string m_tgt;
    string m_domain;
    string m_src_language;
    string m_tgt_language;

    vector<ParaLog> m_para_vec;

#ifdef OUTPUT_TRANS_TIME
    double m_trans_time;
#endif

    typedef struct sentlog
    {
        string _sent_src;
        string _trans_res;

#ifdef OUTPUT_TRANS_TIME
        double m_trans_time;
#endif

    } SentLog;

    typedef struct paralog
    {
        string _para_src;
        vector<SentLog> _sub_sent_vec;
    } ParaLog;
};


class TransText
{
public:
    TransText(const TransType trans_type, const TextID & tid);
    ~TransText(void);

    const string & GetUsrID() const;
    const TextID & GetID() const;
    TransType GetTransType() const;
    const DomainType & GetDomain() const;
    const string & GetDomainName() const;
    const string & GetSrcLanguage() const;
    const string & GetTgtLanguage() const;
    const string & GetSrcData() const;
    size_t GetCurrTransSize() const;


    const TextFormat & GetFromat() const;
    bool GetTgtText(string & tgt_text) const;
    bool GetTgtDetail(string & tgt_text) const;
    bool GetTgtDetail2(string & tgt_text) const;
    size_t GetTransSize() const;
    const TransCell & GetTransCell(const size_t idx) const;
    const TransSrc & GetTransSrc(const size_t idx) const;
    const TransTgt & GetTransTgt(const size_t idx) const;
    bool getPreResult(string& tgt_text) const;
    void GetFinishSent(const size_t offset, vector<pair<long, pair<string, string> > > & sent_vec);
    SubLanguageType SubSrcLanguage() const;


    void SetUsrID(const string & usr_id);
    void SetTextType(const string & text_type);
    void SetDomain(const string & domain_name, const string & src_language, const string & tgt_language);
    void SetDomainName(const string & domain_name);
    void SetSrcLanguage(const string & src_language);
    void SetTgtLanguage(const string & tgt_language);
    void SetFilePath(const string & file_path);
    void SetFormatSrc(const vector<string> & para_vec, const vector<pair<size_t, size_t> > & pos_vec);
    bool SetTransResult(const TransTgt & trans_tgt);
    bool SetSentUniqID(const size_t idx, const SentUniqID suid);
    SubLanguageType SetSubSrcLanguage(const int type);


    void ResetSrcData(const string & src_data);
    static void ResetBaseSentID(const long id);


    static bool String2TextID(const string & str, TextID & tid);

    //生成全局唯一TextID --》uuid
    static TextID GenerateTextUUID();

    string Abstract() const;

    void PrintDetail() const;

    bool IsAllTrans() const;

    size_t CountCharacter();


    //去掉源端词性标注
    void RemoveSrcPosTag();

    bool PreProcess(PreTextHandler    pre_text_handler,
                    SpliteSentHandler splite_sent_handler,
                    SentHandler    pre_sent_handler);

    bool PostProcess(SentHandler  post_sent_handler);

    bool CreateAlignment(string & format_str, string & align_str);

protected:
    //去掉中文的词性标记
    void remove_pos_tag(string & src);
    void remove_pos_tag(const string & src, string & tgt);


    string m_usr_id;
    const TextID m_text_id;         //全局唯一文本id号  与数据中的id号相同。
    string m_text_type;
    const TransType m_trans_type;
    DomainType m_domain;            //领域
    string m_src_data;              //原始文本数据
    string m_file_path;
    size_t m_trans_cnt;             //已翻译计数

    /* mutable FormatText m_format_text; 改为在远端服务器处理格式 */
    mutable TextFormat m_format;        //文本格式信息
    vector<TransAlignment> m_align_vec; //对齐情况,每个元素为一个段落
    mutable TransInfo m_trans_info;     //Translate 信息 记录中间结果
    vector<TransCell> m_trans_cell_vec;

    SubLanguageType m_sub_src_language_type;    //源端语种子类型

    //翻译单元
    class TransCell
    {
    public:
        TransCell(const TextID & text_id,
                  const SentUniqID sent_id,
                  const size_t cell_id,
                  const pair<size_t, size_t> para_idx): _sent_uid(sent_id),
                                                        _trans_src(text_id, cell_id),
                                                        _trans_tgt(text_id, cell_id),
                                                        _cell_id(cell_id),
                                                        _para_idx(para_idx),
                                                        _is_trans(false) {};
        ~TransCell(void) {};

        TransSrc _trans_src;
        TransTgt _trans_tgt;

        size_t _cell_id;
        pair<size_t, size_t> _para_idx; //段落偏移量
        SentUniqID _sent_uid;
        bool _is_trans;
    } ;

private:
    static long ms_base_sent_id;
    static MutexLock ms_sid_mutex;

    static SentUniqID generate_sent_id();

public:
#ifdef ENABLE_TIME_LOG
    mutable TimeLog time_log;
#endif //ENABLE_TIME_LOG
};

#endif //TRANSMASTER_TRANSTEXT_H
