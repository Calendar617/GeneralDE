#ifndef GDPP_OM_MANAGE_H
#define GDPP_OM_MANAGE_H
#include "cpepp/utils/ClassCategory.hpp"
#include "gd/om/om_manage.h"
#include "System.hpp"

namespace Gd { namespace Om {

class Manager : public Cpe::Utils::SimulateObject {
public:
    operator gd_om_mgr_t (void) const { return (gd_om_mgr_t)(this); }

    void * alloc(cpe_hash_string_t className);
    void * alloc_nothrow(cpe_hash_string_t className);
    void free(Object * o);

    void registClass(const char * className, size_t object_size, size_t align = sizeof(int));

    void registClass(cpe_hash_string_t className, size_t object_size, size_t align = sizeof(int)) {
        registClass(cpe_hs_data(className), object_size, align);
    }

    template<typename T>
    T * alloc(void) {
        return reinterpret_cast<T*>(this->alloc(T::CLASS_NAME));
    }

    template<typename T>
    void registClass(size_t align = sizeof(int)) {
        this->registClass(T::CLASS_NAME, sizeof(T), align);
    }

    static Manager & _cast(gd_om_mgr_t omm);
};

}}

#endif
