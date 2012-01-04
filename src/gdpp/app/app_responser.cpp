#include <cassert>
#include <stdexcept>
#include <typeinfo>
#include "gdpp/dp/Request.hpp"
#include "gdpp/app/Responser.hpp"

namespace Gd { namespace App {

Responser::~Responser() {
}

int Responser::_process(gd_dp_req_t req, void * ctx, error_monitor_t em) {
    assert(ctx);
    try {
        return ((Responser *)ctx)->process(*(Dp::Request*)(req), em);
    }
    catch(::std::exception const & e) {
        CPE_ERROR(
            em,
            "process request: catch exception(%s): %s",
            typeid(e).name(), e.what());
    }
    catch(...) {
        CPE_ERROR(em, "process request: catch unknown error!");
    }
    return -1;
}

static void destoryResponser(gd_dp_rsp_t rsp) {
    ((Responser *)gd_dp_rsp_context(rsp))->~Responser();
}

struct gd_dp_rsp_type g_AppResponserType = {
    "Gd.App.Responser"
    , destoryResponser
};

gd_dp_rsp_type_t Responser::_type = &g_AppResponserType;

}}

