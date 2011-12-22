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

    int replay(void * buf, size_t size, error_monitor_t em) { 
        return gd_dp_req_replay((gd_dp_req_t)this, buf, size, em);
    }

    int replay(void * buf, size_t size) { 
        return gd_dp_req_replay((gd_dp_req_t)this, buf, size, NULL);
    }

    Request & parent(cpe_hash_string_t type);

    Request * findParent(cpe_hash_string_t type) { 
        return (Request*)gd_dp_req_parent_find((gd_dp_req_t)this, type);
    }

    Request & brother(cpe_hash_string_t type);

    Request * findBrother(cpe_hash_string_t type) { 
        return (Request*)gd_dp_req_brother_find((gd_dp_req_t)this, type);
    }

    Request & child(cpe_hash_string_t type);

    Request * findChild(cpe_hash_string_t type) { 
        return (Request*)gd_dp_req_child_find((gd_dp_req_t)this, type);
    }

    static Request * _cast(gd_dp_req_t req) { return (Request *)req; }

    template<typename T>
    T & as(void) { checkSize(sizeof(T)); return *((T*)gd_dp_req_data((gd_dp_req_t)this)); }

private:
    void checkSize(size_t size);

    Request();
    ~Request();
};

}}

#endif

