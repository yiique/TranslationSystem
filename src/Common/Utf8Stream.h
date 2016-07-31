//
// Created by ¡ı Ê¬¸ on 16/7/30.
//

#ifndef TRANSMASTER_UTF8STREAM_H
#define TRANSMASTER_UTF8STREAM_H

#include <iostream>
#include <string>
using namespace std;

class Utf8Stream
{
public:
    Utf8Stream(const string & utf8_src);
    ~Utf8Stream(void);

    bool ReadOneCharacter(string & utf8_char_str);

private:
    string m_utf8_src;
    size_t m_byte_idx;

    size_t get_utf8_char_len(char & byte);
};

#endif //TRANSMASTER_UTF8STREAM_H
