//
// Created by ������ on 16/9/11.
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

    // multi word 2 multi dictģʽ,��һ�濼���޸�
    class BaseWord
            : public boost::enable_shared_from_this<BaseWord>
    {
    public:
        string key;
        string src;
        string tgt;

        map<dict_ptr, UWord> dict_map;  // �����ʵ�ļ���

        string DebugInfo(dict_ptr sp_dict) const;

        // ���ش����û�Ϊusrid��ϵͳ�ʵ�,domainΪdomain_info��DOMAIN_ALL��dict_map�е�dict����
        vector<dict_ptr> CheckOwner(const UsrID & usrid, const DomainType & domain_info);
    };
}

#endif //TRANSMASTER_BASEWORD_H
