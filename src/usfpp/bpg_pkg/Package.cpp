#include "gdpp/app/Log.hpp"
#include "usfpp/bpg_pkg/Package.hpp"

namespace Usf { namespace Bpg {

Package & Package::_cast(bpg_pkg_t pkg) {
    if (pkg == NULL) {
        throw ::std::runtime_error("Usf::Bpg::Package::_cast: input pkg is NULL!");
    }

    return *(Package*)pkg;
}

Package & Package::_cast(gd_dp_req_t req) {
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
