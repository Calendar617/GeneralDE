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
    enum TimerDataState {
        TimerDataState_NotInResponserHash = 0
        , TimerDataState_InResponserHash
    };

    struct TimerData {
        TimerID _id;
        TimerDataState _state;
        gd_tl_event_t _tl_event;
        TimerProcessor * _realProcessor;
        TimerProcessor * _useProcessor;
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

        initProcessorHash();
        opGuard.addOp(*this, &TimerCenterImpl::finiProcessorHash);


        _tl = createTl(app, *this);
        opGuard.addOp(*this, &TimerCenterImpl::freeTl);

        opGuard.releaseControl();
    }

    ~TimerCenterImpl() {
        freeTl();
        finiProcessorHash();
        freeTimerBuf();
    }

	virtual TimerID registerTimer(
        TimerProcessor& realProcessor, TimerProcessor & useProcessor, TimerProcessFun fun,
        gd_tl_time_span_t delay, gd_tl_time_span_t span, int repeatCount)
    {
        TimerID newTimerId = allocTimer();
        TimerData * newTimerData = getTimer(newTimerId);
        assert(newTimerData);
        assert(newTimerData->_realProcessor == NULL);
        assert(newTimerData->_tl_event == NULL);
        assert(newTimerData->_state == TimerDataState_NotInResponserHash);

        newTimerData->_realProcessor = &realProcessor;
        newTimerData->_useProcessor = &useProcessor;
        newTimerData->_fun = fun;
        newTimerData->_tl_event = gd_tl_event_create(_tl, sizeof(TimerID));
        if (newTimerData->_tl_event == NULL) {
            clearTimerData(*newTimerData);
            freeTimerId(newTimerId);

            APP_CTX_THROW_EXCEPTION(
                _app,
                ::std::runtime_error,
                "registerTimer: create tl event fail!");
        }
        *(TimerID*)gd_tl_event_data(newTimerData->_tl_event) = newTimerId;

        int r = cpe_hash_table_insert(&_responser_to_timer, newTimerData);
        if (r != 0) {
            clearTimerData(*newTimerData);
            freeTimerId(newTimerId);
            APP_CTX_THROW_EXCEPTION(
                _app,
                ::std::runtime_error,
                "registerTimer: insert to responser timer list fail!");
        }
        newTimerData->_state = TimerDataState_InResponserHash;

        r = gd_tl_event_send_ex(newTimerData->_tl_event, delay, span, repeatCount);
        if (r != 0) {
            clearTimerData(*newTimerData);
            freeTimerId(newTimerId);
            APP_CTX_THROW_EXCEPTION(
                _app,
                ::std::runtime_error,
                "registerTimer: register to tl fail!");
        }

        return newTimerId;
    }

	virtual void unregisterTimer(TimerProcessor & r) {
        TimerData key;
        key._realProcessor = &r;

        TimerData * preNode = (TimerData*)cpe_hash_table_find(&_responser_to_timer, &key);
        while(preNode) {
            TimerData * nextNode = 
                (TimerData *)cpe_hash_table_find_next(&_responser_to_timer, preNode);
            assert(preNode->_realProcessor);
            freeTimer(*preNode);
            preNode = nextNode;
        }
    }

	virtual void unregisterTimer(TimerID timerId) {
        TimerData * timerData = getTimer(timerId);
        if (timerData == NULL) {
            APP_CTX_ERROR(_app, "unregister timer %d fail, not valid timer!", timerId);
        }
        else if (timerData->_realProcessor == NULL) {
            APP_CTX_ERROR(_app, "unregister timer %d fail, not allocked timer!", timerId);
        }
        else {
            freeTimer(*timerData);
        }
    }

    virtual bool haveTimer(TimerID timerId) const {
        int pagePos = timerId / _timer_count_in_page;
        if (pagePos >= (int)_timer_page_count) return false;

        TimerData const * timerPage = _timer_buf[pagePos];

        return timerPage[timerId % _timer_count_in_page]
            ._realProcessor != NULL;
    }

