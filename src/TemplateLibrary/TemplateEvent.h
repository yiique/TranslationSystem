//
// Created by 刘舒曼 on 16/9/11.
//

#ifndef TRANSMASTER_TEMPLATEEVENT_H
#define TRANSMASTER_TEMPLATEEVENT_H

#include "EventEngine/EventData.h"
#include "TemplateDef.h"

#include <set>
#include <map>
using namespace std;

// TemplateLibCreateReq     包含一个TemplateLibInfo             Res     包含一个int类结果          创建模版库
// TemplateLibDeleteReq     包含一个Templatelib_id set          Res     包含一个failure_map结果    删除模版
// TemplateLibModifyReq     包含一个TemplatelibInfo             Res     包含一个int类结果
// TemplateLibRecoverReq    包含一个Templatelib_id              Res     包含一个int类结果          恢复被删除的模版库
// ToSysTemplateLibReq      包含一个vector的template_id和一个templateLib id
//                                                             Res     包含一个int类结果
//
// TemplateInsertReq        包含templateInfo和templatelib_id    Res     包含一个int类结果
// TemplateDeleteReq        包含template_id和templatelib_id     Res     包含一个int类结果
// TemplateModifyReq        包含templateInfo和templatelib_id    Res     包含一个int类结果
// TemplateRecoverReq       包含template_id和templatelib_id     Res     包含一个int类结果
// TemplateLoadReq          包含lib_id, file_name, is_active和is_utf8
//                                                             Res     包含一个int类结果

// TemplateLibCreateReq Res
class TemplateLibCreateReq : public EventData
{
public:
    TemplateLibCreateReq(const CallID & cid) : EventData(cid) {}
    ~TemplateLibCreateReq(void) {}

    const TemplateLibInfo & GetTemplateLibInfo() { return m_templatelibinfo; }
    void SetTemplateLibInfo(const TemplateLibInfo & info) { m_templatelibinfo=info; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_CREATE_REQ; }
private:
    TemplateLibInfo m_templatelibinfo;
};

class TemplateLibCreateRes : public EventData
{
public:
    TemplateLibCreateRes(const CallID & cid): EventData(cid) {}
    ~TemplateLibCreateRes() {}

    int GetRes() const { return m_res; }
    void SetRes(int res) { m_res=res; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_CREATE_RES; }
private:
    int m_res;
};


// TemplateLibDeleteReq Res
class TemplateLibDeleteReq : public EventData
{
public:
    TemplateLibDeleteReq(const CallID & cid) : EventData(cid) {}
    ~TemplateLibDeleteReq(void) {}

    const set<TemplateLibID> & GetTemplateLibIDList() const { return m_templatelib_id_set; }
    void AddTemplateLibID(const TemplateLibID templatelib_id) { m_templatelib_id_set.insert(templatelib_id); }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_DELETE_REQ; }
private:
    set<TemplateLibID> m_templatelib_id_set;
};

class TemplateLibDeleteRes : public EventData
{
public:
    TemplateLibDeleteRes(const CallID & cid) : EventData(cid) {}
    ~TemplateLibDeleteRes() {}

    void AddFailure(const TemplateLibID templatelib_id, const int result) { m_failure_map.insert(make_pair(templatelib_id, result)); }
    const map<TemplateLibID, int> & GetFailureList() const { return m_failure_map; }
    const EDType GetType() const {return EDTYPE_TEMPLATELIB_DELETE_RES; }
private:
    map<TemplateLibID, int> m_failure_map;
};


// TemplateLibModifyReq Res
class TemplateLibModifyReq : public EventData
{
public:
    TemplateLibModifyReq(const CallID & cid) : EventData(cid) {}
    ~TemplateLibModifyReq(void) {}

    TemplateLibInfo & GetTemplateLibInfo() { return m_templatelib_info; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_MODIFY_REQ; }
private:
    TemplateLibInfo m_templatelib_info;
};

