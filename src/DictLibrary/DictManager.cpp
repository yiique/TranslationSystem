//
// Created by 刘舒曼 on 16/9/11.
//

#include "DictManager.h"

using namespace dictionary;

// MatchResult functions
string MatchResult::serialize()
{
    //解码器会在句子前面加上<s> 所以beg, end 为最后一个词典位置
    stringstream ss;
    ss << (pos_beg+1) << ":" << (pos_end+1-1) << " ||| " << src << " ||| " << tgt << " ||| " << weight;

    return ss.str();
}

bool MatchResult::un_serialize(const string & str)
{
    vector<string> s_vec;
    split_string_by_tag(str.c_str(), s_vec, '\t');

    if(s_vec.size() != 5)
    {
        lerr << "un serialize match result failed : " << str << endl;
        return false;
    }

    src = s_vec[0];
    tgt = s_vec[1];

    pos_beg = str_2_num(s_vec[2]);
    pos_end = str_2_num(s_vec[3]);

    weight = str_2_num(s_vec[4]);
    type = 1;

    return true;
}


// DictManager functions
DictManager DictManager::ms_instance;

// public functions
bool DictManager::Initialize(const string & usr_dict_weight,
                             const string & pro_dict_weight,
                             const string & sys_dict_weight,
                             const bool & enable_remote_dict)
{
    m_enable_remote_dict = enable_remote_dict;

    if(m_enable_remote_dict)
    {
        return true;

    }else
    {
        //从持久层读取
        vector<DictInfo> dict_info_vec;
        vector<WordInfo> word_info_vec;

        if(SUCCESS != DictPersistent::LoadAllDict(dict_info_vec))
        {
            lout << "load dict from persistent failed." << endl;
            return false;
        }

        if(SUCCESS != DictPersistent::LoadAllWord(word_info_vec))
        {
            lout << "load dict from persistent failed." << endl;
            return false;
        }

        size_t dict_base_id;
        size_t word_base_id;

        if(SUCCESS != DictPersistent::GetMaxDictWordID(dict_base_id, word_base_id))
        {
            lout << "load dic/word id from persistent failed." << endl;
            return false;
        }

        {// WRITE LOCK
            WriteLockGuard guard(m_lock);

            m_usr_dict_weight = usr_dict_weight;
            m_pro_dict_weight = pro_dict_weight;
            m_sys_dict_weight = sys_dict_weight;

            m_dict_base_id = dict_base_id + 1;
            m_word_base_id = word_base_id + 1;

            for(size_t i=0; i<dict_info_vec.size(); ++i)
            {
                const DictInfo & dict_info = dict_info_vec[i];
                insert_dict(dict_info.dict_id, dict_info);

                if(dict_info.dict_id > m_dict_base_id)
                    m_dict_base_id = dict_info.dict_id;
            }

            for(size_t i=0; i<word_info_vec.size(); ++i)
            {
                const WordInfo & word_info = word_info_vec[i];
                insert_word(word_info.word_id, word_info.dict_id, word_info);

                if(word_info.word_id > m_word_base_id)
                    m_word_base_id = word_info.word_id;
            }
        } //LOCK RELEASE
    }

    return true;
}

int DictManager::CreateDict(DictInfo & dict_info)
{
    {// WRITE LOCK
        WriteLockGuard guard(m_lock);

        size_t new_dict_id = generate_unique_dictid();

        if( SUCCESS != insert_dict(new_dict_id, dict_info) )
        {
            //TODO 回收ID
            return SUCCESS;
        }

        dict_info.dict_id = new_dict_id;
    }

    return DictPersistent::CreateDict(dict_info);
}

