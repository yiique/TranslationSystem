//
// Created by 刘舒曼 on 16/7/29.
//

#include "TransText.h"


//TransText
long TransText::ms_base_sent_id = 0;
MutexLock TransText::ms_sid_mutex;

TransText::TransText(const TransType trans_type, const TextID & tid): m_trans_type(trans_type),
                                                                      m_text_id(tid),
                                                                      m_trans_cnt(0),
                                                                      m_trans_info(tid),
                                                                      m_sub_src_language_type(BASE_SUB_TYPE)
{
}

TransText::~TransText(void)
{
}


// Get Attributes
const string & TransText::GetUsrID() const
{
    return m_usr_id;
}

const TextID & TransText::GetID() const
{
    return m_text_id;
}

TransType TransText::GetTransType() const
{
    return m_trans_type;
}

const DomainType & TransText::GetDomain() const
{
    return m_domain;
}

const string & TransText::GetDomainName() const
{
    return m_domain.first;
}

const string & TransText::GetSrcLanguage() const
{
    return m_domain.second.first;
}

const string & TransText::GetTgtLanguage() const
{
    return m_domain.second.second;
}

const string & TransText::GetSrcData() const
{
    return m_src_data;
}

size_t TransText::GetCurrTransSize() const
{
    return m_trans_cnt;
}


const TextFormat & TransText::GetFromat() const
{
    return m_format;
}


bool TransText::GetTgtText(string & tgt_text) const
{
    //TODO 只适用于即时翻译
    if(m_format.Size() == 0)
    {
        tgt_text = m_src_data;
        return true;
    }

    const Para & last = m_format[m_format.Size()-1];
    if(m_src_data.size() < (last._offset + last._len) )
    {
        ldbg2 << "WARNING: para offset is out src data, maybe it is a olt trans text." << endl;
        return false;
    }

    //为每个段落生成tgt,串联m_trans_cell_vec中的翻译结果,写入tgt_vec
    vector<string> para_tgt_vec;
    para_tgt_vec.resize(m_format.Size(), "");

    size_t i = 0;

    for(i=0; i<m_trans_cell_vec.size(); ++i)
    {
        const TransCell & curr_trans_cell = m_trans_cell_vec[i];

        if(i+1 < m_trans_cell_vec.size() && this->GetTgtLanguage() == LANGUAGE_ENGLISH)
            para_tgt_vec[curr_trans_cell._para_idx.first] += curr_trans_cell._trans_tgt._tgt_str + " ";
        else
            para_tgt_vec[curr_trans_cell._para_idx.first] += curr_trans_cell._trans_tgt._tgt_str;

    }

    //重新生成原文
    assert(para_tgt_vec.size() == m_format.Size());

    tgt_text.clear();
    size_t para_idx = 0;
    size_t offset = 0;
    while(offset < m_src_data.size())
    {
        if(para_idx < m_format.Size())
        {
            size_t len = m_format[para_idx]._offset - offset;

            //输出非翻译段落(offset差大于0,src中间有未翻译部分)
            if(len > 0)
            {
                tgt_text += m_src_data.substr(offset, len);
            }

            //输出翻译段落
            tgt_text += para_tgt_vec[para_idx];

            //更新偏移量
            offset = m_format[para_idx]._offset + m_format[para_idx]._len;
            ++para_idx;
        }else
        {
            size_t len = m_src_data.size() - offset;

            //输出非翻译段落
            if(len > 0)
            {
                tgt_text += m_src_data.substr(offset, len);
            }

            break;
        }
    }

    //输出info
    m_trans_info.SetTransResult(tgt_text);

    return true;
}

bool TransText::GetTgtDetail(string & tgt_text) const
{
    stringstream data;
    data << "[Total Sent Number] " << m_trans_cell_vec.size() << "\r\n";
    data << "\r\n";

    //TODO 只适用于即时翻译
    for(size_t i=0; i<m_trans_cell_vec.size(); ++i)
    {
        const TransCell & trans_cell = m_trans_cell_vec[i];

        data << "[ " << i+1 << " ]" << "\r\n";
        data << "[Segment	]	" << trans_cell._trans_src._pre_proc_result << "\r\n";

        string rules;
        for(size_t k=0; k<trans_cell._trans_src._dict_result_vec.size(); ++k)
        {
            rules += trans_cell._trans_src._dict_result_vec[k] + "\t";
        }

        data << "[Rules]	" << rules << "\r\n";
        data << "[Translate]  " << trans_cell._trans_tgt._decode_result << "\r\n";
        data << "[Post     ]  " << trans_cell._trans_tgt._tgt_str << "\r\n";
        data << "\r\n";
    }

    data << "[END]" << "\r\n" ;

    tgt_text = data.str();
    return true;
}

