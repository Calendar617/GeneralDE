#ifndef USFPP_BPG_RSP_RSPMANAGER_H
#define USFPP_BPG_RSP_RSPMANAGER_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/utils/CString.hpp"
#include "gdpp/app/Application.hpp"
#include "usf/bpg_rsp/bpg_rsp_manage.h"
#include "System.hpp"

namespace Usf { namespace Bpg {

class RspManager : public Cpe::Utils::SimulateObject {
public:
    operator bpg_rsp_manage_t() const { return (bpg_rsp_manage_t)this; }

    Cpe::Utils::CString const & name(void) const { return Cpe::Utils::CString::_cast(bpg_rsp_manage_name(*this)); }

    Gd::App::Application & app(void) { return Gd::App::Application::_cast(bpg_rsp_manage_app(*this)); }
    Gd::App::Application const & app(void) const { return Gd::App::Application::_cast(bpg_rsp_manage_app(*this)); }
    
    static RspManager & _cast(bpg_rsp_manage_t rsp_mgr);
    static RspManager & instance(gd_app_context_t app, const char * name);
};

}}

#endif
