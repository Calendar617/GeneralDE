#include <cassert>
#include <sstream>
#include <stdexcept>
#include "cpepp/utils/ErrorCollector.hpp"
#include "gdpp/om/Manager.hpp"

namespace Gd { namespace Om {

void * Manager::alloc(cpe_hash_string_t className) {
    return NULL;
}

void * Manager::alloc_nothrow(cpe_hash_string_t className) {
    return NULL;
}

void Manager::free(Object * p) {
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
