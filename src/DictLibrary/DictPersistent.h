//
// Created by ¡ı Ê¬¸ on 16/9/11.
//

#ifndef TRANSMASTER_DICTPERSISTENT_H
#define TRANSMASTER_DICTPERSISTENT_H

#include "DataBase/DBOperation.h"
#include "DictDef.h"

namespace dictionary
{
    class DictPersistent
    {
    public:
        static int CreateDict(DictInfo & dict_info);
        static int ModifyDict(const DictInfo & dict_info);
        static int DeleteDict(const DictID dict_id);

        static int InsertWord(const DictID dict_id, const WordInfo & word_info);
        static int InsertWord(const DictID dict_id, const vector<WordInfo> & word_info_vec);
        static int DeleteWord(const WordID word_id);
        static int ModifyWord(const WordInfo & word_info);

        static int LoadWord(const vector<WordID> & word_id_vec, vector<WordInfo> & word_info_vec);
        static int LoadAllDict(vector<DictInfo> & dict_info_vec);
        static int LoadAllWord(vector<WordInfo> & word_info_vec);

        static int ModifyImportStatus(const vector<WordID> & word_id_vec);

        static int GetMaxDictWordID(DictID & dict_id, WordID & word_id);

        //----------------Œ¥ π”√----------------
        static int GetRecoverWordInfo(const WordID & word_id, WordInfo & word_info);
        static int RecoverWord(const WordID word_id);
    };
}

#endif //TRANSMASTER_DICTPERSISTENT_H
