#ifndef GDPP_NM_MANAGE_H
#define GDPP_NM_MANAGE_H
#include "gd/nm/nm_manage.h"
#include "System.hpp"
#include "ObjectIterator.hpp"

namespace Gd { namespace Nm {

class Manager {
public:
    operator gd_nm_mgr_t (void) const { return (gd_nm_mgr_t)(this); }

    ObjectIterator objects(void);
    ConstObjectIterator objects(void) const;

    Object const * findObject(cpe_hash_string_t name) const;
    Object * findObject(cpe_hash_string_t name);
    Object const & object(cpe_hash_string_t name) const;
    Object & object(cpe_hash_string_t name);

    void removeObject(cpe_hash_string_t name);

private:
    ~Manager();
    Manager();
};

}}

#endif
