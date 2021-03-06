//
// Created by 刘舒曼 on 16/7/21.
//

#include "NetPacket.h"

const string NetPacket::HEAD_REQUEST_TAG("Request:");
const string NetPacket::HEAD_RESPONSE_TAG("Response:");
const string NetPacket::HEAD_MSGTYPE_TAG("Msg-Type:");
const string NetPacket::HEAD_LENGTH_TAG("Content-Length:");
const string NetPacket::HEAD_SEP_TAG("\r\n");

const string NetPacket::HEAD_REQUEST_VER("1.0");
const string NetPacket::HEAD_RESPONSE_200("200 OK");

HeadStateMachine NetPacket::HSM_Start('\r',  &NetPacket::HSM_Fir_R);
HeadStateMachine NetPacket::HSM_Fir_R('\n', &NetPacket::HSM_Fir_N);
HeadStateMachine NetPacket::HSM_Fir_N('\r', &NetPacket::HSM_Sec_R);
HeadStateMachine NetPacket::HSM_Sec_R('\n', &NetPacket::HSM_End_N);
HeadStateMachine NetPacket::HSM_End_N('0' , NULL);

NetPacket::NetPacket(sock_t sock): m_sock(sock),
                                   m_write_offset(0),
                                   m_body_size(0),
                                   m_body_pos(0),
                                   m_curr_hsm(&NetPacket::HSM_Start),
                                   m_is_good(false)
{
}

NetPacket::~NetPacket(void)
{
}

//流模式读取data,解析成为NetPacket结构
PACKET_STATE_T NetPacket::Write(const char * buf, size_t len)
{
    if(!buf || len == 0)
        return WRITE_ERR_INPUT;

    size_t idx = 0;
    char byte;

    //读取报文头
    while( m_curr_hsm != &NetPacket::HSM_End_N )
    {
        if(idx >= len)  //报文为空
        {
            return WRITE_NORMAL;
        }

        if(buf[idx] == '\0'){
            ++idx;
            continue;
        }

        //写入头部
        byte = buf[idx];
        m_data += byte;

        //步进状态机
        if( byte == m_curr_hsm->input_b )
            m_curr_hsm = m_curr_hsm->next_state;
        else
            m_curr_hsm = &NetPacket::HSM_Start;

        if(m_curr_hsm == &NetPacket::HSM_End_N)
        {
            //解析报文头
            PACKET_STATE_T res = parse_head();
            if( res != PARSE_HEAD_SUC )
                return res;
        }

        //增加偏移量
        ++idx;

        if(idx >= MAX_HEAD_SIZE)
        {
            //超过最大长度
            return WRITE_ERR_MAXHEAD;
        }
    }

    //开始读取报文体
    if(m_body_size == 0)
    {
        m_is_good = true;
        return WRITE_FINISH;
    }

    while( idx < len)
    {
        if(buf[idx] == '\0'){
            ++idx;
            continue;
        }

        m_data += buf[idx];
        ++idx;

        if(m_data.size()-m_body_pos == m_body_size)
        {
            //接收完毕
            m_is_good = true;
            return WRITE_FINISH;
        }
    }

    return WRITE_NORMAL;
}


//根据NetPacket实例的内容构造报文
PACKET_STATE_T NetPacket::Write(PACKET_HEAD_T head_type,
                                const string &msg_type,
                                const string &body,
                                const string &head_res_code)
{
    stringstream buf;

    m_is_good = false;

    string h_field;
    if( head_type == HEAD_REQUEST )
        h_field = HEAD_REQUEST_TAG + HEAD_REQUEST_VER;
    else
        h_field = HEAD_RESPONSE_TAG +" " + head_res_code;

    //构造报文头
    buf << h_field << HEAD_SEP_TAG;                         //EX: Request: 1.0\r\n
    buf << HEAD_MSGTYPE_TAG << msg_type << HEAD_SEP_TAG;    //EX: Msg-Type: xxx\r\n
    buf << HEAD_LENGTH_TAG << body.size() << HEAD_SEP_TAG;  //EX: Content-Length: xxx\r\n
    buf << HEAD_SEP_TAG;                                    //EX: \r\n

    //构造报文体
    buf << body;

    //给m_data赋值
    m_data = buf.str();

    m_is_good = true;

    return WRITE_FINISH;
}


// 解析已读取入m_data的报头,从中读出m_msg_type, m_body_size, m_body_pos
PACKET_STATE_T NetPacket::parse_head()
{
    //解析"Msg-Type"
    if( !str_tag_find(m_data, NetPacket::HEAD_MSGTYPE_TAG, NetPacket::HEAD_SEP_TAG, m_msg_type) )
        return WRITE_ERR_MSGTYPE;

    del_head_tail_blank(m_msg_type);

    //解析"Content-Length",读入m_body_size
    string sbodylen;
    if( !str_tag_find(m_data, NetPacket::HEAD_LENGTH_TAG, NetPacket::HEAD_SEP_TAG, sbodylen) )
        return WRITE_ERR_CONTENTLEN;

    int bodysize;
    if( !cstr_2_num(sbodylen.c_str(), bodysize) )
        return WRITE_ERR_CONTENTLEN;

    if( bodysize < 0)
        return WRITE_ERR_CONTENTLEN;

    m_body_size = (size_t) bodysize;

    //body置位
    m_body_pos = m_data.size();

    return PARSE_HEAD_SUC;
}