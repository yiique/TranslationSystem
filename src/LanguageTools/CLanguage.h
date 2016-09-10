//
// Created by ¡ı Ê¬¸ on 16/7/29.
//

#ifndef TRANSMASTER_CLANGUAGE_H
#define TRANSMASTER_CLANGUAGE_H

#include "Common/f_utility.h"
#include "Common/Utf8Stream.h"
#include "Common/BaseMutex.h"
#include "Configurator/Configurator.h"
#include "Log/LogStream.h"
#include "TransText/TransText.h"
#include "SentSpliter.h"

#include "boost/asio.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class CLanguage
{
public:
    static void Utf8A2B(string & src);
    static string & SplitStrByChar(string & str);
    static void StandardSentFilter(SentProcCell & sent_cell);
private:
    static string & utf8_char_a2b(string & utf8_char);
};



#endif //TRANSMASTER_CLANGUAGE_H
