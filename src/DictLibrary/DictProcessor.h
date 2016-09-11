//
// Created by ¡ı Ê¬¸ on 16/9/11.
//

#ifndef TRANSMASTER_DICTPROCESSOR_H
#define TRANSMASTER_DICTPROCESSOR_H

#include "EventEngine/EventEngine.h"
#include "Log/LogStream.h"
#include "Common/ICUConverter.h"
#include "DictEvent.h"
#include "DictManager.h"

#include <utility>

using namespace std;

class DictProcessor
        : public EventEngine
{
public:
    ~DictProcessor(){}
    static DictProcessor * GetInstance();
    bool Start(const string & default_file_path = "./");
private:
    static DictProcessor ms_instance;

    string m_default_file_path;

    DictProcessor(){}

    bool Start(void) {return fasle;}
    void on_event(Event & e);

    EventData * on_create_dict(EventData * p_edata);
    EventData * on_modify_dict(EventData * p_edata);
    EventData * on_delete_dict(EventData * p_edata);
    EventData * on_insert_word(EventData * p_edata);
    EventData * on_delete_word(EventData * p_edata);
    EventData * on_modify_word(EventData * p_edata);
    EventData * on_load_word_file(EventData * p_edata);
    EventData * on_to_sys_dict(EventData * p_edata);
    EventData * on_recover_word(EventData * p_edata);
    EventData * on_recover_dict(EventData * p_edata);

    bool load_word_file(const string & file_path, const bool is_utf8, vector<pair<string, string> > & word_vec);
};

#endif //TRANSMASTER_DICTPROCESSOR_H
