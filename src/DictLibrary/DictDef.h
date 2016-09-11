//
// Created by 刘舒曼 on 16/9/11.
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

    int is_active;      // 1表示启用
    int is_checked;     // 1表示已审核
    int is_deleted;     // 1表示已删除

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
    DictType type;          // 词典类型 0 用户词典; 1 系统词典; 2 专业词典;     对应is_system字段
    string description;

    int is_active;          // 1表示启用
    int is_deleted;         // 1表示已删除

    DictInfo(const int _is_deleted = 0, const int _is_active = 1, const int _type = 0) : is_deleted(_is_deleted),
                                                                                         is_active(_is_active),
                                                                                         type(_type)
    {
    }
};

#endif //TRANSMASTER_DICTDEF_H
