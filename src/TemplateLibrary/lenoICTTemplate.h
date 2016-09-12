//
// Created by 刘舒曼 on 16/9/12.
//

#ifndef TRANSMASTER_LENOICTTEMPLATE_H
#define TRANSMASTER_LENOICTTEMPLATE_H

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <strstream>
#include <sstream>
#include <boost/regex.hpp>
#include <sys/types.h>
using namespace std;

namespace leonICTTemplate
{
    // NeedResult
    typedef struct
    {
        string formatResult;
        string sourceStr;

        bool flag;

        vector<std::pair<int, int>> matchgroup;

    }NeedResult;

    // Pattern
    typedef struct
    {
        string e_pattern;
        string c_pattern;

        bool flag;
        int pri;
    }Pattern;

    // myMatchResult
    typedef struct
    {
        vector<int> start;          // 分组字符开始位置
        vector<int> end;            // 分组字符结束位置

        vector<int> start_token;    // token开始位置
        vector<int> end_token;      // token结束位置

        Pattern pattern;            // 匹配所用的pattern

    }myMatchResult;                 // 匹配结果结构体

    // ICTtemplate 模版类
    class ICTtemplate
    {
    public:
        Pattern pattern;            // 保存正则式

        ICTtemplate();
        ~ICTtemplate();

        // _pattern为一对ce模板,用'|||'分割，op为控制参数1-e2c,0-c2e
        bool Reset(string & _pattern, string op);
    };







    // 将string中的所有子串替换为另一个子串
    bool changePunction(string& _pattern, string from, string to);

    // 去除String中所有的空格
    bool delspace(string& _sourceStr);

    // 根据字符位置vector计算token序号vector并写入myMatchResult
    bool ReCalculateEn(const string _sourceStr, std::vector< std::pair<int, int> > result, myMatchResult& _mr); //处理英文token标记
    bool ReCalculateCh(const string _sourceStr, std::vector< std::pair<int, int> > result, myMatchResult& _mr); //处理中文token标记

    bool match(Pattern _pattern, const string _sourceStr, myMatchResult& _mr, string _flag); //sourceStr需要分析的句子，index能够匹配的标号，flag控制使用E或者C

    bool match_all(ICTtemplate _icttemplate, const string _sourceStr, NeedResult& _nr, string _flag);

    bool FormatOut(NeedResult & _nr, myMatchResult _mr,const string _sourceStr, string _op);

    bool changeVariableFlag(string& _sourceStr, vector<std::pair<int, int> > _matchgroup, vector<std::pair<int ,int> > _matchgrouptoken, string _op);

}

#endif //TRANSMASTER_LENOICTTEMPLATE_H