int DictManager::DeleteDict(const DictID dict_id)
{
    {// WRITE LOCK
        WriteLockGuard guard(m_lock);

        map<DictID, dict_ptr>::iterator iter = m_dict_map.find(dict_id);

        if(iter == m_dict_map.end())
            return ERROR_UNDEFINE;

        dict_ptr sp_dict = iter->second;
        m_dict_map.erase(iter);

        for(map<WordID, word_ptr>::iterator iter = sp_dict->word_map.begin();
            iter != sp_dict->word_map.end(); ++iter)
        {
            word_ptr sp_word = iter->second;

            sp_word->dict_map.erase(sp_dict);

            if(sp_word->dict_map.empty())
                m_word_index.Erase(sp_word);
        }
    } //LOCK RELEASE

    return DictPersistent::DeleteDict(dict_id);
}

int DictManager::ModifyDict(const DictInfo & dict_info)
{
    {// WRITE LOCK
        WriteLockGuard guard(m_lock);

        map<DictID, dict_ptr>::iterator iter = m_dict_map.find(dict_info.dict_id);

        lout << "dict id : " << dict_info.dict_id << endl;

        if(iter == m_dict_map.end())
            return ERROR_UNDEFINE;

        UsrID usr_id = iter->second->dict_info.usr_id;

        dict_ptr sp_dict = iter->second;

        sp_dict->dict_info = dict_info;
        sp_dict->dict_info.usr_id = usr_id;

    } //LOCK RELEASE

    return DictPersistent::ModifyDict(dict_info);
}


int DictManager::InsertWord(const DictID dict_id, WordInfo & word_info)
{
    {// WRITE LOCK
        WriteLockGuard guard(m_lock);

        size_t new_word_id = generate_unique_wordid();

        if( SUCCESS != insert_word(new_word_id, dict_id, word_info))
        {
            //TODO 回收ID
            return ERROR_UNDEFINE;
        }

        word_info.word_id = new_word_id;

    } //LOCK RELEASE

    return DictPersistent::InsertWord(dict_id, word_info);
}

int DictManager::InsertWord(const DictID dict_id, vector<WordInfo> & word_info_vec)
{
    vector<WordInfo> suc_vec;

    for(size_t i=0; i<word_info_vec.size(); ++i)
    {
        WordInfo & word_info = word_info_vec[i];

        {// WRITE LOCK
            WriteLockGuard guard(m_lock);

            size_t new_word_id = generate_unique_wordid();
            lout << "new word id : " << new_word_id << endl;
            if( SUCCESS == insert_word(new_word_id, dict_id, word_info))
            {
                word_info.word_id = new_word_id;
                suc_vec.push_back(word_info);
            }else
            {
                //TODO 回收ID
                word_info.word_id = -1;
            }
        } //LOCK RELEASE
    }

    return DictPersistent::InsertWord(dict_id, suc_vec);
}

int DictManager::DeleteWord(const DictID dict_id, const WordID word_id)
{
    {// WRITE LOCK
        WriteLockGuard guard(m_lock);

        map<DictID, dict_ptr>::iterator diter = m_dict_map.find(dict_id);

        if(diter == m_dict_map.end())
            return ERROR_UNDEFINE;

        dict_ptr sp_dict = diter->second;

        map<WordID, word_ptr>::iterator witer = sp_dict->word_map.find(word_id);

        if(witer == sp_dict->word_map.end())
            return ERROR_UNDEFINE;

        word_ptr sp_word = witer->second;
        sp_dict->word_map.erase(witer);
        sp_word->dict_map.erase(sp_dict);

        if(sp_word->dict_map.size() == 0)
            m_word_index.Erase(sp_word);
    } //LOCK RELEASE

    return DictPersistent::DeleteWord(word_id);
}

