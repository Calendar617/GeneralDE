#include <cassert>
#include <sstream>
#include <stdexcept>
#include "cpepp/utils/ErrorCollector.hpp"
#include "gd/om/om_object.h"
#include "gdpp/om/Manager.hpp"

namespace Gd { namespace Om {

void * Manager::alloc(cpe_hash_string_t className) {
    gd_om_oid_t oid = gd_om_obj_alloc(*this, className, NULL);
    if (oid == GD_OM_INVALID_OID) {
        throw ::std::bad_alloc();
    }

    void * p = gd_om_obj_get(*this, oid, NULL);
    if (p == NULL) {
        throw ::std::runtime_error("OmManager: get buf from oid fail!");
    }

    return p;
}

void * Manager::alloc_nothrow(cpe_hash_string_t className) {
    gd_om_oid_t oid = gd_om_obj_alloc(*this, className, NULL);
    if (oid == GD_OM_INVALID_OID) {
        return NULL;
    }

    return gd_om_obj_get(*this, oid, NULL);
}

void Manager::free(Object * p) {
    gd_om_oid_t oid = gd_om_obj_id_from_addr(*this, (void*)p, NULL);
    assert(oid != GD_OM_INVALID_OID);
    gd_om_obj_free(*this, oid, NULL);
}

void Manager::registClass(const char * className, size_t object_size, size_t align) {
    Cpe::Utils::ErrorCollector ec;
    gd_om_mgr_add_class(*this, className, object_size, align, ec);

    ec.checkThrowWithMsg< ::std::runtime_error>();
}

Manager & Manager::_cast(gd_om_mgr_t omm) {
    if (omm == NULL) {
        throw ::std::runtime_error("omm is NULL!"); 
    }

    return *reinterpret_cast<Manager*>(omm);
}

}}
