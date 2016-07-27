//
// Created by ¡ı Ê¬¸ on 16/7/22.
//

#ifndef TRANSMASTER_NETINTERFACE_H
#define TRANSMASTER_NETINTERFACE_H

#include "AsyncNetDriver.h"
#include "Log/LogStream.h"

class NetInterface
{
public:
    static bool Initialize();

    static AsyncNetDriver * GetUsrNet();
    static AsyncNetDriver * GetSerNet();

private:
    static AsyncNetDriver ms_usr_net;
    static AsyncNetDriver ms_ser_net;
};

#endif //TRANSMASTER_NETINTERFACE_H
