#ifndef GDPP_DP_REQUEST_H
#define GDPP_DP_REQUEST_H
#include "cpepp/utils/ClassCategory.hpp"
#include "gd/dp/dp_request.h"
#include "System.hpp"

namespace Gd { namespace Dp {

class Request : public Cpe::Utils::SimulateObject {
public:
    operator gd_dp_req_t () const { return (gd_dp_req_t)this; }

    void * data(void) { return gd_dp_req_data((gd_dp_req_t)this); }
    size_t size(void) { return gd_dp_req_size((gd_dp_req_t)this); }
    mem_allocrator_t talloc(void) { return gd_dp_req_talloc((gd_dp_req_t)this); }

    void setParent(gd_dp_req_t parent) {
        gd_dp_req_set_parent(*this, parent);
    }

    int reply(void * buf, size_t size, error_monitor_t em) { 
        return gd_dp_req_reply((gd_dp_req_t)this, buf, size, em);
    }

    int reply(void * buf, size_t size) { 
        return gd_dp_req_reply((gd_dp_req_t)this, buf, size, NULL);
    }

    Request * parent(void) {
        return (Request*)(gd_dp_req_parent(*this));
    }
    Request const * parent(void) const {
        return (Request*)(gd_dp_req_parent(*this));
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

    static Request * _create(gd_dp_mgr_t mgr, cpe_hash_string_t type, size_t capacity) {
        return (Request*)gd_dp_req_create(mgr, type, capacity);
    }

    static Request * _create(gd_dp_mgr_t mgr, cpe_hash_string_t type, char * buf, size_t capacity) {
        return (Request*)gd_dp_req_create_with_buf(mgr, type, buf, capacity);
    }

    template<typename T>
    T & as(void) { checkSize(sizeof(T)); return *((T*)gd_dp_req_data((gd_dp_req_t)this)); }

private:
    void checkSize(size_t size);
};

}}

#endif

