#include <cassert>
#include "gdpp/dp/dp_request.hpp"
#include "gdpp/app/app_responser.hpp"

namespace Gd { namespace App {

Responser::~Responser() {
}

int Responser::_process(gd_dp_req_t req, void * ctx, error_monitor_t em) {
    assert(ctx);
    return ((Responser *)ctx)->process(*Dp::Request::_cast(req), em);
}

static void destoryResponser(gd_dp_rsp_t rsp) {
    delete ((Responser *)gd_dp_rsp_context(rsp));
}

struct gd_dp_rsp_type g_AppResponserType = {
    "Gd.App.Responser"
    , destoryResponser
};

gd_dp_rsp_type_t Responser::_type = &g_AppResponserType;

}}

