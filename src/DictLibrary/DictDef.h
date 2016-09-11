//
// Created by ������ on 16/9/11.
//

#ifndef TRANSMASTER_DICTDEF_H
#define TRANSMASTER_DICTDEF_H

#include "Common/BasicType.h"

#include <string>
#include <utility>
using namespace std;

#define SYSMANAGER "superadmin"

typedef size_t WordID;
typedef size_t DictID;
typedef int DictType;

#define DICT_TYPE_USR 0
#define DICT_TYPE_SYS 1
#define DICT_TYPE_PRO 2


class WordInfo
{
public:
    WordID word_id;
    DictID dict_id;

    string src;
    string tgt;
    string src_key;

    int is_active;      // 1��ʾ����
    int is_checked;     // 1��ʾ�����
    int is_deleted;     // 1��ʾ��ɾ��

    WordInfo(const int _is_deleted = 0, const int _is_active = 1, const int _is_checked = 1) : is_deleted(_is_deleted),
                                                                                               is_active(_is_active),
                                                                                               is_checked(_is_checked)
    {
    }
};

class DictInfo
{
public:
    DictID dict_id;
    UsrID usr_id;

    string dict_name;
    DomainType domain_info;
    DictType type;          // �ʵ����� 0 �û��ʵ�; 1 ϵͳ�ʵ�; 2 רҵ�ʵ�;     ��Ӧis_system�ֶ�
    string description;

    int is_active;          // 1��ʾ����
    int is_deleted;         // 1��ʾ��ɾ��

    DictInfo(const int _is_deleted = 0, const int _is_active = 1, const int _type = 0) : is_deleted(_is_deleted),
                                                                                         is_active(_is_active),
                                                                                         type(_type)
    {
    }
};

#endif //TRANSMASTER_DICTDEF_H
