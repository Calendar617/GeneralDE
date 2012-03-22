#include "cpepp/dr/Meta.hpp"
#include "cpepp/dr/MetaLib.hpp"
#include "gdpp/app/Log.hpp"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usfpp/bpg_pkg/PackageManager.hpp"

namespace Usf { namespace Bpg {

Cpe::Dr::MetaLib const & PackageManager::dataMetaLib(void) const {
    LPDRMETALIB metalib = bpg_pkg_manage_data_metalib(*this);
    if (metalib == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app(),
            ::std::runtime_error,
            "bpg_pkg_manage %s: no data metalib",
            name().c_str());
    }

    return Cpe::Dr::MetaLib::_cast(metalib);
}

Cpe::Dr::Meta const &
PackageManager::cmdMeta(void) const {
    LPDRMETA meta = bpg_pkg_manage_cmd_meta(*this);
    if (meta == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app(),
            ::std::runtime_error,
            "%s: Usf::Bpg::Package::cmdMeta: not cmd meta!",
            name().c_str());
    }

    return Cpe::Dr::Meta::_cast(meta);
}

bpg_pkg_t PackageManager::createPackage(size_t capacity, LPDRMETA carry_data_meta, size_t carry_data_capacity) {
    bpg_pkg_t pkg = bpg_pkg_create(*this, capacity, carry_data_meta, carry_data_capacity);
    if (pkg == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app(),
            ::std::runtime_error,
            "bpg_pkg_manage %s: crate pkg fail, data-capacity=%zd, caary-capacity=%zd!",
            name().c_str(),
            capacity, carry_data_capacity);
    }
    return pkg;
}

PackageManager & PackageManager::_cast(bpg_pkg_manage_t pkg_manage) {
    if (pkg_manage == NULL) {
        throw ::std::runtime_error("Usf::Bpg::PackageManager::_cast: input pkg_manage is NULL!");
    }

    return *(PackageManager*)pkg_manage;
}

PackageManager & PackageManager::instance(gd_app_context_t app, const char * name) {
    bpg_pkg_manage_t pkg_manage = bpg_pkg_manage_find_nc(app, name);
    if (pkg_manage == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app,
            ::std::runtime_error,
            "bpg_pkg_manage %s not exist!", name ? name : "default");
    }

    return *(PackageManager*)pkg_manage;
}

}}
