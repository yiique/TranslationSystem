//
// Created by 刘舒曼 on 16/9/11.
//

#ifndef TRANSMASTER_BASEWORD_H
#define TRANSMASTER_BASEWORD_H

#include "DictDef.h"
#include "Dictionary.h"
#include "Log/LogStream.h"

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <string>
#include <set>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

namespace dictionary
{
    class Dictionary;
    typedef boost::shared_ptr<Dictionary> dict_ptr;
    typedef boost::shared_ptr<BaseWord> word_ptr;

    class UWord
    {
    public:
        size_t word_id;
        int is_active;
        int is_checked;
    };

    // multi word 2 multi dict模式,下一版考虑修改
    class BaseWord
            : public boost::enable_shared_from_this<BaseWord>
    {
    public:
        string key;
        string src;
        string tgt;

        map<dict_ptr, UWord> dict_map;  // 所属词典的集合

        string DebugInfo(dict_ptr sp_dict) const;

        // 返回创建用户为usrid或系统词典,domain为domain_info或DOMAIN_ALL的dict_map中的dict向量
        vector<dict_ptr> CheckOwner(const UsrID & usrid, const DomainType & domain_info);
    };
}

#endif //TRANSMASTER_BASEWORD_H
