#ifndef GD_APP_TESTENV_WITHAPP_H
#define GD_APP_TESTENV_WITHAPP_H
#include "cpe/utils/tests-env/test-env.hpp"
#include "../app.h"

namespace gd { namespace app { namespace testenv {

class with_app : public ::testenv::env<> {
public:
    with_app();

    void SetUp();
    void TearDown();

    gd_app_context_t t_app(void);

    void t_app_create(size_t capacity, int argc, char ** argv);

    gd_app_module_t
    t_app_install_module(
        const char * name,
        const char * libName,
        const char * cfg);

    gd_app_module_t
    t_app_install_module(
        const char * name,
        const char * cfg);

private:
    gd_app_context_t m_app;
};

}}}

#endif
