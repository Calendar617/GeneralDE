#include <stdexcept>
#include "cpe/utils/error.h"
#include "cpepp/utils/ErrorCollector.hpp"
#include "gd/dp/dp_manage.h"
#include "gdpp/dp/Responser.hpp"

namespace Gd { namespace Dp {

void Responser::bind(const char * targt) {
    Cpe::Utils::ErrorCollector ec;
    if (gd_dp_rsp_bind_string(*this, targt, ec) != 0) {
        ec.checkThrowWithMsg< ::std::runtime_error>();
    }
}

void Responser::bind(int32_t target) {
    Cpe::Utils::ErrorCollector ec;
    if (gd_dp_rsp_bind_numeric(*this, target, ec) != 0) {
        ec.checkThrowWithMsg< ::std::runtime_error>();
    }
}

void Responser::bind(cfg_t cfg) {
    Cpe::Utils::ErrorCollector ec;
    if (gd_dp_rsp_bind_by_cfg(*this, cfg, ec) != 0) {
        ec.checkThrowWithMsg< ::std::runtime_error>();
    }
}

}}


