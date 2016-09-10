//
// Created by ������ on 16/7/21.
//
// �Զ��屨�Ľṹ,���������������ݺ����ɱ�������
// ��������:
// head_type    eg:Request: 1.0\r\n
// msg_type     eg:Msg-Type: xxx\r\n
// body_size    eg:Content-Length: xxx\r\n
// �ָ���        eg:\r\n
// body
//

#ifndef TRANSMASTER_NETPACKET_H
#define TRANSMASTER_NETPACKET_H

#include "Common/f_utility.h"
#include "Log/LogStream.h"
#include "NetType.h"

#include <string>
#include <sstream>
using namespace std;

typedef enum
{
    WRITE_FINISH,
    WRITE_ERR_MAXHEAD,
    WRITE_ERR_INPUT,
    WRITE_ERR_MSGTYPE,
    WRITE_ERR_CONTENTLEN,
    WRITE_NORMAL,
    PARSE_HEAD_SUC
}PACKET_STATE_T;

typedef enum
{
    HEAD_REQUEST,
    HEAD_RESPONSE
}PACKET_HEAD_T;

#define MAX_HEAD_SIZE 1024          //����ͷ��󳤶�
//#define DEF_MAX_BODY_SIZE 1024000   //��������󳤶�


//����ͷʶ��״̬��
typedef struct headstatemachine
{
    headstatemachine(char b, headstatemachine *next): input_b(b),  next_state(next) {};

    char input_b;
    headstatemachine *next_state;

} HeadStateMachine;


class NetPacket
{
public:

    NetPacket(sock_t sock);
    ~NetPacket(void);

    //Get & Set
    sock_t GetSock()
    {
        return m_sock;
    };

    void SetSock(sock_t sockfd)
    {
        m_sock = sockfd;
    };

    const char * GetData()
    {
        return m_data.c_str();
    };

    size_t GetSize()
    {
        return m_data.size();
    };

    bool GetMsgType(string & msgtype)
    {
        if(!m_is_good)
            return false;

        msgtype = m_msg_type;

        return true;
    }

    bool GetBody(string & body)
    {
        if(!m_is_good)
            return false;

        body = m_data.substr(m_body_pos, m_data.size()-m_body_pos);

        return true;

    };

    bool IsGood()
    {
        return m_is_good;
    };

    //��ʽд��  ���ڴ��������н���NetPacket
    PACKET_STATE_T Write(const char * buf, size_t len);

    //һ����д�� ����������Ӧ����
    PACKET_STATE_T Write(PACKET_HEAD_T head_type,
                         const string &msg_type,
                         const string &body,
                         const string &head_res_code = HEAD_RESPONSE_200);


    void Clear()
    {
        m_curr_hsm = &NetPacket::HSM_Start;
        m_write_offset = 0;
        m_body_size = 0;
        m_body_pos = 0;
        m_is_good = false;
        m_data.clear();
    };


private:

    static const string HEAD_REQUEST_TAG;
    static const string HEAD_RESPONSE_TAG;
    static const string HEAD_MSGTYPE_TAG;
    static const string HEAD_LENGTH_TAG;
    static const string HEAD_SEP_TAG;

    static const string HEAD_REQUEST_VER;
    static const string HEAD_RESPONSE_200;

    //��Ϣͷʶ���,�������б�head��body�ķָ���:\r\n\r\n
    static HeadStateMachine HSM_Start;
    static HeadStateMachine HSM_Fir_R;
    static HeadStateMachine HSM_Fir_N;
    static HeadStateMachine HSM_Sec_R;
    static HeadStateMachine HSM_End_N;

    sock_t m_sock;

    PACKET_STATE_T m_state;
    PACKET_HEAD_T m_head_type;

    HeadStateMachine * m_curr_hsm;

    int m_write_offset;

    string m_data;          //����ȫ��

    string m_msg_type;      //��Ϣ����
    size_t m_body_size;     //body����size
    size_t m_body_pos;      //body��ʼλ��

    bool m_is_good;

    //��������ͷ
    PACKET_STATE_T parse_head();

};


#endif //TRANSMASTER_NETPACKET_H
