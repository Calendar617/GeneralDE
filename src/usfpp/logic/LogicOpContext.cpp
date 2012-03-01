#include <vector>
#include "cpe/cfg/cfg_manage.h"
#include "cpepp/dr/Meta.hpp"
#include "cpepp/dr/MetaLib.hpp"
#include "gdpp/app/Log.hpp"
#include "gdpp/app/Random.hpp"
#include "usfpp/logic/LogicOpContext.hpp"

namespace Usf { namespace Logic {

LogicOpData & LogicOpContext::data(const char * name) {
    LogicOpData * r = findData(name);
    if (r == 0) {
        APP_CTX_THROW_EXCEPTION(
            app(),
            ::std::runtime_error,
            "data %s not exist in context!",
            name);
    }
    return *r;
}

LogicOpData const & LogicOpContext::data(const char * name) const {
    LogicOpData const * r = findData(name);
    if (r == 0) {
        APP_CTX_THROW_EXCEPTION(
            app(),
            ::std::runtime_error,
            "data %s not exist in context!",
            name);
    }
    return *r;
}

LogicOpData &
LogicOpContext::checkCreateData(LPDRMETA meta, size_t capacity) {
    logic_data_t data = logic_data_get_or_create(*this, meta, capacity);
    if (data == 0) {
        APP_CTX_THROW_EXCEPTION(
            app(),
            ::std::runtime_error,
            "data %s create in context fail!",
            dr_meta_name(meta));
    }
    return *(LogicOpData*)data;
}

Cpe::Utils::Random &
LogicOpContext::random(cpe_hash_string_t name) {
    return Gd::App::Random::instance(
        app(),
        name == 0 ? Gd::App::Random::DEFAULT_NAME : name);
}

}}
