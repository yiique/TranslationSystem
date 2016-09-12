//
// Created by 刘舒曼 on 16/9/11.
//

#ifndef TRANSMASTER_DICTMANAGER_H
#define TRANSMASTER_DICTMANAGER_H

#include "Common/ErrorCode.h"
#include "Common/f_utility.h"
#include "Common/RWLock.h"
#include "Log/LogStream.h"
#include "SipRPC/SipRPC.h"
#include "DictDef.h"
#include "WordIndex.h"
#include "Dictionary.h"
#include "DictPersistent.h"

#include <algorithm>
#include <list>
#include <fstream>
#include <sstream>

using namespace std;

namespace dictionary
{
    // 描述一个匹配结果的类
    class MatchResult
    {
    public:
        string src;
        string tgt;

        size_t pos_beg;
        size_t pos_end;

        DictType type;
        string weight;

        static bool sort_less(const MatchResult & a, const MatchResult & b)
        {
            if( a.pos_beg < b.pos_beg )
                return true;
            else if( a.pos_beg == b.pos_beg )
                return a.pos_end > b.pos_end;
            else
                return false;
        }

        bool conflict(const MatchResult & other)
        {
            if(other.pos_beg >= pos_end || other.pos_end <= pos_beg)
                return false;
            else
                return true;
        }

    public:
        string serialize();
        bool un_serialize(const string & str);
    };


    class DictManager
    {
    public:
        ~DictManager(void) {}

        static DictManager * GetInstance() { return &ms_instance; }

        // 初始化,将dict&word读入DictManager的m_dict_map中
        bool Initialize(const string & usr_dict_weight = "100 100 100 100 ",
                        const string & pro_dict_weight = "50 50 50 50 ",
                        const string & sys_dict_weight = "20 20 20 20 ",
                        const bool & enable_remote_dict = false);

        int CreateDict(DictInfo & dict_info);       // 向m_dict_map插入新字典
        int DeleteDict(const DictID dict_id);       // 从m_dict_map中删除dict,同时删除dict中每一个word关于这个dict的索引
        int ModifyDict(const DictInfo & dict_info); // 修改dictinfo

        int InsertWord(const DictID dict_id, WordInfo & word_info);             // 插入new word
        int InsertWord(const DictID dict_id, vector<WordInfo> & word_info_vec); // 插入一个vector的new word
        int DeleteWord(const DictID dict_id, const WordID word_id);             // 词典/baseword 双向删除
        int ModifyWord(const DictID dict_id, WordInfo & word_info);             // 修改word info,如果dict变化,需要先删除再插入

        int MatchSent(const UsrID & usrid,
                      const DomainType & domain_info,
                      const bool & is_with_blank,
                      const string & sent,
                      list<MatchResult> & result_list);
        int MatchSent(const UsrID & usrid,
                      const DomainType & domain_info,
                      const string & sent,
                      list<MatchResult> & result_list);
        int MatchSentByMaxSequence(const UsrID & usrid,
                                   const DomainType & domain_info,
                                   const bool & is_with_blank,
                                   const string & sent,
                                   list<MatchResult> & result_list);

        // 将word插入tgt_dict中,并修改import状态
        int ToSysDict(const vector<size_t> & wordid_vec, const DictID & tgt_dict_id);

        int RecoverWord(const DictID dict_id, const WordID word_id)
        {
            return ERROR_UNDEFINE;
        }

        int RecoverDict(const DictID dict_id)
        {
            return ERROR_UNDEFINE;
        }

        string DebugInfo() const;

    private:
        DictManager(void) {}

        size_t generate_unique_dictid() { return m_dict_base_id++; }
        size_t generate_unique_wordid() { return m_word_base_id++; }
        string generate_key(const WordInfo & word_info);

        int insert_dict(const size_t new_dict_id, const DictInfo & dict_info);
        int insert_word(const size_t new_word_id, const DictID dict_id, const WordInfo & word_info);

        // 使用siprpc匹配服务器字典
        int match_sent_remote(const UsrID & usrid,
                              const DomainType & domain_info,
                              const string & sent,
                              list<MatchResult> & result_list);
        // 匹配本地字典
        int match_sent_local(const UsrID & usrid,
                             const DomainType & domain_info,
                             const bool & is_with_blank,
                             const string & sent,
                             list<MatchResult> & result_list);
        int match_sent_local(const UsrID & usrid,
                             const DomainType & domain_info,
                             const string & sent,
                             list<MatchResult> & result_list);
        // 华城——最长序列匹配本地字典
        int match_sent_loacl_by_max_sequence(const UsrID & usrid,
                                             const DomainType & domain_info,
                                             const bool & is_with_blank,
                                             const string & sent,
                                             list<MatchResult> & result_list);
        // 递归保留最长的子序列
        void keep_max_sequence(list<MatchResult> & result_list, int max_len);

        void filter_match_result(list<MatchResult> & result_list);
        void erase_conflict(list<MatchResult> & result_list);

    private:
        static DictManager ms_instance;

        RWLock m_lock;

        size_t m_dict_base_id;
        size_t m_word_base_id;

        string m_usr_dict_weight;
        string m_pro_dict_weight;
        string m_sys_dict_weight;

        bool m_enable_remote_dict;

        WordIndex m_word_index;
        map<DictID, dict_ptr> m_dict_map;
    };
}

#endif //TRANSMASTER_DICTMANAGER_H
