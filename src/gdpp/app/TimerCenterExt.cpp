#include <cassert>
#include <stdexcept>
#include "cpe/pal/pal_external.h"
#include "cpepp/utils/OpGuard.hpp"
#include "cpepp/dr/MetaLib.hpp"
#include "gdpp/nm/Manager.hpp"
#include "gd/tl/tl_action.h"
#include "gdpp/tl/Manager.hpp"
#include "gdpp/app/Log.hpp"
#include "gdpp/app/Application.hpp"
#include "TimerCenterExt.hpp"

namespace Gd { namespace App {

class TimerCenterImpl : public TimerCenterExt {
public:
    struct TimerData {
        TimerProcessor * _realResponser;
        TimerProcessor * _useResponser;
        TimerProcessor _fun;
    };
 
    TimerCenterImpl(
        Gd::App::Application & app, Cpe::Cfg::Node & cfg)
        : _app(app)
        , _tl(NULL)
    {
        Cpe::Utils::OpGuard opGuard;

        _tl = createTl(app, *this);
        opGuard.addOp(*this, &TimerCenterImpl::freeTl);

        opGuard.releaseControl();
    }

    ~TimerCenterImpl() {
        freeTl();
    }

	virtual TimerID registerTimer(
        TimerProcessor& realResponser, TimerProcessor & useResponser, TimerProcessFun fun,
        gd_tl_time_span_t delay, gd_tl_time_span_t span, int repeatCount)
    {
        return 0;
    }

	virtual void unregisterTimer(TimerProcessor & r) {
    }

	virtual void unregisterTimer(TimerID timerId) {
    }

private:
    void freeTl(void) {
        if (_tl) {
            _tl = NULL;
        }
    }

    static void dispatch_timer(gd_tl_event_t input, void * context) {
        TimerCenterImpl * ec = (TimerCenterImpl*)context;
        TimerID timerId = *reinterpret_cast<TimerID*>(gd_tl_event_data(input));

        try {

        }
        catch(::std::exception const & e) {
            APP_CTX_ERROR(
                ec->_app, "dispatch timer(id=%d): catch exception: %s!",
                timerId, e.what());
        }
        catch(...) {
            APP_CTX_ERROR(
                ec->_app, "dispatch timer(id=%d): catch unknown exception!",
                timerId);
        }
    }

    static gd_tl_t createTl(Gd::App::Application & app, TimerCenterImpl & self) {
        gd_tl_t tl = gd_tl_create(app.tlManager());
        if (tl == NULL) {
            APP_CTX_THROW_EXCEPTION(app, ::std::runtime_error, "create tl fail!");
        }

        gd_tl_set_opt(tl, gd_tl_set_event_dispatcher, dispatch_timer);
        gd_tl_set_opt(tl, gd_tl_set_event_op_context, &self);

        return tl;
    }

    Gd::App::Application & _app;
    gd_tl_t _tl;
};

TimerCenter::~TimerCenter() {
}

TimerCenterExt::~TimerCenterExt() {
}

TimerCenter & TimerCenter::instance(Application & app, cpe_hash_string_t name) {
    TimerCenter * r =
        dynamic_cast<TimerCenter *>(
            &app.nmManager().object(name));
    if (r == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app,
            ::std::runtime_error, "TimerCenter cast fail!");
    }

    return *r;
}

static cpe_hash_string_buf s_Event_req_type_name_buf = CPE_HS_BUF_MAKE("app.event.req");
cpe_hash_string_t TimerCenter::req_type_name = (cpe_hash_string_t)s_Event_req_type_name_buf;

static cpe_hash_string_buf s_Event_DEFAULT_NAME_buf = CPE_HS_BUF_MAKE("app.event.req");
cpe_hash_string_t TimerCenter::DEFAULT_NAME = (cpe_hash_string_t)s_Event_DEFAULT_NAME_buf;

}}

extern "C"
EXPORT_DIRECTIVE
int AppTimerCenter_app_init(Gd::App::Application & app, Gd::App::Module & module, Cpe::Cfg::Node & cfg) {
    try {
        return (new (app.nmManager(), cpe_hs_data(Gd::App::TimerCenter::DEFAULT_NAME))
                Gd::App::TimerCenterImpl(app, cfg))
            == NULL
            ? -1
            : 0;
    }
    catch(::std::exception const & e) {
        APP_CTX_ERROR(app, "create AppTimerCenter: catch exception %s!", e.what());
        return -1;
    }
    catch(...) {
        APP_CTX_ERROR(app, "create AppTimerCenter: catch unknown exception!");
        return -1;
    }
}

extern "C"
EXPORT_DIRECTIVE
void AppTimerCenter_app_fini(Gd::App::Application & app, Gd::App::Module & module) {
    app.nmManager().removeObject(Gd::App::TimerCenter::DEFAULT_NAME);
}

