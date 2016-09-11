//
// Created by 刘舒曼 on 16/9/11.
//

#ifndef TRANSMASTER_DICTIONARY_H
#define TRANSMASTER_DICTIONARY_H

#include "BaseWord.h"
#include "DictDef.h"

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <map>
using namespace std;

namespace dictionary
{
    // 字典类,纪录字典的info,weight和字典中的word
    class Dictionary
            : public boost::enable_shared_from_this<Dictionary>
    {
    public:
        Dictionary(const DictInfo & _dict_info) : dict_info(_dict_info) {}
        ~Dictionary(){}

        string DebugInfo();
    private:
        DictInfo dict_info;
        string weight;

        map<WordID, word_ptr> word_map;
    };

    typedef boost::shared_ptr<Dictionary> dict_ptr;

}

#endif //TRANSMASTER_DICTIONARY_H
