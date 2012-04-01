#ifndef USFPP_DM_ROLEMANAGERGEN_H
#define USFPP_DM_ROLEMANAGERGEN_H
#include "RoleManager.hpp"

namespace Usf { namespace Dm {

template<typename ManagerT, typename RoleT>
class RoleManagerGen : public RoleManager {
public:
    /*by id*/
    RoleT const * findRole(dm_role_id_t id) const { return static_cast<RoleT const *>(RoleManager::findRole(*this, id)); }
    RoleT * findRole(dm_role_id_t id) { return static_cast<RoleT *>(RoleManager::findRole(*this, id)); }
    Role & role(dm_role_id_t id) { return static_cast<RoleT &>(RoleManager::role(*this, id)); }
    Role const & role(dm_role_id_t id) const { return static_cast<RoleT const &>(RoleManager::role(*this, id)); }

    /*by index int8*/
    RoleT * findRole(const char * index, int8_t key) { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT const * findRole(const char * index, int8_t key) const { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT & role(const char * index, int8_t key) { return static_cast<RoleT &>(RoleManager::role(index, key)); }
    RoleT const & role(const char * index, int8_t key) const { return static_cast<RoleT &>(RoleManager::role(index, key)); }

    /*by index uint8*/
    RoleT * findRole(const char * index, uint8_t key) { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT const * findRole(const char * index, uint8_t key) const { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT & role(const char * index, uint8_t key) { return static_cast<RoleT &>(RoleManager::role(index, key)); }
    RoleT const & role(const char * index, uint8_t key) const { return static_cast<RoleT &>(RoleManager::role(index, key)); }

    /*by index int16*/
    RoleT * findRole(const char * index, int16_t key) { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT const * findRole(const char * index, int16_t key) const { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT & role(const char * index, int16_t key) { return static_cast<RoleT &>(RoleManager::role(index, key)); }
    RoleT const & role(const char * index, int16_t key) const { return static_cast<RoleT &>(RoleManager::role(index, key)); }

    /*by index uint16*/
    RoleT * findRole(const char * index, uint16_t key) { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT const * findRole(const char * index, uint16_t key) const { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT & role(const char * index, uint16_t key) { return static_cast<RoleT &>(RoleManager::role(index, key)); }
    RoleT const & role(const char * index, uint16_t key) const { return static_cast<RoleT &>(RoleManager::role(index, key)); }

    /*by index int32*/
    RoleT * findRole(const char * index, int32_t key) { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT const * findRole(const char * index, int32_t key) const { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT & role(const char * index, int32_t key) { return static_cast<RoleT &>(RoleManager::role(index, key)); }
    RoleT const & role(const char * index, int32_t key) const { return static_cast<RoleT &>(RoleManager::role(index, key)); }

    /*by index uint32*/
    RoleT * findRole(const char * index, uint32_t key) { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT const * findRole(const char * index, uint32_t key) const { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT & role(const char * index, uint32_t key) { return static_cast<RoleT &>(RoleManager::role(index, key)); }
    RoleT const & role(const char * index, uint32_t key) const { return static_cast<RoleT &>(RoleManager::role(index, key)); }

    /*by index int64*/
    RoleT * findRole(const char * index, int64_t key) { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT const * findRole(const char * index, int64_t key) const { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT & role(const char * index, int64_t key) { return static_cast<RoleT &>(RoleManager::role(index, key)); }
    RoleT const & role(const char * index, int64_t key) const { return static_cast<RoleT &>(RoleManager::role(index, key)); }

    /*by index uint64*/
    RoleT * findRole(const char * index, uint64_t key) { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT const * findRole(const char * index, uint64_t key) const { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT & role(const char * index, uint64_t key) { return static_cast<RoleT &>(RoleManager::role(index, key)); }
    RoleT const & role(const char * index, uint64_t key) const { return static_cast<RoleT &>(RoleManager::role(index, key)); }

    /*by index double*/
    RoleT * findRole(const char * index, double key) { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT const * findRole(const char * index, double key) const { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT & role(const char * index, double key) { return static_cast<RoleT &>(RoleManager::role(index, key)); }
    RoleT const & role(const char * index, double key) const { return static_cast<RoleT &>(RoleManager::role(index, key)); }

    /*by index float*/
    RoleT * findRole(const char * index, float key) { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT const * findRole(const char * index, float key) const { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT & role(const char * index, float key) { return static_cast<RoleT &>(RoleManager::role(index, key)); }
    RoleT const & role(const char * index, float key) const { return static_cast<RoleT &>(RoleManager::role(index, key)); }

    /*by index string*/
    RoleT * findRole(const char * index, const char * key) { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT const * findRole(const char * index, const char * key) const { return static_cast<RoleT *>(RoleManager::findRole(index, key)); }
    RoleT & role(const char * index, const char * key) { return static_cast<RoleT &>(RoleManager::role(index, key)); }
    RoleT const & role(const char * index, const char * key) const { return static_cast<RoleT &>(RoleManager::role(index, key)); }

    static ManagerT & _cast(dm_role_manage_t role_manage) { 
        return static_cast<ManagerT &>(RoleManager::_cast(role_manage));
    }

    static ManagerT & instance(gd_app_context_t app, const char * name = NULL) {
        return static_cast<ManagerT &>(RoleManager::instance(app, name));
    }
};

}}

#endif
