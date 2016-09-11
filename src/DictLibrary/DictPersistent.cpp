//
// Created by ������ on 16/9/11.
//

#include "DictPersistent.h"

using namespace dictionary;



// GetMaxDictWordID
// InsertWord   DeleteWord  ModifyWord
// CreateDict   ModifyDict  DeleteDict
// LoadAllDict  LoadAllWord LoadWord    ModifyImportStatus
// downloadWords

// ����
// GetRecoverWordInfo   RecoverWord





// ����,ɾ��,�޸�Dict
int DictPersistent::CreateDict(DictInfo & dict_info)
{
    return DBOperation::CreateDict(dict_info);
}

int DictPersistent::DeleteDict(const DictID dict_id)
{
    return DBOperation::DeleteDict(dict_id);
}

int DictPersistent::ModifyDict(const DictInfo & dict_info)
{
    return DBOperation::ModifyDict(dict_info);
}

// ����,ɾ��,�޸�word
int DictPersistent::InsertWord(const DictID dict_id, const WordInfo & word_info)
{
    return DBOperation::InsertWord(dict_id, word_info);
}

int DictPersistent::InsertWord(const DictID dict_id, const vector<WordInfo> & word_info_vec)
{
    return DBOperation::InsertWord(dict_id, word_info_vec);
}

int DictPersistent::DeleteWord(const WordID word_id)
{
    return DBOperation::DeleteWord(word_id);
}

int DictPersistent::ModifyWord(const WordInfo & word_info)
{
    return DBOperation::ModifyWord(word_info);
}

// load�������
int DictPersistent::LoadWord(const vector<WordID> & word_id_vec, vector<WordInfo> & word_info_vec)
{
    return DBOperation::LoadWord(word_id_vec, word_info_vec);
}

int DictPersistent::LoadAllDict(vector<DictInfo> & dict_info_vec)
{
    return DBOperation::LoadAllDict(dict_info_vec);
}

int DictPersistent::LoadAllWord(vector<WordInfo> & word_info_vec)
{
    return DBOperation::LoadAllWord(word_info_vec);
}

// import ״̬,��ʵ����û�������(Ӧ�ñ�����)
int DictPersistent::ModifyImportStatus(const vector<WordID> & word_id_vec)
{
    return DBOperation::ModifyImportStatus(word_id_vec);
}

// ��ȡ���word_id
int DictPersistent::GetMaxDictWordID(DictID & dict_id, WordID & word_id)
{
    return DBOperation::GetMaxDictWordID(dict_id, word_id);
}

// ----------------δʹ��----------------
// �ָ���ɾ������
int DictPersistent::GetRecoverWordInfo(const WordID & word_id, WordInfo & word_info)
{
    return DBOperation::GetRecoverWordInfo(word_id, word_info);
}

int DictPersistent::RecoverWord(const WordID word_id)
{
    return DBOperation::RecoverWord(word_id);
}