class TemplateLibModifyRes : public EventData
{
public:
    TemplateLibModifyRes(const CallID & cid): EventData(cid) {}
    ~TemplateLibModifyRes() {}

    int GetRes() const { return m_res; }
    void SetRes(int res) { m_res=res; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_MODIFY_RES; }
private:
    int m_res;
};


// TemplateLibRecoverReq Res
class TemplateLibRecoverReq : public EventData
{
public:
    TemplateLibRecoverReq(const CallID & cid) : EventData(cid) {}
    ~TemplateLibRecoverReq(void) {}

    const TemplateLibID & GetTemplateLibID() { return m_templatelibid; }
    void SetTemplateLibID(const TemplateLibID & templatelibid) { m_templatelibid=templatelibid; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_RECOVER_REQ; }
private:
    TemplateLibID m_templatelibid;
};

class TemplateLibRecoverRes : public EventData
{
public:
    TemplateLibRecoverRes(const CallID & cid): EventData(cid) {}
    ~TemplateLibRecoverRes() {}

    int GetRes() const { return m_res; }
    void SetRes(int res) { m_res=res; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_RECOVER_RES; }
private:
    int m_res;
};


// ToSysTemplateLibReq Res
class ToSysTemplateLibReq
        : public EventData
{
public:
    ToSysTemplateLibReq(const CallID & cid) : EventData(cid) {}
    ~ToSysTemplateLibReq(void) {}

    vector<int> & GetSrcTemplateID()
    {
        return m_template_id;
    }
    const TemplateLibID & GetTgtTemplateLibID() const { return m_tgt_templatelib_id; }
    void SetSrcTemplateID(vector<int> & src_template_id)
    {
        for(int i=0; i<src_template_id.size(); i++)
        {
            m_template_id.push_back(src_template_id[i]);
        }
    }
    void SetTgtTemplateLibID(TemplateLibID tgt_templatelib_id) {m_tgt_templatelib_id=tgt_templatelib_id;}
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_TOSYSTEM_REQ; }
private:
    vector<int> m_template_id;
    TemplateLibID m_tgt_templatelib_id;
};

class ToSysTemplateLibRes
        :public EventData
{
public:
    ToSysTemplateLibRes(const CallID & cid) : EventData(cid) {}
    ~ToSysTemplateLibRes(void) {}

    int GetRes() const { return m_res; }
    void SetRes(int res) { m_res=res; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_TOSYSTEM_RES; }
private:
    int m_res;
};


// TemplateInsertReq Res
class TemplateInsertReq : public EventData
{
public:
    TemplateInsertReq(const CallID & cid) : EventData(cid) {}
    ~TemplateInsertReq(void) {}

    const TemplateLibID & GetTemplateLibID() const { return m_templatelib_id; }
    TemplateInfo & GetTemplateInfo() { return m_template_info; }
    void SetTemplateLibID(const TemplateLibID templatelib_id) { m_templatelib_id = templatelib_id; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_TEMPLATES_INSERT_REQ; }
private:
    TemplateInfo m_template_info;
    TemplateLibID m_templatelib_id;
};

class TemplateInsertRes : public EventData
{
public:
    TemplateInsertRes(const CallID & cid): EventData(cid) {}
    ~TemplateInsertRes() {}

    int GetRes() const { return m_res; }
    void SetRes(int res) { m_res=res; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_TEMPLATES_INSERT_RES; }
private:
    int m_res;
};


// TemplateDeleteReq Res
class TemplateDeleteReq : public EventData
{
public:
    TemplateDeleteReq(const CallID & cid) : EventData(cid) {}
    ~TemplateDeleteReq(void) {}

    const TemplateLibID & GetTemplateLibID() const { return m_templatelib_id; }
    const TemplateID & GetTemplateID() const { return m_template_id; }
    void SetTemplateLibID(const TemplateLibID templatelib_id) { m_templatelib_id = templatelib_id; }
    void SetTemplateID(const TemplateID template_id) { m_template_id = template_id; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_TEMPLATES_DELETE_REQ; }
private:
    TemplateLibID m_templatelib_id;
    TemplateID m_template_id;
};

