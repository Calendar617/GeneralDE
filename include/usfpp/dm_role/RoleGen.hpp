#ifndef USFPP_DM_ROLEGEN_H
#define USFPP_DM_ROLEGEN_H
#include "Role.hpp"

namespace Usf { namespace Dm {

template<typename RoleT, typename DataT>
class RoleGen : public Role {
public:
    typedef DataT data_type;

    static RoleT & _cast(dm_role_t role) { 
        return static_cast<RoleT &>(Role::_cast(role));
    }

    static RoleT & create(dm_role_manage_t mgr, DataT const & data) {
        return static_cast<RoleT &>(Role::create(mgr, &data, sizeof(data)));
    }
};

}}

#endif