int DictManager::ModifyWord(const DictID dict_id, WordInfo & word_info)
{
    {// WRITE LOCK
        WriteLockGuard guard(m_lock);

        map<DictID, dict_ptr>::iterator diter = m_dict_map.find(dict_id);

        if(diter == m_dict_map.end())
            return ERROR_UNDEFINE;

        dict_ptr sp_dict = diter->second;
        map<WordID, word_ptr>::iterator witer = sp_dict->word_map.find(word_info.word_id);

        if(witer == sp_dict->word_map.end())
            return ERROR_UNDEFINE;

        word_ptr sp_word = witer->second;

        if(sp_word->src == word_info.src && sp_word->tgt == word_info.tgt)
        {
            map<dict_ptr, UWord>::iterator iter = sp_word->dict_map.find(sp_dict);

            if(iter == sp_word->dict_map.end())
                return ERROR_UNDEFINE;

            UWord & uword = iter->second;
            uword.is_active = word_info.is_active;
            uword.is_checked = word_info.is_checked;
        }else
        {
            //先删除
            sp_dict->word_map.erase(witer);
            sp_word->dict_map.erase(sp_dict);

            if(sp_word->dict_map.size() == 0)
                m_word_index.Erase(sp_word);

            //再插入
            word_ptr new_sp_word(new BaseWord());

            new_sp_word->key = generate_key(word_info);
            new_sp_word->src = word_info.src;
            new_sp_word->tgt = word_info.tgt;

            new_sp_word = m_word_index.Insert(new_sp_word);

            if(new_sp_word)  //已经存在 不需要插入
            {
                UWord uword;
                uword.word_id = word_info.word_id;
                uword.is_active = word_info.is_active;
                uword.is_checked = word_info.is_checked;

                new_sp_word->dict_map.insert(make_pair(sp_dict, uword));
                sp_dict->word_map.insert(make_pair(uword.word_id, new_sp_word));
            } // else 已经存在 不需要插入
        }
    } //LOCK RELEASE

    return DictPersistent::ModifyWord(word_info);
}

int DictManager::MatchSent(const UsrID & usrid,
                           const DomainType & domain_info,
                           const bool & is_with_blank,
                           const string & sent,
                           list<MatchResult> & result_list)
{
    if(m_enable_remote_dict)
        return match_sent_remote(usrid, domain_info, sent, result_list);
    else
        return match_sent_local(usrid, domain_info, is_with_blank, sent, result_list);
}

int DictManager::MatchSent(const UsrID & usrid,
                           const DomainType & domain_info,
                           const string & sent,
                           list<MatchResult> & result_list)
{
    return MatchSent(usrid, domain_info, true, sent, result_list);
}

int DictManager::MatchSentByMaxSequence(const UsrID & usrid,
                                        const DomainType & domain_info,
                                        const bool & is_with_blank,
                                        const string & sent,
                                        list<MatchResult> & result_list)
{
    return match_sent_loacl_by_max_sequence(usrid, domain_info, is_with_blank, sent, result_list);
}



int DictManager::ToSysDict(const vector<size_t> & wordid_vec, const DictID & tgt_dict_id)
{
    vector<WordInfo> word_info_vec;

    if(SUCCESS != DictPersistent::LoadWord(wordid_vec, word_info_vec))
        return ERROR_UNDEFINE;

    if(SUCCESS == InsertWord(tgt_dict_id, word_info_vec) )
        return DictPersistent::ModifyImportStatus(wordid_vec);

    return ERROR_UNDEFINE;
}

string DictManager::DebugInfo() const
{
    stringstream ss;

    ss << "======================Debug Infomation=====================" << endl;
    ss << "Dict base id : " << m_dict_base_id << endl;
    ss << "Word base id : " << m_word_base_id << endl;

    ss << endl;

    map<DictID, dict_ptr>::const_iterator iter = m_dict_map.begin();

    for(; iter != m_dict_map.end(); ++iter)
    {
        dict_ptr sp_dict = iter->second;

        ss << sp_dict->DebugInfo() << endl;
    }

    ss << endl;

    return ss.str();
}

string Dictionary::DebugInfo()
{
    stringstream ss;

    ss << "--- Dictionary ---" << endl;
    ss << "    id         : " << dict_info.dict_id << endl;
    ss << "    name       : " << dict_info.dict_name << endl;
    ss << "    domain     : " << dict_info.domain_info.first << endl;
    ss << "    language   : " << dict_info.domain_info.second.first << " -> " << dict_info.domain_info.second.second << endl;
    ss << "    type       : " << dict_info.type << endl;
    ss << "    is_deleted : " << dict_info.is_deleted << endl;
    ss << "    is_active  : " << dict_info.is_active << endl;
    ss << "    wight      : " << weight << endl;
    ss << endl;
    ss << "  words : " << endl;

    map<WordID, word_ptr>::const_iterator iter = word_map.begin();

    for(; iter != word_map.end(); ++iter)
    {
        ss << iter->second->DebugInfo(shared_from_this()) << endl;
    }

    ss << endl;

    return ss.str();
}

