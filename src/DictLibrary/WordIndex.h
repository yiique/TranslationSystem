//
// Created by ������ on 16/9/11.
//

#ifndef TRANSMASTER_WORDINDEX_H
#define TRANSMASTER_WORDINDEX_H

#include "BaseWord.h"

#include <boost/unordered_map.hpp>

#include <string>
#include <set>
#include <map>
#include <list>

using namespace std;

namespace dictionary
{
    // ����index����,��һ����Ҫ�޸�
    class WordIndex
    {
    public:
        int Size();

        word_ptr Insert(word_ptr & sp_word);
        void Erase(word_ptr & sp_word);
        bool Search(const string & key, list<word_ptr> & word_list);

    private:
        typedef boost::unordered_map<string, set<word_ptr>> word_map_t;

        word_map_t m_word_map;
    };

}

#endif //TRANSMASTER_WORDINDEX_H
