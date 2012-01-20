#ifndef GDPP_APP_TESTENV_WITHAPPEX_H
#define GDPP_APP_TESTENV_WITHAPPEX_H
#include "gd/app/tests-env/with_app.hpp"
#include "../Application.hpp"

namespace Gd { namespace App { namespace testenv {

class with_app : public gd::app::testenv::with_app {
public:
    with_app();

    Application & t_app_ex(void) { return Application::_cast(t_app()); }
};

}}}

#endif