// private functions
string DictManager::generate_key(const WordInfo & word_info)
{
    string key(word_info.src);
    return filter_head_tail(key);
}

int DictManager::insert_dict(const size_t new_dict_id, const DictInfo & dict_info)
{
    dict_ptr sp_dict(new Dictionary(dict_info));

    if(false == m_dict_map.insert(make_pair(new_dict_id, sp_dict)).second )
        return ERROR_UNDEFINE;

    sp_dict->dict_info.dict_id = new_dict_id;

    switch(dict_info.type)
    {
        case DICT_TYPE_USR:
            sp_dict->weight = m_usr_dict_weight;
            break;
        case DICT_TYPE_PRO:
            sp_dict->weight = m_pro_dict_weight;
            break;
        default:
            sp_dict->weight = m_sys_dict_weight;
    };

    return SUCCESS;
}

int DictManager::insert_word(const size_t new_word_id, const DictID dict_id, const WordInfo & word_info)
{
    map<DictID, dict_ptr>::iterator iter = m_dict_map.find(dict_id);

    if(iter == m_dict_map.end())
    {
        lout << "Not find dict id : " << dict_id << endl;
        return ERROR_UNDEFINE;
    }

    dict_ptr sp_dict = iter->second;

    word_ptr sp_word(new BaseWord());

    sp_word->key = generate_key(word_info);
    sp_word->src = word_info.src;
    sp_word->tgt = word_info.tgt;
    sp_word = m_word_index.Insert(sp_word);
    map<dict_ptr, UWord>::iterator diter = sp_word->dict_map.find(sp_dict);

    if( diter == sp_word->dict_map.end())
    {
        UWord uword;
        uword.word_id = new_word_id;
        uword.is_active = word_info.is_active;
        uword.is_checked = word_info.is_checked;

        sp_word->dict_map.insert(make_pair(sp_dict, uword));
        sp_dict->word_map.insert(make_pair(uword.word_id, sp_word));
    }else
    {
        return ERROR_UNDEFINE;
    }

    return SUCCESS;
}

int DictManager::match_sent_remote(const UsrID & usrid,
                                   const DomainType & domain_info,
                                   const string & sent,
                                   list<MatchResult> & result_list)
{
    siprpc::rpc_data_ptr sp_data(new siprpc::SipRpcData());

    sp_data->data_vec.push_back(usrid);
    sp_data->data_vec.push_back(domain_info.first);
    sp_data->data_vec.push_back(domain_info.second.first);
    sp_data->data_vec.push_back(domain_info.second.second);
    sp_data->data_vec.push_back(sent);

    siprpc::rpc_data_ptr sp_res = siprpc::SipRPC::Request("10.28.0.169", "10010", "glossariesmatch.do", sp_data);

    if(sp_res)
    {
        for(size_t i=0; i<sp_res->data_vec.size(); ++i)
        {
            MatchResult result;
            if(true == result.un_serialize(sp_res->data_vec[i]))
            {
                result_list.push_back(result);
            }
        }
    }else
    {
        lerr << "Error glossariesmatch.do failed ." << endl;
    }

    return SUCCESS;
}


