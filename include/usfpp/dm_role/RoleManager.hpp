#ifndef USFPP_DM_ROLEMANAGER_H
#define USFPP_DM_ROLEMANAGER_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/utils/CString.hpp"
#include "cpepp/dr/Meta.hpp"
#include "gdpp/app/Application.hpp"
#include "usf/dm_role/dm_role_manage.h"
#include "usf/dm_role/dm_role.h"
#include "System.hpp"

namespace Usf { namespace Dm {

class RoleManager : public Cpe::Utils::SimulateObject {
public:
    operator dm_role_manage_t() const { return (dm_role_manage_t)this; }

    Cpe::Utils::CString const & name(void) const { return Cpe::Utils::CString::_cast(dm_role_manage_name(*this)); }

    Gd::App::Application & app(void) { return Gd::App::Application::_cast(dm_role_manage_app(*this)); }
    Gd::App::Application const & app(void) const { return Gd::App::Application::_cast(dm_role_manage_app(*this)); }

    Cpe::Dr::Meta const & roleMeta(void) const { return Cpe::Dr::Meta::_cast(dm_role_manage_meta(*this)); }

    /*by id*/
    Role const * findRole(dm_role_id_t id) const { return (Role*)dm_role_find_by_id(*this, id); }
    Role * findRole(dm_role_id_t id) { return (Role*)dm_role_find_by_id(*this, id); }
    Role & role(dm_role_id_t id);
    Role const & role(dm_role_id_t) const;

    /*by index int8*/
    Role * findRole(const char * index, int8_t key) { return (Role*)dm_role_find_by_index_int8(*this, index, key); }
    Role const * findRole(const char * index, int8_t key) const { return (Role const *)dm_role_find_by_index_int8(*this, index, key); }
    Role & role(const char * index, int8_t key);
    Role const & role(const char * index, int8_t key) const;

    /*by index uint8*/
    Role * findRole(const char * index, uint8_t key) { return (Role*)dm_role_find_by_index_uint8(*this, index, key); }
    Role const * findRole(const char * index, uint8_t key) const { return (Role const *)dm_role_find_by_index_uint8(*this, index, key); }
    Role & role(const char * index, uint8_t key);
    Role const & role(const char * index, uint8_t key) const;

    /*by index int16*/
    Role * findRole(const char * index, int16_t key) { return (Role*)dm_role_find_by_index_int16(*this, index, key); }
    Role const * findRole(const char * index, int16_t key) const { return (Role const *)dm_role_find_by_index_int16(*this, index, key); }
    Role & role(const char * index, int16_t key);
    Role const & role(const char * index, int16_t key) const;

    /*by index uint16*/
    Role * findRole(const char * index, uint16_t key) { return (Role*)dm_role_find_by_index_uint16(*this, index, key); }
    Role const * findRole(const char * index, uint16_t key) const { return (Role const *)dm_role_find_by_index_uint16(*this, index, key); }
    Role & role(const char * index, uint16_t key);
    Role const & role(const char * index, uint16_t key) const;

    /*by index int32*/
    Role * findRole(const char * index, int32_t key) { return (Role*)dm_role_find_by_index_int32(*this, index, key); }
    Role const * findRole(const char * index, int32_t key) const { return (Role const *)dm_role_find_by_index_int32(*this, index, key); }
    Role & role(const char * index, int32_t key);
    Role const & role(const char * index, int32_t key) const;

    /*by index uint32*/
    Role * findRole(const char * index, uint32_t key) { return (Role*)dm_role_find_by_index_uint32(*this, index, key); }
    Role const * findRole(const char * index, uint32_t key) const { return (Role const *)dm_role_find_by_index_uint32(*this, index, key); }
    Role & role(const char * index, uint32_t key);
    Role const & role(const char * index, uint32_t key) const;

    /*by index int64*/
    Role * findRole(const char * index, int64_t key) { return (Role*)dm_role_find_by_index_int64(*this, index, key); }
    Role const * findRole(const char * index, int64_t key) const { return (Role const *)dm_role_find_by_index_int64(*this, index, key); }
    Role & role(const char * index, int64_t key);
    Role const & role(const char * index, int64_t key) const;

    /*by index uint64*/
    Role * findRole(const char * index, uint64_t key) { return (Role*)dm_role_find_by_index_uint64(*this, index, key); }
    Role const * findRole(const char * index, uint64_t key) const { return (Role const *)dm_role_find_by_index_uint64(*this, index, key); }
    Role & role(const char * index, uint64_t key);
    Role const & role(const char * index, uint64_t key) const;

    /*by index float*/
    Role * findRole(const char * index, float key) { return (Role*)dm_role_find_by_index_float(*this, index, key); }
    Role const * findRole(const char * index, float key) const { return (Role const *)dm_role_find_by_index_float(*this, index, key); }
    Role & role(const char * index, float key);
    Role const & role(const char * index, float key) const;

    /*by index double*/
    Role * findRole(const char * index, double key) { return (Role*)dm_role_find_by_index_double(*this, index, key); }
    Role const * findRole(const char * index, double key) const { return (Role const *)dm_role_find_by_index_double(*this, index, key); }
    Role & role(const char * index, double key);
    Role const & role(const char * index, double key) const;

    /*by index string*/
    Role * findRole(const char * index, const char * key) { return (Role*)dm_role_find_by_index_string(*this, index, key); }
    Role const * findRole(const char * index, const char * key) const { return (Role const *)dm_role_find_by_index_string(*this, index, key); }
    Role & role(const char * index, const char * key);
    Role const & role(const char * index, const char * key) const;

    static RoleManager & _cast(dm_role_manage_t role_manage);
    static RoleManager & instance(gd_app_context_t app, const char * name = NULL);
};

}}

#endif
