//
// Created by ������ on 16/9/8.
//

#include "LanguageProcess.h"

bool LanguageProcess::StandardPreTextProcess(string &src)
{
    CLanguage::Utf8A2B(src);

    return true;
}

bool LanguageProcess::StandardSplitSent(const string &src, vector<string> &sent_vec)
{
    ssplite::EnSentSpliter spliter(src, false);
    sent_vec = spliter.GetSpliteResult();
    return true;
}

bool LanguageProcess::StandardPreSentProcess(SentProcCell &sent_cell)
{
    // ���˾����еĶ���ո�
    CLanguage::StandardSentFilter(sent_cell);

    // ƥ��ʵ�
    list<dictionary::MatchResult> match_result_list;
    dictionary::DictManager::GetInstance()->MatchSent(sent_cell.usr_id, sent_cell.domain,
                                                      sent_cell.trans_src._src_str, match_result_list);

    for (list<dictionary::MatchResult>::iterator iter = match_result_list.begin(); iter != match_result_list.end(); ++iter)
    {
        sent_cell.trans_src._dict_result_vec.push_back(iter->serialize());
    }

    // ƥ��ģ��,Ӧ�ÿ������ɿսӿ�,�Ա��������ģ��
    UsrTemplateLibManager::GetInstance()->MatchSent(sent_cell.usr_id, sent_cell.domain,
                                                    sent_cell.trans_src._src_str, sent_cell.trans_src._dict_result_vec);
    return true;
}