private:
    void freeTl(void) {
        gd_tl_free(_tl);
        _tl = NULL;
    }

    TimerData * getTimer(TimerID timerId) {
        size_t pagePos = timerId / _timer_count_in_page;
        assert(pagePos < _timer_page_count);

        TimerData * timerPage = _timer_buf[pagePos];

        return &timerPage[timerId % _timer_count_in_page];
    }

    TimerID allocTimer(void) {
        if (_ids.empty()) {
            if (_timer_page_count + 1 >= _timer_page_capacity) {
                size_t newTimerPageCapacity = _timer_page_count + 128;
                TimerData ** newTimerBuf = (TimerData **)mem_alloc(_alloc, sizeof(TimerData*) * newTimerPageCapacity);
                if (newTimerBuf == NULL) throw ::std::bad_alloc();

                bzero(newTimerBuf, sizeof(TimerData *) * newTimerPageCapacity);
                memcpy(newTimerBuf, _timer_buf, sizeof(TimerData*) * _timer_page_count);

                if (_timer_buf) mem_free(_alloc, _timer_buf);

                _timer_buf = newTimerBuf;
                _timer_page_capacity = newTimerPageCapacity;

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

    static void destory_timer(gd_tl_event_t event, void * context) {
        TimerCenterImpl * ec = (TimerCenterImpl*)context;
        TimerID timerId = *reinterpret_cast<TimerID*>(gd_tl_event_data(event));

        TimerData * timerData = ec->getTimer(timerId);
        if (timerData == NULL) {
            APP_CTX_ERROR(ec->_app, "destory timer %d: timer not exist!", timerId);
            return;
        }

        if (timerData->_tl_event != event) {
            APP_CTX_ERROR(ec->_app, "destory timer %d: tl_event mismatch!", timerId);
            return;
        }

        timerData->_tl_event = NULL;
        ec->clearTimerData(*timerData);
        ec->freeTimerId(timerId);
    }

    static void dispatch_timer(gd_tl_event_t input, void * context) {
        TimerCenterImpl * ec = (TimerCenterImpl*)context;
        TimerID timerId = *reinterpret_cast<TimerID*>(gd_tl_event_data(input));

        try {
            TimerData * timerData = ec->getTimer(timerId);
            if (timerData == NULL) {
                APP_CTX_THROW_EXCEPTION(
                    ec->_app,
                    ::std::runtime_error,
                    "get timer data fail!");
            }

            (timerData->_useProcessor->*(timerData->_fun))(timerId);
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
        gd_tl_set_opt(tl, gd_tl_set_event_destory, destory_timer);
        gd_tl_set_opt(tl, gd_tl_set_event_op_context, &self);

        return tl;
    }

    void freeTimer(TimerData & data) {
        assert(data._realProcessor);
        assert(data._tl_event);

        clearTimerData(data);
        freeTimerId(data._id);

        if (_debug) {
            APP_CTX_INFO(_app, "unregister timer %d", data._id);
        }
    }
    
    void clearTimerData(TimerData & data) {
        if (data._tl_event) {
            gd_tl_event_free(data._tl_event);
            data._tl_event = NULL;
        }
        else {
            if (data._state == TimerDataState_InResponserHash) {
                cpe_hash_table_remove_by_ins(&_responser_to_timer, &data);
                data._state = TimerDataState_NotInResponserHash;
            }

            data._realProcessor = NULL;
            data._useProcessor = NULL;
            data._fun = NULL;
        }
    }

    void freeTimerId(TimerID timerId) {
        _ids.putOne(timerId);
    }

    static  uint32_t cpe_responser_hash_fun(const void * o) {
        const TimerData * timerData = (const TimerData*)o;
        return (((ptr_int_t)timerData->_realProcessor) & 0xFFFFFFFF);
    }

    static int cpe_responser_cmp_fun(const void * l, const void * r) {
        const TimerData * lTimerData = (const TimerData*)l;
        const TimerData * rTimerData = (const TimerData*)r;

        return lTimerData->_realProcessor == rTimerData->_realProcessor;
    }

    void initProcessorHash(void) {
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

    void finiProcessorHash(void) {
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

static cpe_hash_string_buf s_Event_DEFAULT_NAME_buf = CPE_HS_BUF_MAKE("AppTimerCenter");
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
    APP_CTX_CATCH_EXCEPTION(app, "create AppTimerCenter: ");
    return -1;
}

extern "C"
EXPORT_DIRECTIVE
void AppTimerCenter_app_fini(Gd::App::Application & app, Gd::App::Module & module) {
    app.nmManager().removeObject(Gd::App::TimerCenter::DEFAULT_NAME);
}

