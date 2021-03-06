//
// Created by 刘舒曼 on 16/7/28.
//

#ifndef TRANSMASTER_TEXTFORMAT_H
#define TRANSMASTER_TEXTFORMAT_H

#include "Log/LogStream.h"

#include <vector>
#include <utility>
#include <string>
#include <algorithm>
using namespace std;

//段落的定义
typedef struct
{
    string _content; //有效内容（即待翻译的内容）
    size_t _offset;  //在原文中的位置
    size_t _len;     //在原文中的长度
} Para;

//文本格式描述结构
class TextFormat
{
public:
    TextFormat(void) {};
    ~TextFormat(void) { this->Clear(); };
    void Clear();

    size_t Size() const { return m_para_vec.size(); };

    void Reset(const vector<string> & para_vec,
               const vector<pair<size_t, size_t> > & pos_vec);

    void Serialization(string & data) const ;
    bool UnSerialization(const string & data);

    Para & operator [] (size_t idx)
    {
        assert( idx < m_para_vec.size() );
        return *(m_para_vec[idx]);
    }

private:
    vector<Para*> m_para_vec;

    class ParaInAhead
    {
    public:
        bool operator () (const Para * pa, const Para * pb) const
        {
            assert(pa && pb);
            return pa->_offset < pb->_offset;
        }
    };

    bool check_format_info(vector<Para*> & para_vec);
};

#endif //TRANSMASTER_TEXTFORMAT_H
