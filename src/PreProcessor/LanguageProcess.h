//
// Created by 刘舒曼 on 16/9/8.
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
    // 对原文本进行utf8切分预处理,无需token,原文本保持不变
    static bool StandardPreTextProcess(string & src);
    // 断句
    static bool StandardSplitSent(const string & src, vector<string> & sent_vec);

    static bool StandardPreSentProcess(SentProcCell & sent_cell);
};

#endif //TRANSMASTER_LANGUAGEPROCESS_H
