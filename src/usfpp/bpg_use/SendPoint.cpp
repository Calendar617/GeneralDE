#include "cpe/cfg/cfg_read.h"
#include "cpepp/dr/MetaLib.hpp"
#include "cpepp/dr/Meta.hpp"
#include "gdpp/app/Log.hpp"
#include "usfpp/bpg_pkg/Package.hpp"
#include "usfpp/bpg_use/SendPoint.hpp"

namespace Usf { namespace Bpg {


SendPoint::SendPoint(gd_app_context_t app, cfg_t cfg) 
    : m_sp(0)
{
    m_sp = bpg_use_sp_create(app, cfg, gd_app_em(app));
    if (m_sp == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app, ::std::runtime_error,
            "construct Usf::Bpg::SendPoint: crate sp fail");
    }
}

SendPoint::~SendPoint() {
    bpg_use_sp_free(m_sp);
}

PackageManager const &
SendPoint::pkgManager(void) const {
    bpg_pkg_manage_t pkg_manage = bpg_use_sp_pkg_manage(m_sp);
    if (pkg_manage == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app(), ::std::runtime_error,
            "%s: have no pkg_manage!", name().c_str());
    }

    return *(PackageManager*)pkg_manage;
}

void SendPoint::send(Usf::Bpg::Package & pkg) {
    if (bpg_use_sp_send(m_sp, pkg) != 0) {
        APP_CTX_THROW_EXCEPTION(
            app(), ::std::runtime_error,
            "%s: send pkg fail!", name().c_str());
    }
}

void SendPoint::send(Cpe::Dr::Data const & data) {
    Usf::Bpg::Package & pkg = pkgBuf() ;
    pkg.clearData();
    pkg.setErrCode(0);
    pkg.setCmdAndData(data);
    send(pkg);
}

Cpe::Dr::MetaLib const & SendPoint::metaLib(void) const {
    LPDRMETALIB metalib = bpg_use_sp_metalib(m_sp);
    if (metalib == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app(), ::std::runtime_error,
            "%s: meta lib not exist!", name().c_str());
    }

    return Cpe::Dr::MetaLib::_cast(metalib);
}

Cpe::Dr::Meta const & SendPoint::meta(const char * metaName) const {
    LPDRMETA meta = bpg_use_sp_meta(m_sp, metaName);
    if (meta == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app(), ::std::runtime_error,
            "%s: meta %s not exist!", name().c_str(), metaName);
    }

    return Cpe::Dr::Meta::_cast(meta);
}

Usf::Bpg::Package & SendPoint::pkgBuf(void) {
    bpg_pkg_t pkg_buf = bpg_use_sp_pkg_buf(m_sp);
    if (pkg_buf == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app(), ::std::runtime_error,
            "%s: pkg-buf is NULL!", name().c_str());
    }

    return Usf::Bpg::Package::_cast(pkg_buf);
}

Cpe::Dr::Data SendPoint::dataBuf(const char * metaName) {
    void * buf = bpg_use_sp_data_buf(m_sp);
    if (buf == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app(), ::std::runtime_error,
            "%s: data-buf is NULL!", name().c_str());
    }

    return Cpe::Dr::Data(buf, meta(metaName), bpg_use_sp_buf_capacity(m_sp));
}

}}