int DictManager::match_sent_local(const UsrID & usrid,
                                  const DomainType & domain_info,
                                  const bool & is_with_blank,
                                  const string & sent,
                                  list<MatchResult> & result_list)
{
    vector<string> word_vec;
    split_string_by_blank(sent.c_str(), word_vec);

    {// READ LOCK
        ReadLockGuard guard(m_lock);
        for(size_t i=0; i<word_vec.size(); ++i)
        {
            string key;

            for(size_t len=1; i+len <= word_vec.size(); ++len)
            {
                if(len > 1 && is_with_blank)
                    key += " ";

                key += word_vec[i+len-1];

                list<word_ptr> word_list;

                m_word_index.Search(key, word_list);

                //过滤结果
                list<word_ptr>::iterator iter = word_list.begin();
                for(; iter != word_list.end(); ++iter)
                {
                    word_ptr sp_word = *iter;
                    vector<dict_ptr> sp_dict_vec = (*iter)->CheckOwner(usrid, domain_info);

                    for(vector<dict_ptr>::iterator iter = sp_dict_vec.begin(); iter != sp_dict_vec.end(); ++iter)
                    {
                        MatchResult result;
                        result.src = sp_word->src;
                        result.tgt = sp_word->tgt;
                        result.weight = (*iter)->weight;
                        result.pos_beg = i;
                        result.pos_end = i+len;
                        result.type = (*iter)->dict_info.type;
                        result_list.push_back(result);
                    }
                }
            }
        }
    } //LOCK RELEASE
    return SUCCESS;
}


int DictManager::match_sent_local(const UsrID & usrid,
                                  const DomainType & domain_info,
                                  const string & sent,
                                  list<MatchResult> & result_list)
{
    return match_sent_local(usrid, domain_info, true, sent, result_list);
}


int DictManager::match_sent_loacl_by_max_sequence(const UsrID & usrid,
                                                  const DomainType & domain_info,
                                                  const bool & is_with_blank,
                                                  const string & sent,
                                                  list<MatchResult> & result_list)
{
    match_sent_local(usrid, domain_info, is_with_blank, sent, result_list);

    // 按递减长度尝试寻找最长串,如果存在则删除冲突的串
    vector<string> word_vec;
    split_string_by_blank(sent.c_str(), word_vec);

    for (int len = word_vec.size(); len > 0; len --)
    {
        keep_max_sequence(result_list, len);
    }
    return SUCCESS;
}

void DictManager::keep_max_sequence(list<MatchResult> &result_list, int max_len)
{
    list<MatchResult> max_len_sequences;
    list<MatchResult> filter_result_list;

    // 找到互不冲突的最长序列
    for (int i = 0; i < result_list.size(); i ++)
    {
        MatchResult temp_match_result = result_list[i];
        if (temp_match_result.pos_end - temp_match_result.pos_beg == max_len)
        {
            // 如果不和已有冲突,加入max_len_sequences
            bool conflict = false;
            for (int j = 0; j < max_len_sequences.size(); j ++)
            {
                if (temp_match_result.conflict(max_len_sequences[j]))
                {
                    conflict = true;
                    break;
                }
            }
            if (!conflict)
            {
                max_len_sequences.push_back(temp_match_result);
            }
        }
    }

    // 根据最长序列过滤result_list至filter_result_list
    for (int i = 0; i < result_list.size(); i ++)
    {
        MatchResult temp_match_result = result_list[i];
        // 如果不和max_len_sequence中的任一个冲突,或者直接相等,加入filter_result_list
        bool conflict = false;
        for (int j = 0; j < max_len_sequences.size(); j ++)
        {
            if (temp_match_result.conflict(max_len_sequences[j]))
            {
                conflict = true;
                break;
            }
        }
        if (!conflict)
        {
            filter_result_list.push_back(temp_match_result);
        }
    }

    result_list = filter_result_list;
}


void DictManager::filter_match_result(list<MatchResult> & result_list)
{
    result_list.sort(MatchResult::sort_less);
    list<MatchResult>::iterator iter = result_list.begin();
    erase_conflict(result_list);
}

void DictManager::erase_conflict(list<MatchResult> & result_list)
{
    list<MatchResult>::iterator iter = result_list.begin();
    while(iter != result_list.end())
    {
        list<MatchResult>::iterator next_iter = iter;
        next_iter++;

        if(next_iter == result_list.end())
            break;

        if(iter->conflict(*next_iter))
        {
            //冲突
            result_list.erase(next_iter);
        }else
        {
            iter++;
        }
    }

    return;
}
