#include <cassert>
#include <stdexcept>
#include <typeinfo>
#include "gdpp/dp/Request.hpp"
#include "gdpp/app/ReqResponser.hpp"

namespace Gd { namespace App {

ReqResponser::~ReqResponser() {
}

int ReqResponser::_process(gd_dp_req_t req, void * ctx, error_monitor_t em) {
    assert(ctx);
    try {
        return ((ReqResponser *)ctx)->process(*(Dp::Request*)(req), em);
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

static void destoryReqResponser(gd_dp_rsp_t rsp) {
    ((ReqResponser *)gd_dp_rsp_context(rsp))->~ReqResponser();
}

struct gd_dp_rsp_type g_AppReqResponserType = {
    "Gd.App.ReqResponser"
    , destoryReqResponser
};

gd_dp_rsp_type_t ReqResponser::_type = &g_AppReqResponserType;

}}

