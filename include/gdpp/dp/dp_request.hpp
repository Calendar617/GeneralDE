#ifndef GDPP_DP_REQUEST_H
#define GDPP_DP_REQUEST_H
#include "gd/dp/dp_request.h"
#include "dp_system.hpp"

namespace Gd { namespace Dp {

class Request {
public:
    void * data(void) { return gd_dp_req_data((gd_dp_req_t)this); }
    size_t size(void) { return gd_dp_req_size((gd_dp_req_t)this); }
    mem_allocrator_t talloc(void) { return gd_dp_req_talloc((gd_dp_req_t)this); }

    static Request * _cast(gd_dp_req_t req) { return (Request *)req; }
private:
    Request();
    ~Request();
};

}}

#endif

