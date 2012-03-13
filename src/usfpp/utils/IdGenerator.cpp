#include "gd/app/app_context.h"
#include "gdpp/app/Log.hpp"
#include "usfpp/utils/IdGenerator.hpp"

namespace Usf { namespace Utils {

usf_id_t IdGenerator::generate(void) {
    usf_id_t r;
    if (usf_id_generator_generate(&r, *this) != 0) {
        APP_CTX_THROW_EXCEPTION(
            app(),
            ::std::runtime_error,
            "IdGeneratro %s generate id fail!", name());
    }

    return r;
}

IdGenerator & IdGenerator::instance(gd_app_context_t app, cpe_hash_string_t name) {
    usf_id_generator_t r = usf_id_generator_find(app, name);
    if (r == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app,
            ::std::runtime_error,
            "IdGeneratro %s not exist!", cpe_hs_data(name));
    }

    return *(IdGenerator*)r;
}

IdGenerator & IdGenerator::instance(gd_app_context_t app, const char * name) {
    usf_id_generator_t r = usf_id_generator_find_nc(app, name);
    if (r == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app,
            ::std::runtime_error,
            "IdGeneratro %s not exist!", name);
    }

    return *(IdGenerator*)r;
}

}}
