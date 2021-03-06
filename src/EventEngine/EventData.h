//
// Created by 刘舒曼 on 16/7/20.
// 事件数据类
//

#ifndef PROJECT_EVENTDATA_H
#define PROJECT_EVENTDATA_H

#include <iostream>
using namespace std;

// 事件调用ID
typedef unsigned long CallID;
#define NULL_CALL_ID 0

// EventData的类型
typedef enum
{
    //NetRequest
    EDTYPE_LISTEN_NET_REQ, //0
    EDTYPE_CONN_NET_REQ,   //1
    EDTYPE_CLOSE_NET_REQ,  //2
    EDTYPE_RECV_NET_REQ,   //3
    EDTYPE_SEND_NET_REQ,   //4
    //NetResponse
    EDTYPE_LISTEN_NET_RES, //5
    EDTYPE_CONN_NET_RES,   //6
    EDTYPE_CLOSE_NET_RES,  //7
    EDTYPE_RECV_NET_RES,   //8
    EDTYPE_SEND_NET_RES,   //9
    EDTYPE_UNDEF_NET_RES,   //10

    //心跳包
    EDTYPE_PULSE_REQ, //11
    EDTYPE_PULSE_RES, //12
    //翻译通知
            EDTYPE_TRANS_STATE, //13
    EDTYPE_TRANS_TASK,   //14
    EDTYPE_TRANS_RESULT,  //15
    //领域管理
            EDTYPE_ADD_DOMAIN_REQ, //16
    EDTYPE_ADD_DOMAIN_RES,  //17

    EDTYPE_USR_CMD, //用户命令基类18
    EDTYPE_USR_CMD_ADD,//添加任务19
            EDTYPE_USR_CMD_DEL,//删除任务20
            EDTYPE_USR_CMD_REORDER,//任务调序21
            EDTYPE_USR_CMD_QUERY,//任务查询22
            EDTYPE_USR_CMD_ADDUSR,//添加用户23
            EDTYPE_USR_CMD_DELUSR,//删除用户24
            EDTYPE_USR_CMD_FINDFILE,//单个文件查询25
            EDTYPE_USR_CMD_STARTDOMAIN,//启动领域26
            EDTYPE_USR_CMD_RESTARTDOMAIN,//重启领域27
            EDTYPE_USR_CMD_SUSPENDDOMAIN,//暂停领域28
            EDTYPE_CMD_RES, //命令返回结果基类29
    EDTYPE_CMD_RES_QUERY,//查询返回结果30
            EDTYPE_CMD_RES_DEL,//查询返回结果31

    //预处理
            EDTYPE_PREPROC_REQ, //32
    EDTYPE_PREPROC_RES,  //33

    //DB
            EDTYPE_DB_GET_DATA_REQ,  //34
    EDTYPE_DB_GET_DATA_RES,  //35
    EDTYPE_DB_COMMIT_RESULT_REQ,//36
            EDTYPE_DB_REPORT_REQ,//37

    //Dict
            EDTYPE_DICT_CREATE_REQ,//38
            EDTYPE_DICT_CREATE_RES,//39
            EDTYPE_DICT_DELETE_REQ,//40
            EDTYPE_DICT_DELETE_RES,//41
    //
            EDTYPE_DICT_TOSYSTEM_REQ,
    EDTYPE_DICT_TOSYSTEM_RES,
    EDTYPE_DICT_WORDS_RECOVER_REQ,
    EDTYPE_DICT_WORDS_RECOVER_RES,
    EDTYPE_DICT_RECOVER_REQ,
    EDTYPE_DICT_RECOVER_RES,
    //
            EDTYPE_DICT_WORDS_INSERT_REQ,//42
            EDTYPE_DICT_WORDS_INSERT_RES,//43
            EDTYPE_DICT_WORDS_DELETE_REQ,//44
            EDTYPE_DICT_WORDS_DELETE_RES,//45
            EDTYPE_DICT_WORDS_MODIFY_REQ,//46
            EDTYPE_DICT_WORDS_MODIFY_RES,//47

    //文件翻译提交
            EDTYPE_OLT_SUBMIT_REQ, //48
    EDTYPE_OLT_SUBMIT_RES, //49

    //日志
            EDTYPE_LOG_DATA, //50

    //Timer
            EDTYPE_TIMER,  //51


    EDTYPE_DB_DELETE_REQ, //52

    //Template
            EDTYPE_TEMPLATE_CREATE_REQ,//53
            EDTYPE_TEMPLATE_CREATE_RES,//54
            EDTYPE_TEMPLATE_DELETE_REQ,//55
            EDTYPE_TEMPLATE_DELETE_RES,//56
            EDTYPE_TEMPLATE_INSERT_REQ,//57
            EDTYPE_TEMPLATE_INSERT_RES,//58
            EDTYPE_TEMPLATE_DEL_ENTRY_REQ,//59
            EDTYPE_TEMPLATE_DEL_ENTRY_RES,//60

    //远程文本处理
            EDTYPE_TEXT_PROCESS_REQ,//61
            EDTYPE_TEXT_PROCESS_RES,//62

    //终止翻译
            EDTYPE_TRANS_STOP_REQ, //63
    EDTYPE_TRANS_STOP_RES, //64

    //获取DB中临时文件表的数据
            EDTYPE_DB_GET_TEMP_DATA_REQ, //65
    EDTYPE_DB_GET_TEMP_DATA_RES, //66

    //批量导入词典
            EDTYPE_DICT_IMPORT_REQ, //67
    EDTYPE_DICT_IMPORT_RES, //68

    //更新数据库中Text的信息
            EDTYPE_DB_UPDATE_TEXT_REQ, //69
    EDTYPE_DB_UPDATE_TEXT_RES, //70
    EDTYPE_DB_COMMIT_RESULT_RES,//71


    EDTYPE_USR_CMD_QUEUEINFO, //72 查询用户队列信息
    EDTYPE_USR_CMD_QUEUEINFO_RES, //73

    //TEXT HANDLER EVENT
            EDTYPE_LOAD_TEXT_REQ,
    EDTYPE_LOAD_TEXT_RES,
    EDTYPE_CREATE_RESULT_REQ,
    EDTYPE_CREATE_RESULT_RES,

    EDTYPE_DICT_WORDS_LOAD_REQ,
    EDTYPE_DICT_WORDS_LOAD_RES,

    EDTYPE_DICT_MODIFY_REQ,
    EDTYPE_DICT_MODIFY_RES,

    //模板管理
            EDTYPE_TEMPLATELIB_CREATE_REQ,
    EDTYPE_TEMPLATELIB_CREATE_RES,
    EDTYPE_TEMPLATELIB_DELETE_REQ,
    EDTYPE_TEMPLATELIB_DELETE_RES,
    EDTYPE_TEMPLATELIB_TEMPLATES_INSERT_REQ,
    EDTYPE_TEMPLATELIB_TEMPLATES_INSERT_RES,
    EDTYPE_TEMPLATELIB_TEMPLATES_DELETE_REQ,
    EDTYPE_TEMPLATELIB_TEMPLATES_DELETE_RES,
    EDTYPE_TEMPLATELIB_TEMPLATES_LOAD_REQ,
    EDTYPE_TEMPLATELIB_TEMPLATES_LOAD_RES,
    EDTYPE_TEMPLATELIB_TEMPLATES_MODIFY_REQ,
    EDTYPE_TEMPLATELIB_TEMPLATES_MODIFY_RES,
    EDTYPE_TEMPLATELIB_MODIFY_REQ,
    EDTYPE_TEMPLATELIB_MODIFY_RES,
    EDTYPE_TEMPLATE_RECOVER_RES,
    EDTYPE_TEMPLATE_RECOVER_REQ,
    EDTYPE_TEMPLATELIB_RECOVER_REQ,
    EDTYPE_TEMPLATELIB_RECOVER_RES,
    //
            EDTYPE_TEMPLATELIB_TOSYSTEM_REQ,
    EDTYPE_TEMPLATELIB_TOSYSTEM_RES,
    //
            EDTYPE_SUBMIT_RESULT_REQ,
    EDTYPE_SUBMIT_RESULT_RES,


    EDTYPE_SLAVE_PULSE_REQ,
    EDTYPE_SLAVE_PULSE_RES,
    EDTYPE_TRANS_CONTROL_SUBMIT_REQ,
    EDTYPE_TRANS_CONTROL_SUSPEND_REQ,
    EDTYPE_TRANS_CONTROL_SUSPEND_RES,
    EDTYPE_TRANS_CONTROL_RERUN_REQ,
    EDTYPE_TRANS_CONTROL_RERUN_RES,
    EDTYPE_TRANS_CONTROL_CANCEL_REQ,
    EDTYPE_TRANS_CONTROL_CANCEL_RES,
    EDTYPE_GET_TRANS_SENT_REQ,
    EDTYPE_GET_TRANS_SENT_RES,

    EDTYPE_POSTPROC_REQ,
    EDTYPE_POSTPROC_RES,

    EDTYPE_CH_SEG_INSERT_REQ,
    EDTYPE_CH_SEG_DELETE_REQ,
    EDTYPE_CH_SEG_MODIFY_REQ,
    EDTYPE_CH_SEG_INSERT_RES,
    EDTYPE_CH_SEG_DELETE_RES,
    EDTYPE_CH_SEG_MODIFY_RES,
    EDTYPE_CH_SEG_LOAD_REQ,
    EDTYPE_CH_SEG_LOAD_RES,
    EDTYPE_CH_SEG_RECOVERY_REQ,
    EDTYPE_CH_SEG_RECOVERY_RES,

    /////////////////
            EDTYPE_AFTER_DICT_CREATE_REQ,
    EDTYPE_AFTER_DICT_CREATE_RES,
    EDTYPE_AFTER_DICT_MODIFY_REQ,
    EDTYPE_AFTER_DICT_MODIFY_RES,
    EDTYPE_AFTER_DICT_DELETE_REQ,
    EDTYPE_AFTER_DICT_DELETE_RES,
    EDTYPE_AFTER_DICT_WORDS_INSERT_REQ,
    EDTYPE_AFTER_DICT_WORDS_INSERT_RES,
    EDTYPE_AFTER_DICT_WORDS_DELETE_REQ,
    EDTYPE_AFTER_DICT_WORDS_DELETE_RES,
    EDTYPE_AFTER_DICT_WORDS_LOAD_REQ,
    EDTYPE_AFTER_DICT_WORDS_LOAD_RES,
    EDTYPE_AFTER_DICT_WORDS_MODIFY_REQ,
    EDTYPE_AFTER_DICT_WORDS_MODIFY_RES,

    EDTYPE_DICT_AFTER_WORDS_RECOVER_REQ,
    EDTYPE_DICT_AFTER_WORDS_RECOVER_RES,
    EDTYPE_DICT_AFTER_RECOVER_REQ,
    EDTYPE_DICT_AFTER_RECOVER_RES,

    EDTYPE_DICT_WORDS_DOWNLOAD_REQ,
    EDTYPE_DICT_WORDS_DOWNLOAD_RES,
    //END
            EDTYPE_END
} EDType;

class EventData
{
public:
    EventData(const CallID & cid): m_call_id(cid) {}
    EventData(void): m_call_id(NULL_CALL_ID) {};
    virtual ~EventData(void) {};

    const CallID GetCallID() const
    {
        return m_call_id;
    };

    void SetCallID(const CallID & cid)
    {
        m_call_id = cid;
    };

    // 由子类实现,返回EventData的type,纯虚函数
    virtual const EDType GetType() const = 0;

protected:
    CallID m_call_id;   // 发送方的调用ID
};

#endif //PROJECT_EVENTDATA_H
