#ifndef GDPP_UTILS_IDGENERATOR_H
#define gdpp_UTILS_IDGENERATOR_H
#include "cpepp/utils/ClassCategory.hpp"
#include "gdpp/app/Application.hpp"
#include "gd/utils/id_generator.h"

namespace Gd { namespace Utils {

class IdGenerator : public Cpe::Utils::SimulateObject  {
public:
    operator gd_id_generator_t () const { return (gd_id_generator_t)this; }

    gd_id_t generateId(void);
    void setNextId(gd_id_t id);

    Gd::App::Application & app(void) { return Gd::App::Application::_cast(gd_id_generator_app(*this)); }
    Gd::App::Application const & app(void) const { return Gd::App::Application::_cast(gd_id_generator_app(*this)); }

    const char * name(void) const { return gd_id_generator_name(*this); }
    cpe_hash_string_t name_hs(void) const { return gd_id_generator_name_hs(*this); }

    static IdGenerator & instance(gd_app_context_t app, cpe_hash_string_t name);
    static IdGenerator & instance(gd_app_context_t app, const char * name);
};

}}

#endif
