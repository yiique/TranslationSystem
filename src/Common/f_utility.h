//
// Created by ������ on 16/7/20.
//

#ifndef PROJECT_F_UTILITY_H
#define PROJECT_F_UTILITY_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

//�ַ���ת����
bool cstr_2_num(const char *cstr, int &num);
int str_2_num(const string & str);

//����ת�ַ���
template <typename num_t>
string num_2_str(const num_t num)
{
    string s_num;
    stringstream ss;
    ss << num;
    ss >> s_num;

    return s_num;
}

//��������д���
size_t count_words(const string & src);

//ȥ���ַ����еĿո�
void del_str_blank(string &str);
//ȥ���ַ�����β�ո�,���ı�ԭ�ַ���
string & del_head_tail_blank(string & str);
//ȥ���ַ�����β���з��Ϳո�,���ı�ԭ�ַ���
string & filter_head_tail(string & str);

//���ո��з־�����vector,���������ո�
bool split_string_by_blank(const char * src, vector<string> & tgt_vec);
//��tag�з־�����vector,��������tag
bool split_string_by_tag(const char * src, vector<string> & tgt_vec, const char tag);
//��tag�з��ַ�������vector,����������tag,�ϸ��з�
bool split_sequence_by_tag(const char * src, vector<string> & tgt_vec, const char tag);
//��vector�ϲ�Ϊstring,�ÿո�ָ�
bool merge_strvec_to_str(const vector<string> & str_vec, string & tgt);

//��ȡ�ַ���
bool str_tag_find(const string & src, const string & beg_tag, const string & end_tag, string & content);
//����&�滻 �����滻����
size_t find_and_replace(string & src, const string & from_s, const string & to_s);

//����һ��UUID
string GenerateUUID();

#endif //PROJECT_F_UTILITY_H
