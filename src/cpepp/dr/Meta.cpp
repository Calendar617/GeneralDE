#include <sstream>
#include <stdexcept>
#include "cpe/dr/dr_cfg.h"
#include "cpepp/dr/Meta.hpp"
#include "cpepp/utils/ErrorCollector.hpp"

namespace Cpe { namespace Dr {

Entry const &
Meta::entryAt(int idx) const {
    LPDRMETAENTRY r = dr_meta_entry_at(*this, idx);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "meta " << name() << ": get entry at " << idx << " fail!";
        throw ::std::runtime_error(os.str());
    }
    return *(Entry const *)r;
}

int Meta::entryIdx(const char * name) const {
    int pos = findEntryIdx(name);
    if (pos < 0) {
        ::std::ostringstream os;
        os << "meta " << this->name() << ": get entry (name=" << name << ") fail!";
        throw ::std::runtime_error(os.str());
    }
    return pos;
}

int Meta::entryIdx(int id) const {
    int pos = findEntryIdx(id);
    if (pos < 0) {
        ::std::ostringstream os;
        os << "meta " << name() << ": get entry (id=" << id << ") fail!";
        throw ::std::runtime_error(os.str());
    }
    return pos;
}

Entry const * Meta::findEntry(int id) const {
    int pos = findEntryIdx(id);
    return pos < 0 ? NULL : (Entry const *)dr_meta_entry_at(*this, pos);
}

Entry const & Meta::entry(const char * name) {
    Entry const * r = findEntry(name);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "meta " << this->name() << ": get entry (name=" << name << ") fail!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

Entry const & Meta::entry(int id) const {
    Entry const * r = findEntry(id);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "meta " << name() << ": get entry (id=" << id << ") fail!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

Entry const &
Meta::entryByPath(const char * path) const {
    Entry const * r = findEntryByPath(path);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "meta " << name() << ": get entry at " << path << " fail!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

Meta const & Meta::_cast(LPDRMETA meta) {
    if (meta == NULL) {
        throw ::std::runtime_error("cast to Meta: input is NULL");
    }
    return *(Meta const *)meta;
}

void Meta::load_from_cfg(void * data, size_t capacity, cfg_t cfg) const {
    Utils::ErrorCollector ec;
    if (dr_cfg_read(data, capacity, cfg, *this, ec) != 0) {
        ec.checkThrowWithMsg< ::std::runtime_error>();
    }
}

bool Meta::try_load_from_cfg(void * data, size_t capacity, cfg_t cfg) const {
    if (dr_cfg_read(data, capacity, cfg, *this, 0) != 0) {
        bzero(data, capacity);
        return false;
    }
    else {
        return true;
    }
}

}}