class TemplateDeleteRes : public EventData
{
public:
    TemplateDeleteRes(const CallID & cid): EventData(cid) {}
    ~TemplateDeleteRes() {}

    int GetRes() const { return m_res; }
    void SetRes(int res) { m_res=res; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_TEMPLATES_DELETE_RES; }
private:
    int m_res;
};


// TemplateModifyReq Res
class TemplateModifyReq : public EventData
{
public:
    TemplateModifyReq(const CallID & cid) : EventData(cid) {}
    ~TemplateModifyReq(void) {}

    const TemplateLibID & GetTemplateLibID() const { return m_templatelib_id; }
    TemplateInfo & GetTemplateInfo() { return m_template_info; }
    void SetTemplateLibID(const TemplateLibID templatelib_id) { m_templatelib_id = templatelib_id; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_TEMPLATES_MODIFY_REQ; }
private:
    TemplateInfo m_template_info;
    TemplateLibID m_templatelib_id;
};

class TemplateModifyRes : public EventData
{
public:
    TemplateModifyRes(const CallID & cid): EventData(cid) {}
    ~TemplateModifyRes() {}

    int GetRes() const { return m_res; }
    void SetRes(int res) { m_res=res; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_TEMPLATES_MODIFY_RES; }
private:
    int m_res;
};


// TemplateRecoverReq Res
class TemplateRecoverReq : public EventData
{
public:
    TemplateRecoverReq(const CallID & cid) : EventData(cid) {}
    ~TemplateRecoverReq(void) {}

    const TemplateLibID & GetTemplateLibID() const { return m_templatelib_id; }
    const TemplateID & GetTemplateID() const { return m_template_id; }
    void SetTemplateLibID(const TemplateLibID templatelib_id) { m_templatelib_id = templatelib_id; }
    void SetTemplateID(const TemplateID template_id) { m_template_id = template_id; }
    const EDType GetType() const { return EDTYPE_TEMPLATE_RECOVER_REQ; }
private:
    TemplateLibID m_templatelib_id;
    TemplateID m_template_id;
};

class TemplateRecoverRes : public EventData
{
public:
    TemplateRecoverRes(const CallID & cid): EventData(cid) {}
    ~TemplateRecoverRes() {}

    int GetRes() const { return m_res; }
    void SetRes(int res) { m_res=res; }
    const EDType GetType() const { return EDTYPE_TEMPLATE_RECOVER_RES; }
private:
    int m_res;
};


// TemplateLoadReq Res
class TemplateLoadReq : public EventData
{
public:
    TemplateLoadReq(const CallID & cid) : EventData(cid) {}
    ~TemplateLoadReq(void) {}

    const TemplateLibID & GetTemplateLibID() const { return m_templatelib_id; }
    const string & GetTemplateFileName() const { return m_template_file_name; }
    int GetIsActive() const { return m_is_active; }
    bool IsUTF8() const { return m_is_utf8; }
    void SetActive(const int is_active) { m_is_active = is_active; }
    void SetTemplateLibID(const TemplateLibID templatelib_id) { m_templatelib_id = templatelib_id; }
    void SetTemplateFileName(const string & template_file_name) { m_template_file_name = template_file_name; }
    void SetIsUTF8(bool is)
    {
        m_is_utf8 = is;
    }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_TEMPLATES_LOAD_REQ; }
private:
    TemplateLibID m_templatelib_id;
    string m_template_file_name;
    int m_is_active;
    bool m_is_utf8;
};

class TemplateLoadRes : public EventData
{
public:
    TemplateLoadRes(const CallID & cid): EventData(cid) {}
    ~TemplateLoadRes() {}

    int GetRes() const { return m_res; }
    void SetRes(int res) { m_res=res; }
    const EDType GetType() const { return EDTYPE_TEMPLATELIB_TEMPLATES_LOAD_RES; }
private:
    int m_res;
};

#endif //TRANSMASTER_TEMPLATEEVENT_H
