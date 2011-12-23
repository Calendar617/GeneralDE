#ifndef GDPP_NM_MANAGE_H
#define GDPP_NM_MANAGE_H
#include "gd/nm/nm_manage.h"
#include "System.hpp"
#include "ObjectIterator.hpp"

namespace Gd { namespace Nm {

class Manager {
public:
    operator gd_nm_mgr_t (void) const { return (gd_nm_mgr_t)(this); }

    ObjectIterator objects(void) {
        ObjectIterator it;
        gd_nm_mgr_nodes(&it.m_it, *this);
        return it;
    }

    Object const * findObject(cpe_hash_string_t name) const {
        return (Object *)gd_nm_mgr_find_node(*this, name);
    }

    Object * findObject(cpe_hash_string_t name) {
        return (Object *)gd_nm_mgr_find_node(*this, name);
    }

    Object const & object(cpe_hash_string_t name) const;
    Object & object(cpe_hash_string_t name);

    void removeObject(cpe_hash_string_t name);

private:
    ~Manager();
    Manager();
};

}}

#endif
