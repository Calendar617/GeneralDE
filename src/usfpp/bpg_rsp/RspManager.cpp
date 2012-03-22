#include "gdpp/app/Log.hpp"
#include "usfpp/bpg_rsp/RspManager.hpp"

namespace Usf { namespace Bpg {

RspManager & RspManager::_cast(bpg_rsp_manage_t rsp_manage) {
    if (rsp_manage == NULL) {
        throw ::std::runtime_error("Usf::Bpg::RspManager::_cast: input rsp_manage is NULL!");
    }

    return *(RspManager*)rsp_manage;
}

RspManager & RspManager::instance(gd_app_context_t app, const char * name) {
    bpg_rsp_manage_t rsp_manage = bpg_rsp_manage_find_nc(app, name);
    if (rsp_manage == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app,
            ::std::runtime_error,
            "bpg_rsp_manage %s not exist!", name ? name : "default");
    }

    return *(RspManager*)rsp_manage;
}

}}
