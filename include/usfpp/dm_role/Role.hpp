#ifndef USFPP_DM_ROLE_H
#define USFPP_DM_ROLE_H
#include "cpepp/utils/ClassCategory.hpp"
#include "usf/dm_role/dm_role.h"
#include "System.hpp"

namespace Usf { namespace Dm {

class Role : public Cpe::Utils::SimulateObject {
public:
    operator dm_role_t() const { return (dm_role_t)this; }

    static Role & _cast(dm_role_t role);
    static Role & create(dm_role_manage_t mgr, const void * data, size_t size);
};

}}

#endif
