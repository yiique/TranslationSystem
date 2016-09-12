//
// Created by ������ on 16/9/12.
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
        vector<int> start;          // �����ַ���ʼλ��
        vector<int> end;            // �����ַ�����λ��

        vector<int> start_token;    // token��ʼλ��
        vector<int> end_token;      // token����λ��

        Pattern pattern;            // ƥ�����õ�pattern

    }myMatchResult;                 // ƥ�����ṹ��

    // ICTtemplate ģ����
    class ICTtemplate
    {
    public:
        Pattern pattern;            // ��������ʽ

        ICTtemplate();
        ~ICTtemplate();

        // _patternΪһ��ceģ��,��'|||'�ָopΪ���Ʋ���1-e2c,0-c2e
        bool Reset(string & _pattern, string op);
    };







    // ��string�е������Ӵ��滻Ϊ��һ���Ӵ�
    bool changePunction(string& _pattern, string from, string to);

    // ȥ��String�����еĿո�
    bool delspace(string& _sourceStr);

    // �����ַ�λ��vector����token���vector��д��myMatchResult
    bool ReCalculateEn(const string _sourceStr, std::vector< std::pair<int, int> > result, myMatchResult& _mr); //����Ӣ��token���
    bool ReCalculateCh(const string _sourceStr, std::vector< std::pair<int, int> > result, myMatchResult& _mr); //��������token���

    bool match(Pattern _pattern, const string _sourceStr, myMatchResult& _mr, string _flag); //sourceStr��Ҫ�����ľ��ӣ�index�ܹ�ƥ��ı�ţ�flag����ʹ��E����C

    bool match_all(ICTtemplate _icttemplate, const string _sourceStr, NeedResult& _nr, string _flag);

    bool FormatOut(NeedResult & _nr, myMatchResult _mr,const string _sourceStr, string _op);

    bool changeVariableFlag(string& _sourceStr, vector<std::pair<int, int> > _matchgroup, vector<std::pair<int ,int> > _matchgrouptoken, string _op);

}

#endif //TRANSMASTER_LENOICTTEMPLATE_H
