//
// Created by 刘舒曼 on 16/7/28.
//

#ifndef TRANSMASTER_TEXTTOOLS_H
#define TRANSMASTER_TEXTTOOLS_H

#include <utility>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

//判断是否是英文文本
//返回值： TRUE 英文， FALSE 其他（中文）
bool IsEnglishText(const string & src);

//根据position解析结果还原文本
bool RevertTextWithPos(const string & src,
                       const vector<string> & tgt_vec,
                       const vector<pair<size_t,size_t> > & pos_vec,
                       string & tgt);

#endif //TRANSMASTER_TEXTTOOLS_H
