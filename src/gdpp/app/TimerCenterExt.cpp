#include <cassert>
#include <stdexcept>
#include "cpe/pal/pal_external.h"
#include "cpe/pal/pal_strings.h"
#include "cpe/utils/hash.h"
#include "cpepp/utils/OpGuard.hpp"
#include "cpepp/cfg/Node.hpp"
#include "cpepp/dr/MetaLib.hpp"
#include "cpepp/utils/RangeMgr.hpp"
#include "cpepp/utils/MemBuffer.hpp"
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
        TimerID _id;
        TimerProcessor * _realResponser;
        TimerProcessor * _useResponser;
        TimerProcessFun _fun;
        cpe_hash_entry _hh_for_responser_to_timer;
    };
 
    TimerCenterImpl(
        Gd::App::Application & app, Cpe::Cfg::Node & cfg)
        : _app(app)
        , _debug(cfg["debug"].dft(0))
        , _alloc(app.allocrator())
        , _tl(NULL)
        , _timer_count_in_page(cfg["timer_count_per_page"].dft(2048))
        , _timer_page_count(0)
        , _timer_page_capacity(0)
        , _timer_buf(NULL)
        , _ids(app.allocrator())
    {
        Cpe::Utils::OpGuard opGuard;

        opGuard.addOp(*this, &TimerCenterImpl::freeTimerBuf);

        initResponserHash();
        opGuard.addOp(*this, &TimerCenterImpl::finiResponserHash);

        _tl = createTl(app, *this);
        opGuard.addOp(*this, &TimerCenterImpl::freeTl);

        opGuard.releaseControl();
    }

    ~TimerCenterImpl() {
        freeTl();
        finiResponserHash();
        freeTimerBuf();
    }

	virtual TimerID registerTimer(
        TimerProcessor& realResponser, TimerProcessor & useResponser, TimerProcessFun fun,
        gd_tl_time_span_t delay, gd_tl_time_span_t span, int repeatCount)
    {
        TimerID newTimerId = allocTimer();
        TimerData * newTimerData = getTimer(newTimerId);
        assert(newTimerData);
        assert(newTimerData->_realResponser == NULL);

        newTimerData->_realResponser = &realResponser;
        newTimerData->_useResponser = &useResponser;
        newTimerData->_fun = fun;

        int r = cpe_hash_table_insert(&_responser_to_timer, newTimerData);
        assert(r == 0);

        return newTimerId;
    }

	virtual void unregisterTimer(TimerProcessor & r) {
        TimerData key;
        key._realResponser = &r;

        TimerData * preNode = (TimerData*)cpe_hash_table_find(&_responser_to_timer, &key);
        while(preNode) {
            TimerData * nextNode = 
                (TimerData *)cpe_hash_table_find_next(&_responser_to_timer, preNode);
            assert(preNode->_realResponser);
            freeTimer(*preNode);
            preNode = nextNode;
        }
    }

	virtual void unregisterTimer(TimerID timerId) {
        TimerData * timerData = getTimer(timerId);
        if (timerData == NULL) {
            APP_CTX_ERROR(_app, "unregister timer %d fail, not valid timer!", timerId);
        }
        else if (timerData->_realResponser == NULL) {
            APP_CTX_ERROR(_app, "unregister timer %d fail, not allocked timer!", timerId);
        }
        else {
            freeTimer(*timerData);
        }
    }