bool TransText::GetTgtDetail2(string & tgt_text) const
{
    stringstream data;

    //TODO 只适用于即时翻译
    for(size_t i=0; i<m_trans_cell_vec.size(); ++i)
    {
        const TransCell & trans_cell = m_trans_cell_vec[i];

        data << trans_cell._trans_src._pre_proc_result << endl;
        data << trans_cell._trans_tgt._tgt_str << endl;
    }

    tgt_text = data.str();
    return true;
}

size_t TransText::GetTransSize() const
{
    return m_trans_cell_vec.size();
}

const TransCell & TransText::GetTransCell(const size_t idx) const
{
    assert(idx < m_trans_cell_vec.size());
    return m_trans_cell_vec[idx];
}

const TransSrc & TransText::GetTransSrc(const size_t idx) const
{
    assert(idx < m_trans_cell_vec.size());
    return m_trans_cell_vec[idx]._trans_src;
}

const TransTgt & TransText::GetTransTgt(const size_t idx) const
{
    assert(idx < m_trans_cell_vec.size());
    return m_trans_cell_vec[idx]._trans_tgt;
}

//add by cqw
bool TransText::getPreResult(string& tgt_text) const
{
    tgt_text = "";
    for(int i=0; i<m_trans_cell_vec.size(); ++i){
        tgt_text += m_trans_cell_vec[i]._trans_src._pre_proc_result;
    }

    return true;
}

void TransText::GetFinishSent(const size_t offset, vector<pair<long, pair<string, string> > > & sent_vec)
{
    sent_vec.clear();

    for(size_t i=offset; i<m_trans_cell_vec.size(); ++i)
    {
        const TransCell & cell = m_trans_cell_vec[i];

        if(cell._is_trans)
        {
            pair<long, pair<string, string> > sent;
            sent.first = cell._sent_uid;
            sent.second.first = cell._trans_src._src_str;
            sent.second.second = cell._trans_tgt._tgt_str;

            sent_vec.push_back(sent);
        }
    }
}

SubLanguageType TransText::SubSrcLanguage() const
{
    return m_sub_src_language_type;
}


// Set Attributes
void TransText::SetUsrID(const string & usr_id)
{
    m_usr_id = usr_id;
}

void TransText::SetTextType(const string & text_type)
{
    m_text_type = text_type;
}

void TransText::SetDomain(const string & domain_name, const string & src_language, const string & tgt_language)
{
    m_domain.first = domain_name;
    m_domain.second.first = src_language;
    m_domain.second.second = tgt_language;

    m_trans_info.SetDomain(domain_name);
    m_trans_info.SetSrcLanguage(src_language);
    m_trans_info.SetTgtLanguage(tgt_language);
}

void TransText::SetDomainName(const string & domain_name)
{
    m_domain.first = domain_name;
    m_trans_info.SetDomain(domain_name);
}

void TransText::SetSrcLanguage(const string & src_language)
{
    m_domain.second.first = src_language;
    m_trans_info.SetSrcLanguage(src_language);
}

void TransText::SetTgtLanguage(const string & tgt_language)
{
    m_domain.second.second = tgt_language;
    m_trans_info.SetTgtLanguage(tgt_language);
}

void TransText::SetFilePath(const string & file_path)
{
    m_file_path = file_path;
}

void TransText::SetFormatSrc(const vector<string> & para_vec, const vector<pair<size_t, size_t> > & pos_vec)
{
    m_format.Reset(para_vec, pos_vec);
    m_trans_info.BuildPara(para_vec);
}

