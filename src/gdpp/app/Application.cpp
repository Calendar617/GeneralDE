#include <stdexcept>
#include "gdpp/app/Application.hpp"

namespace Gd { namespace App {

Application &
Application::instance(void) {
    if (g_app_context == NULL) {
        throw ::std::runtime_error("Application have not been created!");
    }

    return *(Application*)g_app_context;
}

}}
