#include <cassert>
#include "cpepp/utils/ErrorCollector.hpp"
#include "cpepp/dr/Data.hpp"
#include "cpepp/dr/Entry.hpp"
#include "cpepp/dr/Meta.hpp"
#include "cpepp/dr/MetaLib.hpp"
#include "gdpp/app/Log.hpp"
#include "usfpp/bpg_pkg/Package.hpp"
#include "usfpp/bpg_pkg/PackageManager.hpp"

namespace Usf { namespace Bpg {

Gd::App::Application & Package::app(void) {
    return mgr().app();
}

Gd::App::Application const & Package::app(void) const {
    return mgr().app();
}

Cpe::Dr::MetaLib const &
Package::dataMetaLib(void) const {
    LPDRMETALIB metalib = bpg_pkg_data_meta_lib(*this);
    if (metalib == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app(),
            ::std::runtime_error,
            "%s: Usf::Bpg::Package::dataMetaLib: not data meta lib!",
            mgr().name().c_str());
    }

    return *(Cpe::Dr::MetaLib const *)metalib;
}

Cpe::Dr::Meta const &
Package::cmdMeta(void) const {
    return mgr().cmdMeta();
}

Cpe::Dr::Meta const &
Package::mainDataMeta(void) const {
    Cpe::Utils::ErrorCollector em;

    LPDRMETA meta = bpg_pkg_main_data_meta(*this, em);
    if (meta == NULL) {
        em.checkThrowWithMsg< ::std::runtime_error>();
    }

    return Cpe::Dr::Meta::_cast(meta);
}

void Package::setCmdAndData(Cpe::Dr::ConstData const & data, size_t * write_size) {
    setCmd(cmdMeta().entry(data.meta().name()).id());
    setMainData(data.data(), data.capacity(), write_size);
}

void Package::setCmdAndData(Cpe::Dr::ConstData const & data, size_t size, size_t * write_size) {
    setCmd(cmdMeta().entry(data.meta().name()).id());
    setMainData(data.data(), size, write_size);
}

void Package::setCmdAndData(int cmd, const void * data, size_t data_size, size_t * write_size) {
    setCmd(cmd);
    setMainData(data, data_size, write_size);
}
        
void Package::setMainData(void const * data, size_t size, size_t * write_size) {
    Cpe::Utils::ErrorCollector em;
    if (bpg_pkg_set_main_data(*this, mainDataMeta(), data, size, write_size, em) != 0) {
        em.checkThrowWithMsg< ::std::runtime_error>();
    }
}

void Package::mainData(void * buf, size_t capacity, size_t * size) const {
    Cpe::Utils::ErrorCollector em;

    if (bpg_pkg_get_main_data(*this, mainDataMeta(), buf, &capacity, em) != 0) {
        em.checkThrowWithMsg< ::std::runtime_error>();
    }

    if (size) *size = capacity;
}

void Package::mainData(Cpe::Dr::Data & data) {
    size_t real_capacity;
    mainData(data.data(), data.capacity(), &real_capacity);
    data.setCapacity(real_capacity);
    data.setMeta(mainDataMeta());
}

void Package::addAppendData(const char * metaName, void const * data, size_t size, size_t * write_size) {
    addAppendData(dataMetaLib().meta(metaName), data, size, write_size);
}

void Package::addAppendData(int metaid, void const * data, size_t size, size_t * write_size) {
    addAppendData(dataMetaLib().meta(metaid), data, size, write_size);
}

void Package::addAppendData(LPDRMETA meta, void const * data, size_t size, size_t * write_size) {
    Cpe::Utils::ErrorCollector em;

    if (bpg_pkg_add_append_data(*this, meta, data, size, write_size, em)) {
        em.checkThrowWithMsg< ::std::runtime_error>();
    }
}

void Package::appendData(int metaId, void * buf, size_t capacity, size_t * size) const {
    appendData(dataMetaLib().meta(metaId), buf, capacity, size);
}

void Package::appendData(const char * metaName, void * buf, size_t capacity, size_t * size) const {
    appendData(dataMetaLib().meta(metaName), buf, capacity, size);
}

void Package::appendData(LPDRMETA meta, void * buf, size_t capacity, size_t * size) const {
    bpg_pkg_append_info_t appendInfo = 0;
    for(int32_t i = 0; i < bpg_pkg_append_info_count(*this); ++i) {
        appendInfo = bpg_pkg_append_info_at(*this, i);
        if ((int)bpg_pkg_append_info_id(appendInfo) == (int)dr_meta_id(meta)) {
            Cpe::Utils::ErrorCollector em;
            if (bpg_pkg_get_append_data(*this, appendInfo, meta, buf, &capacity, em) != 0) {
                em.checkThrowWithMsg< ::std::runtime_error>();
            }

            if (size) *size = capacity;
            return;
        }
    }

    APP_CTX_THROW_EXCEPTION(
        app(),
        ::std::runtime_error,
        "%s: Usf::Bpg::Package::appendData: no append info of meta %s(%d)!",
        mgr().name().c_str(), dr_meta_name(meta), dr_meta_id(meta));
}

bool Package::tryGetAppendData(int metaId, void * buf, size_t capacity, size_t * size) const {
    LPDRMETALIB metalib = bpg_pkg_data_meta_lib(*this);
    if (metalib == NULL) return false;

    LPDRMETA meta = dr_lib_find_meta_by_id(metalib, metaId);
    if (meta == NULL) return false;

    return tryGetAppendData(meta, buf, capacity, size);
}

bool Package::tryGetAppendData(const char * metaName, void * buf, size_t capacity, size_t * size) const {
    LPDRMETALIB metalib = bpg_pkg_data_meta_lib(*this);
    if (metalib == NULL) return false;

    LPDRMETA meta = dr_lib_find_meta_by_name(metalib, metaName);
    if (meta == NULL) return false;

    return tryGetAppendData(meta, buf, capacity, size);
}

bool Package::tryGetAppendData(LPDRMETA meta, void * buf, size_t capacity, size_t * size) const {
    bpg_pkg_append_info_t appendInfo = 0;
    for(int32_t i = 0; i < bpg_pkg_append_info_count(*this); ++i) {
        appendInfo = bpg_pkg_append_info_at(*this, i);
        if ((int)bpg_pkg_append_info_id(appendInfo) == (int)dr_meta_id(meta)) {
            if (bpg_pkg_get_append_data(*this, appendInfo, meta, buf, &capacity, NULL) != 0) {
                return false;
            }
            else {
                if (size) *size = capacity;
                return true;
            }
        }
    }

    return false;
}

Package & Package::_cast(bpg_pkg_t pkg) {
    if (pkg == NULL) {
        throw ::std::runtime_error("Usf::Bpg::Package::_cast: input pkg is NULL!");
    }

    return *(Package*)pkg;
}

Package & Package::_cast(dp_req_t req) {
    if (req == NULL) {
        throw ::std::runtime_error("Usf::Bpg::Package::_cast: input req is NULL!");
    }

    bpg_pkg_t pkg = bpg_pkg_from_dp_req(req);
    if (pkg == NULL) {
        throw ::std::runtime_error("Usf::Bpg::Package::_cast: cast dp_req to pkg fail!");
    }

    return *(Package*)pkg;
}

}}