private:
    void freeTl(void) {
        if (_tl) {
            _tl = NULL;
        }
    }

    TimerData * getTimer(TimerID timerId) {
        size_t pagePos = timerId / _timer_count_in_page;
        assert(pagePos < _timer_page_count);

        TimerData * timerPage = _timer_buf[pagePos];

        return &timerPage[timerId % _timer_page_count];
    }

    TimerID allocTimer(void) {
        if (_ids.empty()) {
            if (_timer_page_count + 1 >= _timer_page_capacity) {
                size_t newTimerPageCapacity = _timer_page_count + 128;
                TimerData ** newTimerBuf = (TimerData **)mem_alloc(_alloc, sizeof(TimerData*) * newTimerPageCapacity);
                if (newTimerBuf == NULL) throw ::std::bad_alloc();

                bzero(newTimerBuf, sizeof(TimerData *) * newTimerPageCapacity);
                memcpy(newTimerBuf, _timer_buf, sizeof(TimerData*) * _timer_page_count);

                mem_free(_alloc, _timer_buf);
                _timer_buf = newTimerBuf;
                _timer_page_count = newTimerPageCapacity;

                if (_debug) {
                    APP_CTX_INFO(_app, "resize timer buf to %zd", newTimerPageCapacity);
                }
            }

            ptr_int_t newStart = _timer_page_count * _timer_count_in_page;
            TimerData * newPage =
                (TimerData *)mem_alloc(_alloc, sizeof(TimerData) * _timer_count_in_page);
            if (newPage == NULL) {
                throw ::std::bad_alloc();
            }
            bzero(newPage, sizeof(TimerData) * _timer_count_in_page);
            for(size_t i = 0; i < _timer_count_in_page; ++i) {
                newPage[i]._id = newStart + i;
            }

            _ids.putRange(newStart, newStart + _timer_count_in_page);
            _timer_buf[_timer_page_count] = newPage;
            ++_timer_page_count;

            if (_debug) {
                APP_CTX_INFO(
                    _app,
                    "alloc a new timer page[%d,%d), page count is %zd",
                    (TimerID)newStart, (TimerID)(newStart + _timer_page_count),
                    _timer_page_count);
            }
        }

        return (TimerID)_ids.getOne();
    }

    void freeTimerBuf(void) {
        if (_timer_buf == NULL) return;

        for(size_t i = 0; i < _timer_page_count; ++i) {
            mem_free(_alloc, _timer_buf[i]);
        }

        mem_free(_alloc, _timer_buf);
        _timer_page_count = 0;
        _timer_page_capacity = 0;
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

    void freeTimer(TimerData & data) {
        assert(data._realResponser);
        cpe_hash_table_remove_by_ins(&_responser_to_timer, &data);
        data._realResponser = NULL;
        data._useResponser = NULL;
        data._fun = NULL;
        _ids.putOne(data._id);

        if (_debug) {
            APP_CTX_INFO(_app, "unregister timer %d", data._id);
        }
    }

    
    static  uint32_t cpe_responser_hash_fun(const void * o) {
        const TimerData * timerData = (const TimerData*)o;
        return (((ptr_int_t)timerData->_realResponser) & 0xFFFFFFFF);
    }

    static int cpe_responser_cmp_fun(const void * l, const void * r) {
        const TimerData * lTimerData = (const TimerData*)l;
        const TimerData * rTimerData = (const TimerData*)r;

        return 
            ((ptr_int_t)lTimerData->_realResponser)
            - ((ptr_int_t)rTimerData->_realResponser);
        
    }

    void initResponserHash(void) {
        int r = cpe_hash_table_init(
            &_responser_to_timer,
            _alloc,
            cpe_responser_hash_fun,
            cpe_responser_cmp_fun,
            CPE_HASH_OBJ2ENTRY(TimerData, _hh_for_responser_to_timer),
            _timer_count_in_page);
        if (r != 0) {
            APP_CTX_THROW_EXCEPTION(
                _app,
                ::std::runtime_error,
                "init responser_to_timer fail!");
        }
    }

    void finiResponserHash(void) {
        cpe_hash_table_fini(&_responser_to_timer);
    }

    Gd::App::Application & _app;
    int _debug;
    mem_allocrator_t _alloc;
    gd_tl_t _tl;
    size_t _timer_count_in_page;
    size_t _timer_page_count;
    size_t _timer_page_capacity;
    TimerData ** _timer_buf;
    cpe_hash_table _responser_to_timer;

    Cpe::Utils::RangeMgr _ids;
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

