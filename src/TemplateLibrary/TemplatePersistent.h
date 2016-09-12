//
// Created by ¡ı Ê¬¸ on 16/9/11.
//

#ifndef TRANSMASTER_TEMPLATEPERSISTENT_H
#define TRANSMASTER_TEMPLATEPERSISTENT_H

#include "DataBase/DBOperation.h"
#include "TemplateDef.h"

class TemplatePersistent
{
public:
    static int CreateTemplateLib(TemplateLibInfo & templatelib_info);
    static int DeleteTemplateLib(const TemplateLibInfo & templatelib_info);
    static int ModifyTemplateLib(const TemplateLibInfo & templatelib_info);

    static int InsertTemplate(const TemplateLibID templatelib_id, TemplateInfo & template_info);
    static int InsertTemplateGroup(const TemplateLibID templatelib_id, const vector<bool> & filter_vec, vector<TemplateInfo> & template_info_vec);
    static int DeleteTemplate(const TemplateID template_id);
    static int ModifyTemplate(const TemplateInfo & template_info);

    static int LoadAllTemplateLib(vector<TemplateLibInfo> & templatelib_info_vec);
    static int LoadTemplate(const TemplateLibInfo & templatelib_info, vector<TemplateInfo> & template_info_vec);

    static int GetRecoverTemplateInfo(const TemplateID & template_id, TemplateInfo & template_info);
    static int RecoverTemplate(const TemplateID & template_id);

    static int GetMaxTemplateID(TemplateLibID & templatelib_id, TemplateID & template_id);
};

#endif //TRANSMASTER_TEMPLATEPERSISTENT_H
