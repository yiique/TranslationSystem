//
// Created by 刘舒曼 on 16/9/11.
//

#ifndef TRANSMASTER_TEMPLATEDEF_H
#define TRANSMASTER_TEMPLATEDEF_H

#include "Common/BasicType.h"

#include <string>

using namespace std;

#define SYSMANAGER "superadmin"
typedef int TemplateLibID;
typedef int TemplateID;

class TemplateInfo
{
public:
    TemplateInfo(const int _is_checked = 0, const int _is_deleted = 0, const int _is_active = 1) : is_checked(_is_checked),
                                                                                                   is_active(_is_active),
                                                                                                   is_deleted(_is_deleted)
    {
    }

    TemplateID template_id;

    string src;
    string tgt;

    int _is_checked;
    int _is_active;     // 1表示已启用
    int _is_deleted;    // 1表示已删除
};

class TemplateLibInfo
{
public:
    TemplateLibInfo(const int _is_deleted = 0, const int _is_active = 1, const int _is_system = 0) : is_deleted(_is_deleted),
                                                                                                     is_active(_is_active),
                                                                                                     is_system(_is_system)
    {
    }

    UsrID usr_id;
    TemplateLibID templatelib_id;
    string templatelib_name;
    DomainType domain_type;

    string description;

    int is_system;
    int is_active;  // 1表示启用
    int is_deleted; // 1表示删除

    bool operator == ( const TemplateLibInfo & rsh ) const
    {
        if(templatelib_id == rsh.templatelib_id
           && domain_info == rsh.domain_info
           && templatelib_name == rsh.templatelib_name
           && usr_id == rsh.usr_id
           && is_deleted == rsh.is_deleted
           && is_active == rsh.is_active
           && description == rsh.description)
            return true;

        return false;
    }
};

#endif //TRANSMASTER_TEMPLATEDEF_H
