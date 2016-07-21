//
// Created by 刘舒曼 on 16/7/20.
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

//字符串转数字
bool cstr_2_num(const char *cstr, int &num);
int str_2_num(const string & str);

//数字转字符串
template <typename num_t>
string num_2_str(const num_t num)
{
    string s_num;
    stringstream ss;
    ss << num;
    ss >> s_num;

    return s_num;
}

//计算句子中词数
size_t count_words(const string & src);

//去掉字符串中的空格
void del_str_blank(string &str);
//去掉字符串首尾空格,不改变原字符串
string & del_head_tail_blank(string & str);
//去掉字符串首尾换行符和空格,不改变原字符串
string & filter_head_tail(string & str);

//按空格切分句子至vector,忽略连续空格
bool split_string_by_blank(const char * src, vector<string> & tgt_vec);
//按tag切分句子至vector,忽略连续tag
bool split_string_by_tag(const char * src, vector<string> & tgt_vec, const char tag);
//按tag切分字符序列至vector,不忽略连续tag,严格切分
bool split_sequence_by_tag(const char * src, vector<string> & tgt_vec, const char tag);
//将vector合并为string,用空格分割
bool merge_strvec_to_str(const vector<string> & str_vec, string & tgt);

//截取字符串
bool str_tag_find(const string & src, const string & beg_tag, const string & end_tag, string & content);
//查找&替换 返回替换次数
size_t find_and_replace(string & src, const string & from_s, const string & to_s);

//产生一个UUID
string GenerateUUID();

#endif //PROJECT_F_UTILITY_H
