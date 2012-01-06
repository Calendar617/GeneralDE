#ifndef GDPP_DP_REQUEST_H
#define GDPP_DP_REQUEST_H
#include "cpepp/utils/ClassCategory.hpp"
#include "gd/dp/dp_request.h"
#include "System.hpp"

namespace Gd { namespace Dp {

class Request : public Cpe::Utils::SimulateObject {
public:
    operator gd_dp_req_t () const { return (gd_dp_req_t)this; }

    void * data(void) { return gd_dp_req_data(*this); }
    void const * data(void) const { return gd_dp_req_data(*this); }

    size_t size(void) const { return gd_dp_req_size(*this); }
    void setSize(size_t size) { gd_dp_req_set_size(*this, size); }

    size_t capacity(void) const { return gd_dp_req_capacity(*this); }

    const char * type(void) const { return gd_dp_req_type(*this); }
    cpe_hash_string_t type_hs(void) const { return gd_dp_req_type_hs(*this); }

    mem_allocrator_t talloc(void) { return gd_dp_req_talloc(*this); }

    gd_dp_mgr_t mgr(void) {
        return gd_dp_req_mgr( *this );
    }

    void setParent(gd_dp_req_t parent) {
        gd_dp_req_set_parent(*this, parent);
    }

    int reply(void * buf, size_t size, error_monitor_t em) { 
        return gd_dp_req_reply(*this, buf, size, em);
    }

    int reply(void * buf, size_t size) { 
        return gd_dp_req_reply(*this, buf, size, NULL);
    }

    Request * parent(void) {
        return (Request*)(gd_dp_req_parent(*this));
    }
    Request const * parent(void) const {
        return (Request*)(gd_dp_req_parent(*this));
    }

    Request & parent(cpe_hash_string_t type);
    Request const & parent(cpe_hash_string_t type) const;

    Request * findParent(cpe_hash_string_t type) { 
        return (Request*)gd_dp_req_parent_find(*this, type);
    }
    Request const * findParent(cpe_hash_string_t type) const { 
        return (Request*)gd_dp_req_parent_find(*this, type);
    }

    Request & brother(cpe_hash_string_t type);
    Request const & brother(cpe_hash_string_t type) const;

    Request * findBrother(cpe_hash_string_t type) { 
        return (Request*)gd_dp_req_brother_find(*this, type);
    }
    Request const * findBrother(cpe_hash_string_t type) const { 
        return (Request*)gd_dp_req_brother_find(*this, type);
    }

    Request & child(cpe_hash_string_t type);
    Request const & child(cpe_hash_string_t type) const;

    Request * findChild(cpe_hash_string_t type) { 
        return (Request*)gd_dp_req_child_find(*this, type);
    }
    Request const * findChild(cpe_hash_string_t type) const { 
        return (Request*)gd_dp_req_child_find(*this, type);
    }

    static Request * _create(gd_dp_mgr_t mgr, cpe_hash_string_t type, size_t capacity) {
        return (Request*)gd_dp_req_create(mgr, type, capacity);
    }

    static Request * _create(gd_dp_mgr_t mgr, cpe_hash_string_t type, char * buf, size_t capacity) {
        return (Request*)gd_dp_req_create_with_buf(mgr, type, buf, capacity);
    }

    static Request & _cast(gd_dp_req_t req);

    static void _free(Request * req) { if (req) { gd_dp_req_free(*req); } }

    template<typename T>
    T & as(void) { checkSize(sizeof(T)); return *((T*)gd_dp_req_data(*this)); }

    template<typename T>
    T const & as(void) const { checkSize(sizeof(T)); return *((T const*)gd_dp_req_data(*this)); }

private:
    void checkSize(size_t size) const;
};

}}

#endif

