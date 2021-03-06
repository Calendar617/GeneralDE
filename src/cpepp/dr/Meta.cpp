#include <sstream>
#include <stdexcept>
#include "cpe/pal/pal_strings.h"
#include "cpe/utils/stream_buffer.h"
#include "cpe/dr/dr_cfg.h"
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_json.h"
#include "cpepp/dr/Meta.hpp"
#include "cpepp/dr/MetaLib.hpp"
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

Entry const & Meta::entry(const char * name) const {
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

const char * Meta::dump_data(mem_buffer_t buffer, const void * data) const {
    write_stream_buffer stream = CPE_WRITE_STREAM_BUFFER_INITIALIZER(buffer);

    dump_data((write_stream_t)&stream, data);

    return (const char *)mem_buffer_make_continuous(buffer, 0);
}

void Meta::dump_data(write_stream_t stream, const void * data) const {
    dr_json_print(
        stream,
        data,
        *this,
        DR_JSON_PRINT_BEAUTIFY,
        0);
}

void Meta::load_from_cfg(void * data, size_t capacity, cfg_t cfg, int policy) const {
    Utils::ErrorCollector ec;
    if (dr_cfg_read(data, capacity, cfg, *this, policy, ec) <= 0) {
        ec.checkThrowWithMsg< ::std::runtime_error>();
    }
}

bool Meta::try_load_from_cfg(void * data, size_t capacity, cfg_t cfg, error_monitor_t em, int policy) const {
    if (dr_cfg_read(data, capacity, cfg, *this, policy, em) <= 0) {
        bzero(data, capacity);
        return false;
    }
    else {
        return true;
    }
}

void Meta::write_to_cfg(cfg_t cfg, const void * data) const {
    Utils::ErrorCollector ec;
    if (dr_cfg_write(cfg, data, *this, ec) != 0) {
        ec.checkThrowWithMsg< ::std::runtime_error>();
    }
}

bool Meta::try_write_to_cfg(cfg_t cfg, const void * data,  error_monitor_t em) const {
    return dr_cfg_write(cfg, data, *this, em) == 0 ? true : false;
}

void Meta::set_defaults(void * data, size_t capacity, int policy) const {
    dr_meta_set_defaults(data, capacity, *this, policy);
}

void Meta::copy_same_entries(
    void * data, size_t capacity,
    const void * src, LPDRMETA srcMeta, size_t srcCapacity,
    int policy, error_monitor_t em) const
{
    dr_meta_copy_same_entry(
        data, capacity, *this,
        src, (srcCapacity == 0 ? dr_meta_size(srcMeta) : srcCapacity), srcMeta,
        policy, em);
}

void Meta::copy_same_entries(
    void * data, size_t capacity,
    const void * srcData, const char * srcMeta, size_t srcCapacity,
    int policy, error_monitor_t em) const
{
    copy_same_entries(
        data, capacity,
        srcData, MetaLib::_cast(dr_meta_owner_lib(*this)).meta(srcMeta), srcCapacity,
        policy, em);
}

}}