bool TransText::SetTransResult(const TransTgt & trans_tgt)
{
    assert(trans_tgt._cell_id < m_trans_cell_vec.size());
    assert(trans_tgt._text_id == m_text_id);

    TransCell & trans_cell = m_trans_cell_vec[trans_tgt._cell_id];

    if(true == trans_cell._is_trans )
    {
        ldbg2 << "This trans cell is already trans , cell_id = " << trans_tgt._cell_id << "  text_id = " << m_text_id << endl;
        return false;
    }

    trans_cell._trans_tgt = trans_tgt;
    trans_cell._is_trans = true;
    ++m_trans_cnt;

    m_trans_info.SetTransResult(trans_cell._para_idx.first, trans_cell._para_idx.second, trans_tgt._tgt_str);

#ifdef OUTPUT_TRANS_TIME
    m_trans_info.SetSentTransTime(trans_cell._para_idx.first, trans_cell._para_idx.second, trans_tgt._trans_time);
#endif

    return true;
}

bool TransText::SetSentUniqID(const size_t idx, const SentUniqID suid)
{
    assert(idx < m_trans_cell_vec.size());
    m_trans_cell_vec[idx]._sent_uid = suid;
    return true;
}

SubLanguageType TransText::SetSubSrcLanguage(const int type)
{
    return m_sub_src_language_type = (SubLanguageType) type;
}


// Reset func
void TransText::ResetSrcData(const string & src_data)
{
    //重置
    m_trans_cnt = 0;
    m_trans_cell_vec.clear();
    m_src_data = src_data;

    m_trans_info.SetSrc(src_data);
}

void TransText::ResetBaseSentID(const long id)
{
    MutexLockGuard guard(ms_sid_mutex);
    ms_base_sent_id = id;
}


// Tools func
static bool TransText::String2TextID(const string & str, TextID & tid)
{
    tid = str;
    del_head_tail_blank(tid);
    return true;
}

TextID TransText::GenerateTextUUID()
{
    //用Guid来实现唯一性控制
    return GenerateUUID();
}

string TransText::Abstract() const
{
    static size_t MAX_LEN = 100;

    size_t max_len = ((this->GetSrcLanguage() == LANGUAGE_ENGLISH) ? MAX_LEN : MAX_LEN/2 );

    Utf8Stream utf8_ss(m_src_data);

    string utf8_c;
    string abstracts;
    while(utf8_ss.ReadOneCharacter(utf8_c) && abstracts.size() < max_len)
    {
        abstracts.append(utf8_c);
    }

    return abstracts;
}

void TransText::PrintDetail() const
{
    lout << "[Domain] " << m_domain << endl;
    lout << "[Total Number] " << m_trans_cell_vec.size() << endl;
    lout << endl;
    lout << endl;

    for(size_t i=0; i<m_trans_cell_vec.size(); ++i)
    {
        lout << "[Sent " << i << " ]" << endl;
        lout << "   [   Pre     ] " << m_trans_cell_vec[i]._trans_src._pre_proc_result << endl;
        lout << "   [   Rules   ] " << m_trans_cell_vec[i]._trans_src.Rules() << endl;
        lout << "   [  Decode   ] " << m_trans_cell_vec[i]._trans_tgt._decode_result << endl;
        lout << "   [   Final   ] " << m_trans_cell_vec[i]._trans_tgt._tgt_str << endl;
        lout << "[-------]" << endl;
        lout << endl;
    }

    lout << "[END]" << endl;
}

bool TransText::IsAllTrans() const
{
    return (m_trans_cnt >= m_trans_cell_vec.size());
}

size_t TransText::CountCharacter()
{
    if(this->GetSrcLanguage() == LANGUAGE_CHINESE)
    {

        Utf8Stream utf8_stream(m_src_data);

        size_t count = 0;
        string character;
        while(utf8_stream.ReadOneCharacter(character))
        {
            count++;
        }

        return count;

    }else if(this->GetSrcLanguage() == LANGUAGE_ENGLISH)
    {
        vector<string> word_vec;
        split_string_by_blank(m_src_data.c_str(), word_vec);

        return word_vec.size();

    }else
    {
        return m_src_data.size();
    }
}


//
void TransText::RemoveSrcPosTag()
{
    //如果源端为中文，则需要去掉词性标记
    if(LANGUAGE_CHINESE == this->GetSrcLanguage())
    {
        for(size_t i=0; i<this->GetTransSize(); ++i)
        {
            remove_pos_tag(m_trans_cell_vec[i]._trans_src._src_str);
        }
    }
}

