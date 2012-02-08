#ifndef CPEPP_DR_META_H
#define CPEPP_DR_META_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/utils/CString.hpp"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/cfg/cfg_types.h"
#include "System.hpp"

namespace Cpe { namespace Dr {

class Meta : public Cpe::Utils::SimulateObject {
public:
    operator LPDRMETA (void) const { return (LPDRMETA)this; }

    int type(void) const { return dr_meta_type(*this); }

    int baseVersion(void) const { return dr_meta_based_version(*this); }
    int currentVersion(void) const { return dr_meta_current_version(*this); }
 
    Utils::CString const & name(void) const { return Utils::CString::_cast(dr_meta_name(*this)); } 
    Utils::CString const & desc(void) const { return Utils::CString::_cast(dr_meta_desc(*this)); } 
    int id(void) const { return dr_meta_id(*this); } 

    size_t size(void) const { return dr_meta_size(*this); } 
    int align(void) const { return dr_meta_align(*this); } 

    int entryCount(void) const { return dr_meta_align(*this); } 
    Entry const & entryAt(int idx) const;

    int findEntryIdx(const char * name) const { return dr_meta_find_entry_idx_by_name(*this, name); }
    int findEntryIdx(int id) const { return dr_meta_find_entry_idx_by_id(*this, id); }
    int entryIdx(const char * name) const;
    int entryIdx(int id) const;

    Entry const * findEntry(const char * name) const { return (Entry const *)dr_meta_find_entry_by_name(*this, name); }
    Entry const * findEntry(int id) const;
    Entry const & entry(const char * name);
    Entry const & entry(int id) const;

    Entry const * findEntryByPath(const char * path) const { return (Entry const*)dr_meta_find_entry_by_path(*this, path); }
    Entry const & entryByPath(const char * path) const;

    void load_from_cfg(void * data, size_t capacity, cfg_t cfg) const;

    template<typename T>
    void load_from_cfg(T & data, cfg_t cfg) const {
        load_from_cfg(&data, sizeof(data), cfg);
    }

    static Meta const & _cast(LPDRMETA meta);
};

}}

#endif
