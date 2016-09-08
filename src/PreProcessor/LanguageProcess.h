//
// Created by ������ on 16/9/8.
//

#ifndef TRANSMASTER_LANGUAGEPROCESS_H
#define TRANSMASTER_LANGUAGEPROCESS_H

#include "DictLibrary/DictManager.h"
#include "TemplateLibrary/UsrTemplateManager.h"
#include "LanguageTools/CLanguage.h"
#include "LanguageTools/SentSpliter.h"
#include "TransText/TransText.h"
#include "SipRPC/SipRPC.h"

#include <string>
#include <vector>

using namespace std;

class LanguageProcess
{
public:
    // ��ԭ�ı�����utf8�з�Ԥ����,����token,ԭ�ı����ֲ���
    static bool StandardPreTextProcess(string & src);
    // �Ͼ�
    static bool StandardSplitSent(const string & src, vector<string> & sent_vec);

    static bool StandardPreSentProcess(SentProcCell & sent_cell);
};

#endif //TRANSMASTER_LANGUAGEPROCESS_H