bool TransText::PreProcess(PreTextHandler    pre_text_handler,
                           SpliteSentHandler splite_sent_handler,
                           SentHandler    pre_sent_handler)
{
    //Step 1. 对整个输入做前处理
#ifdef ENABLE_TIME_LOG
    time_log[T_TOTAL][T_PER_PROCESS][T_PRE_PROCESS_TEXT].Start();
#endif //ENABLE_TIME_LOG

    m_align_vec.resize(m_format.Size());

    for(size_t i=0; i<m_format.Size(); ++i)
    {
#ifdef ENABLE_TIME_LOG
        time_log[T_TOTAL][T_PER_PROCESS][T_PRE_PROCESS_TEXT][i].Start();
#endif //ENABLE_TIME_LOG

        m_align_vec[i].src = m_format[i]._content;
        pre_text_handler(m_format[i]._content, m_align_vec[i]);

#ifdef ENABLE_TIME_LOG
        time_log[T_TOTAL][T_PER_PROCESS][T_PRE_PROCESS_TEXT][i].Stop();
#endif //ENABLE_TIME_LOG
    }

#ifdef ENABLE_TIME_LOG
    time_log[T_TOTAL][T_PER_PROCESS][T_PRE_PROCESS_TEXT].Stop();
#endif //ENABLE_TIME_LOG

    //Step 2.断句
#ifdef ENABLE_TIME_LOG
    time_log[T_TOTAL][T_PER_PROCESS][T_PER_PROCESS_SPLITE].Start();
#endif //ENABLE_TIME_LOG

    for(size_t i=0; i<m_format.Size(); ++i)
    {
        //断句
        const string & src_para = m_format[i]._content;

        vector<string> sent_vec;
        splite_sent_handler(src_para, sent_vec, m_align_vec[i]);

        assert(sent_vec.size() > 0);

        m_trans_info.BuildSubSent(i, sent_vec);

        //将断句结果生成为TransCell
        for(size_t k=0; k<sent_vec.size(); ++k)
        {

            TransCell trans_cell(m_text_id, generate_sent_id(), m_trans_cell_vec.size(), make_pair(i,k));
            m_trans_cell_vec.push_back(trans_cell);

            //设置src_str
            m_trans_cell_vec.back()._trans_src._src_str = sent_vec[k];
            m_trans_cell_vec.back()._trans_src._pre_proc_result = sent_vec[k];

            if(this->GetTgtLanguage() == LANGUAGE_CHINESE )
                CLanguage::RemoveChPosTag(m_trans_cell_vec.back()._trans_src._pre_proc_result);

        }
    }

#ifdef ENABLE_TIME_LOG
    time_log[T_TOTAL][T_PER_PROCESS][T_PER_PROCESS_SPLITE].Stop();
#endif //ENABLE_TIME_LOG

    //Step 3.对每个单独的句子作预处理
#ifdef ENABLE_TIME_LOG
    time_log[T_TOTAL][T_PER_PROCESS][T_PER_PROCESS_SENT].Start();
#endif //ENABLE_TIME_LOG

    for(size_t i=0; i<m_trans_cell_vec.size(); ++i)
    {
#ifdef ENABLE_TIME_LOG
        time_log[T_TOTAL][T_PER_PROCESS][T_PER_PROCESS_SENT][i].Start();
#endif //ENABLE_TIME_LOG

        SentProcCell sent(m_usr_id, m_domain, m_trans_cell_vec[i]._trans_src, m_trans_cell_vec[i]._trans_tgt);
        pre_sent_handler(sent);
        //m_trans_cell_vec[i]._trans_src._pre_proc_result = m_fake;

#ifdef ENABLE_TIME_LOG
        time_log[T_TOTAL][T_PER_PROCESS][T_PER_PROCESS_SENT][i].Stop();
#endif //ENABLE_TIME_LOG

        //lout << "[" << m_trans_cell_vec[i]._trans_src._src_str << "]" << endl;
    }

#ifdef ENABLE_TIME_LOG
    time_log[T_TOTAL][T_PER_PROCESS][T_PER_PROCESS_SENT].Stop();
#endif //ENABLE_TIME_LOG

    return true;
}

