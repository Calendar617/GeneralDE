#include <sstream>
#include <stdexcept>
#include "cpe/dr/dr_cfg.h"
#include "cpe/utils/stream_buffer.h"
#include "cpe/dr/dr_json.h"
#include "cpepp/utils/ErrorCollector.hpp"
#include "tdr/tdr.h"
#include "tsf4gpp/tdr/Meta.hpp"

namespace Tsf4g { namespace Tdr {

int Meta::findEntryIdx(const char * name) const {
    int buf;
    return tdr_get_entry_by_name(&buf, *this, name) >= 0
        ? buf
        : -1;
}

int Meta::findEntryIdx(int id) const { 
    int buf;
    return tdr_get_entry_by_id(&buf, *this, id) >= 0
        ? buf
        : -1;
}

Entry const *
Meta::findEntryByPath(const char * path) const { 
    LPTDRMETAENTRY entry;
    TDROFF off;

    return (Entry const *)(
        tdr_entry_path_to_off(*this, &entry, &off, path) == 0
        ? entry
        : NULL);
}

Entry const &
Meta::entryAt(int idx) const {
    LPTDRMETAENTRY r = tdr_get_entry_by_index(*this, idx);
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
    return pos < 0 ? NULL : (Entry const *)tdr_get_entry_by_index(*this, pos);
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

Meta const & Meta::_cast(LPTDRMETA meta) {
    if (meta == NULL) {
        throw ::std::runtime_error("cast to Meta: input is NULL");
    }
    return *(Meta const *)meta;
}

size_t Meta::encode(void * buf, size_t capacity, const void * data, size_t data_size, int version) const {
    TDRDATA stNetInfo;
    TDRDATA stHostInfo;

    stNetInfo.iBuff = capacity;
    stNetInfo.pszBuff = (char*)buf;
	    
    stHostInfo.iBuff = data_size;
    stHostInfo.pszBuff = (char*)data;
    
    if (version < 0) version = currentVersion();

    int iRet = tdr_hton(*this, &stNetInfo, &stHostInfo, version);
    if(TDR_ERR_IS_ERROR(iRet)) {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s encode fail: %s", name().c_str(), tdr_error_string(iRet));
        throw ::std::runtime_error(buf);
    }

    return stNetInfo.iBuff;
}

size_t Meta::decode(void * buf, size_t capacity, const void * data, size_t data_size, int version) const {
    TDRDATA stNetInfo;
    TDRDATA stHostInfo;

    stHostInfo.iBuff = capacity;
    stHostInfo.pszBuff = (char*)buf;
	    
    stNetInfo.iBuff = data_size;
    stNetInfo.pszBuff = (char*)data;
    
    if (version < 0) version = currentVersion();

    int iRet = tdr_ntoh(*this, &stHostInfo, &stNetInfo, version);
    if(TDR_ERR_IS_ERROR(iRet)) {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s decode fail: %s", name().c_str(), tdr_error_string(iRet));
        throw ::std::runtime_error(buf);
    }

    return stHostInfo.iBuff;
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

void Meta::load_from_cfg(void * data, size_t capacity, cfg_t cfg) const {
    Cpe::Utils::ErrorCollector ec;
    if (dr_cfg_read(data, capacity, cfg, *this, ec) <= 0) {
        ec.checkThrowWithMsg< ::std::runtime_error>();
    }
}

bool Meta::try_load_from_cfg(void * data, size_t capacity, cfg_t cfg) const {
    if (dr_cfg_read(data, capacity, cfg, *this, 0) <= 0) {
        bzero(data, capacity);
        return false;
    }
    else {
        return true;
    }
}

}}
