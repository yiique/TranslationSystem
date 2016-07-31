//
// Created by ������ on 16/7/28.
//

#ifndef TRANSMASTER_TEXTFORMAT_H
#define TRANSMASTER_TEXTFORMAT_H

#include "Log/LogStream.h"

#include <vector>
#include <utility>
#include <string>
#include <algorithm>
using namespace std;

//����Ķ���
typedef struct
{
    string _content; //��Ч���ݣ�������������ݣ�
    size_t _offset;  //��ԭ���е�λ��
    size_t _len;     //��ԭ���еĳ���
} Para;

//�ı���ʽ�����ṹ
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