bool TransText::PostProcess(SentHandler post_sent_handler)
{
#ifdef ENABLE_TIME_LOG
    time_log[T_TOTAL][T_POST_PROCESS].Start();
#endif //ENABLE_TIME_LOG

    //去掉源端词性标注结果
    this->RemoveSrcPosTag();

    for(size_t i=0; i<m_trans_cell_vec.size(); ++i)
    {
#ifdef ENABLE_TIME_LOG
        time_log[T_TOTAL][T_POST_PROCESS][T_POST_PROCESS_SENT][i].Start();
#endif //ENABLE_TIME_LOG

        SentProcCell sent(m_usr_id, m_domain, m_trans_cell_vec[i]._trans_src, m_trans_cell_vec[i]._trans_tgt);
        post_sent_handler(sent);

        if(this->GetTgtLanguage() == LANGUAGE_ENGLISH)
        {
            //处理英文标点
            const char puncf[4] = {0xef, 0xbc, 0x8c, 0x00};
            const char punct[2] = {0x2c, 0x00};
            find_and_replace(m_trans_cell_vec[i]._trans_tgt._tgt_str, puncf, punct);
        }

#ifdef ENABLE_TIME_LOG
        time_log[T_TOTAL][T_POST_PROCESS][T_POST_PROCESS_SENT][i].Stop();
#endif //ENABLE_TIME_LOG

    }

#ifdef ENABLE_TIME_LOG
    time_log[T_TOTAL][T_POST_PROCESS].Stop();
#endif //ENABLE_TIME_LOG

    return true;
}


