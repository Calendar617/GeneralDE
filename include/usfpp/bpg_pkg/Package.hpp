#ifndef USFPP_BPG_PKG_PACKAGE_H
#define USFPP_BPG_PKG_PACKAGE_H
#include "cpepp/utils/ClassCategory.hpp"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "System.hpp"

namespace Usf { namespace Bpg {

class Package : public Cpe::Utils::SimulateObject {
public:
    operator bpg_pkg_t() const { return (bpg_pkg_t)this; }

    PackageManager & mgr(void) { return *(PackageManager*)bpg_pkg_mgr(*this); }
    PackageManager const & mgr(void) const { return *(PackageManager*)bpg_pkg_mgr(*this); }

    void init(void) { bpg_pkg_init(*this); }

    uint32_t cmd(void) const { return bpg_pkg_cmd(*this); }
    void setCmd(uint32_t cmd) { bpg_pkg_set_cmd(*this, cmd); }

    uint32_t errCode(void) const { return bpg_pkg_errno(*this); }
    void setErrCode(uint32_t en) { bpg_pkg_set_errno(*this, en); }

    uint32_t clientId(void) const { return bpg_pkg_client_id(*this); }
    void setClientId(uint32_t client_id) { bpg_pkg_set_client_id(*this, client_id); }

    static Package & _cast(bpg_pkg_t pkg);
    static Package & _cast(gd_dp_req_t req);
};

}}

#endif