bool TransText::CreateAlignment(string & format_str, string & align_str)
{
    for(size_t i=0; i<m_align_vec.size(); ++i)
    {
        TransAlignment & para_align = m_align_vec[i];

        lout << "para_align.src : " << para_align.src << endl;

        string str1, str2, str3;
        BaseAlign::Serialize(para_align.src_a2b_vec, str1);
        BaseAlign::Serialize(para_align.a2b_seg_vec, str2);
        BaseAlign::Serialize(para_align.seg_splite_vec, str3);

        lout << "src_a2b_vec.str1 : " << str1 << endl;
        lout << "a2b_seg_vec.str2 : " << str2 << endl;
        lout << "seg_splite_vec.str3 : " << str3 << endl;

        para_align.sent_vec.resize(para_align.seg_splite_vec.size());

        for(size_t k=0; k<para_align.seg_splite_vec.size(); ++k)
        {
            const BaseAlign & align = para_align.seg_splite_vec[k];

            // 原句在词语序列中对应的起始词和终止词
            size_t beg_word = align.src_offset;
            size_t end_word = beg_word + align.src_size - 1;

            // 转换为原句在字符序列中对应的起始字符和终止字符
            size_t beg_char = para_align.a2b_seg_vec[beg_word].src_offset;
            size_t end_char = para_align.a2b_seg_vec[end_word].src_offset + para_align.a2b_seg_vec[end_word].src_size - 1;

            if(beg_char > end_char || end_char >= para_align.src_a2b_vec.size())
            {
                lout << "Error: beg_char = " << beg_char << " , end_char = " << end_char << endl;
                return false;
            }

            // 转换为原句在原字符序列中对应的offset,找出原句,写入para_align
            lout << "beg_char = " << beg_char << " , end_char = " << end_char << endl;
            lout << "substr.src_offset = " << para_align.src_a2b_vec[beg_char].src_offset << "substr.offset = " << para_align.src_a2b_vec[end_char].src_offset + para_align.src_a2b_vec[end_char].src_size - para_align.src_a2b_vec[beg_char].src_offset << endl;
            lout << "para_align.sent_vec[" << k << "].src : " << para_align.sent_vec[k].src << endl;
            para_align.sent_vec[k].src = para_align.src.substr(para_align.src_a2b_vec[beg_char].src_offset,
                                                               para_align.src_a2b_vec[end_char].src_offset + para_align.src_a2b_vec[end_char].src_size - para_align.src_a2b_vec[beg_char].src_offset);

            // 找出原句中到词的对齐,写入sent_vec中的align_vec
            for(size_t j=beg_word; j<=end_word; ++j)
            {
                BaseAlign word_align;
                const BaseAlign & a2b_align = para_align.a2b_seg_vec[j];

                // 词在原段落中的起止字符
                size_t beg_char_idx = a2b_align.src_offset;
                size_t end_char_idx = a2b_align.src_offset + a2b_align.src_size - 1;

                // 词在原段落中下标对应句子在原段落中下标对应的偏移 和 词本身的长度
                word_align.src_offset = para_align.src_a2b_vec[beg_char_idx].src_offset - para_align.src_a2b_vec[beg_char].src_offset;
                word_align.src_size = para_align.src_a2b_vec[end_char_idx].src_offset + para_align.src_a2b_vec[end_char_idx].src_size - para_align.src_a2b_vec[beg_char_idx].src_offset;
                // TODO
                word_align.tgt_offset = a2b_align.tgt_offset - beg_word;
                word_align.tgt_size = a2b_align.tgt_size;

                para_align.sent_vec[k].align_vec.push_back(word_align);
            }
        }
    }

    // 为每个句子生成最终的对齐
    for(size_t i=0; i<m_trans_cell_vec.size(); ++i)
    {
        TransCell & trans_cell = m_trans_cell_vec[i];
        TransTgt & trans_tgt = m_trans_cell_vec[i]._trans_tgt;
        const vector<BaseAlign> & decode_align_vec = m_trans_cell_vec[i]._trans_tgt._decode_align_vec;

        // 取出当前cell对应的句子对齐,将cell的tgt赋给句子对齐
        TransAlignment::SentAlignment & sent_align = m_align_vec[trans_cell._para_idx.first].sent_vec[trans_cell._para_idx.second];
        sent_align.tgt = m_trans_cell_vec[i]._trans_tgt._tgt_str;

        // 对解码结果的每一个词,找出其在原句中对应词的offset和size,写入句子的final_align_vec
        for(size_t k=0; k<decode_align_vec.size(); ++k)
        {
            BaseAlign align = decode_align_vec[k];

            size_t beg_word_idx = align.src_offset;
            size_t end_word_idx = align.src_offset + align.src_size - 1;

            align.src_offset = sent_align.align_vec[beg_word_idx].src_offset;
            align.src_size = sent_align.align_vec[end_word_idx].src_offset + sent_align.align_vec[end_word_idx].src_size - sent_align.align_vec[beg_word_idx].src_offset;
            sent_align.final_align_vec.push_back(align);
        }

        string str1, str2, str3;
        BaseAlign::Serialize(sent_align.align_vec, str1);
        BaseAlign::Serialize(decode_align_vec, str2);
        BaseAlign::Serialize(sent_align.final_align_vec, str3);

        lout << "src : " << sent_align.src << endl;
        lout << "tgt : " << sent_align.tgt << endl;
        lout << "src    align : " << str1 << endl;
        lout << "decode align : " << str2 << endl;
        lout << "final  align : " << str3 << endl;
    }

    //序列化
    stringstream ss;
    ss << m_align_vec.size() << "\n";
    for(size_t i=0; i< m_align_vec.size(); ++i)
    {
        TransAlignment & trans_align = m_align_vec[i];
        ss << trans_align.sent_vec.size() << "\n";

        for(size_t k=0; k<trans_align.sent_vec.size(); ++k)
        {
            TransAlignment::SentAlignment & sent_align = trans_align.sent_vec[k];

            ss << sent_align.src << "\n";
            ss << sent_align.tgt << "\n";

            string align_str;
            ss << BaseAlign::Serialize(sent_align.final_align_vec, align_str) << "\n";
        }
    }

    align_str = ss.str();

    m_format.Serialization(format_str);

    return false;
}


// protected func
void TransText::remove_pos_tag(string & src)
{
    vector<string> src_vec;
    string tgt;

    split_string_by_blank(src.c_str(), src_vec);

    for(size_t i=0; i<src_vec.size(); ++i)
    {
        size_t pos = src_vec[i].find("/");

        if(string::npos != pos)
        {
            tgt += src_vec[i].substr(0, pos);
        }else
        {
            tgt += src_vec[i];
        }

        if(i+1 < src_vec.size())
            tgt += " ";
    }

    src = tgt;
}

void TransText::remove_pos_tag(const string & src, string & tgt)
{
    vector<string> src_vec;
    tgt.clear();

    split_string_by_blank(src.c_str(), src_vec);

    for(size_t i=0; i<src_vec.size(); ++i)
    {
        size_t pos = src_vec[i].find("/");

        if(string::npos != pos)
        {
            tgt += src_vec[i].substr(0, pos);
        }else
        {
            tgt += src_vec[i];
        }

        if(i+1 < src_vec.size())
            tgt += " ";
    }
}


// private func
SentUniqID TransText::generate_sent_id()
{
    MutexLockGuard guard(ms_sid_mutex);
    return ++ms_base_sent_id;
}