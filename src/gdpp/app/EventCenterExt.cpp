#include <cassert>
#include <stdexcept>
#include <vector>
#include <map>
#include <sstream>
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_strings.h"
#include "cpe/pal/pal_external.h"
#include "cpe/utils/hash.h"
#include "cpe/utils/stream_mem.h"
#include "cpepp/utils/OpGuard.hpp"
#include "cpepp/utils/RangeMgr.hpp"
#include "cpepp/utils/MemBuffer.hpp"
#include "cpepp/cfg/Node.hpp"
#include "cpepp/dr/MetaLib.hpp"
#include "gdpp/dp/Manager.hpp"
#include "gdpp/dp/Request.hpp"
#include "gdpp/dp/Responser.hpp"
#include "gdpp/nm/Manager.hpp"
#include "gdpp/tl/Manager.hpp"
#include "gdpp/app/Log.hpp"
#include "gdpp/app/Application.hpp"
#include "gdpp/evt/EventCenter.hpp"
#include "EventCenterExt.hpp"

namespace Gd { namespace App {

class EventCenterImpl : public EventCenterExt {
public:
    enum ProcessorDataState {
        ProcessorDataState_NotInResponserHash = 0
        , ProcessorDataState_InResponserHash
    };

    struct ProcessorData {
        ProcessorID _id;
        ProcessorDataState _state;
        EventResponser * _realResponser;
        EventResponser * _useResponser;
        EventProcessFun _fun;
        cpe_hash_entry _hh_for_responser_to_processor;
    };
 
    EventCenterImpl(
        Gd::App::Application & app, Cpe::Cfg::Node & cfg)
        : _app(app)
        , _alloc(app.allocrator())
        , _is_debug(cfg["debug"].dft(0))
        , _tl(NULL)
        , _emm(NULL)
        , _req(NULL)
        , _oid_max_len(cfg["oid-max-len"].dft(32))
        , _metaLibBuf(app.allocrator())
        , _processor_count_in_page(cfg["processor_count_per_page"].dft(2048))
        , _processor_page_count(0)
        , _processor_page_capacity(0)
        , _processor_buf(NULL)
        , _ids(app.allocrator())
   {
        Cpe::Utils::OpGuard opGuard;

        initProcessorHash();
        opGuard.addOp(*this, &EventCenterImpl::finiProcessorHash);

        _tl = createTl(app, *this);
        opGuard.addOp(*this, &EventCenterImpl::freeTl);

        Cpe::Dr::MetaLib const & metaLib = loadMetaLib(_metaLibBuf, cfg);
        _emm = createEvtMgr(_tl, app, metaLib);
        opGuard.addOp(*this, &EventCenterImpl::freeEvtMgr);

        _req = createReq(app);
        opGuard.addOp(*this, &EventCenterImpl::freeReq);

        opGuard.releaseControl();
    }

    ~EventCenterImpl() {
        freeReq();
        freeEvtMgr();
        freeTl();
        finiProcessorHash();
        freeProcessorBuf();
    }

    virtual Event & createEvent(const char * typeName, ssize_t capacity) {
        Event & e = ec().createEvent(
            cpe_hs_len_to_binary_len(_oid_max_len),
            typeName,
            capacity);

        return e;
    }

	virtual void sendEvent(const char * oid, Event & event) {
        assert(event.attach_capacity() == cpe_hs_len_to_binary_len(_oid_max_len));
        cpe_hs_init(
            (cpe_hash_string_t)event.attach_buf(),
            event.attach_capacity(),
            oid);
        ec().sendEvent(event);
    }

	virtual void registerResponser(const char * oid, EventResponser& realResponser, EventResponser & useResponser, EventProcessFun fun) {
        try {
            ProcessorID newProcessorId = allocProcessor();
            ProcessorData * newProcessorData = getProcessor(newProcessorId);
            assert(newProcessorData);
            assert(newProcessorData->_realResponser == NULL);
            assert(newProcessorData->_state == ProcessorDataState_NotInResponserHash);

            newProcessorData->_realResponser = &realResponser;
            newProcessorData->_useResponser = &useResponser;
            newProcessorData->_fun = fun;

            int r = cpe_hash_table_insert(&_responser_to_processor, newProcessorData);
            if (r != 0) {
                clearProcessorData(*newProcessorData);
                freeProcessorId(newProcessorId);
                APP_CTX_THROW_EXCEPTION(
                    _app,
                    ::std::runtime_error,
                    "registerProcessor: insert to responser processor list fail!");
            }
            newProcessorData->_state = ProcessorDataState_InResponserHash;

            char rspNameBuf[32];
            snprintf(rspNameBuf, sizeof(rspNameBuf), "rsp.%d", newProcessorId);
            try {
                Gd::Dp::Responser & rsp = _app.dpManager().createResponser(rspNameBuf);

                rsp.setProcessor(apply_evt, newProcessorData);

                _app.dpManager().bind(rsp, oid);
            }
            catch(...) {
                clearProcessorData(*newProcessorData);
                freeProcessorId(newProcessorId);
                throw;
            }
        }
        APP_CTX_CATCH_EXCEPTION_RETHROW(_app, "register EventResponser to oid %s: ", oid);
    }

	virtual void unregisterResponser(EventResponser & r) {
        ProcessorData key;
        key._realResponser = &r;

        ProcessorData * preNode = (ProcessorData*)cpe_hash_table_find(&_responser_to_processor, &key);
        while(preNode) {
            ProcessorData * nextNode = 
                (ProcessorData *)cpe_hash_table_find_next(&_responser_to_processor, preNode);
            assert(preNode->_realResponser);
            freeProcessor(*preNode);
            preNode = nextNode;
        }
    }

private:
    Cpe::Dr::MetaLib const &  loadMetaLib(Cpe::Utils::MemBuffer & buf, Cpe::Cfg::Node & cfg) {
        const char * file = cfg["load-from"].dft((const char *)NULL);
        if (file) {
            return Cpe::Dr::MetaLib::_load_from_xml_file(file, buf);
        }

        if (const char * xml = cfg["load-from-memory"].dft((const char *)NULL)) {
            return Cpe::Dr::MetaLib::_load_from_xml(xml, buf);
        }

        APP_CTX_THROW_EXCEPTION(
            _app,
            ::std::runtime_error,
            "unknown event def, no load-from and load-from-memory configured!");
    }

    Gd::Evt::EventCenter & ec(void) {
        return Gd::Evt::EventCenter::_cast(_emm);
    }

    void freeTl(void) {
        gd_tl_free(_tl);
    }

    void freeEvtMgr(void) {
        if (_emm) {
            gd_evt_mgr_free(_emm);
            _emm = NULL;
        }
    }

    void freeReq(void) {
        if (_req) {
            gd_dp_req_free(_req);
            _req = NULL;
        }
    }

    static int apply_evt(gd_dp_req_t req, void * ctx, error_monitor_t em) {
        ProcessorData * pn = (ProcessorData*)ctx;

        try {
            gd_tl_event_t input = (gd_tl_event_t)gd_dp_req_data(req);
            Event & e = Event::_cast(input);

            (pn->_useResponser->*(pn->_fun))(cpe_hs_data((cpe_hash_string_t)e.attach_buf()), e);
        }
        APP_CATCH_EXCEPTION("process event:"); //TODO: record to app

        return 0;
    }

    static void dispatch_evt(gd_tl_event_t input, void * context) {
        EventCenterImpl * ec = (EventCenterImpl*)context;
        Event * evt = NULL;
        try {
            evt = &Event::_cast(input);

            gd_dp_req_set_buf(ec->_req, (void*)input, (size_t)0);

            ec->_app.dpManager().dispatch((cpe_hash_string_t)evt->attach_buf(), ec->_req);

            if (ec->_is_debug) {
                char buf[512];
                struct write_stream_mem stream = CPE_WRITE_STREAM_MEM_INITIALIZER(buf, sizeof(buf) - 1);
                bzero(buf, sizeof(buf));
                evt->dump((write_stream_t)&stream);

                APP_CTX_INFO(
                    ec->_app, "dispatch event: success, oid=%s, event=%s",
                    cpe_hs_data((cpe_hash_string_t)evt->attach_buf()),
                    buf);
            }
        }
        catch(::std::exception const & e) {
            if (evt) {
                char buf[512];
                struct write_stream_mem stream = CPE_WRITE_STREAM_MEM_INITIALIZER(buf, sizeof(buf) - 1);
                bzero(buf, sizeof(buf));
                evt->dump((write_stream_t)&stream);

                APP_CTX_ERROR(
                    ec->_app, "dispatch event: catch exception: %s, oid=%s, event=%s%s",
                    e.what(),
                    cpe_hs_data((cpe_hash_string_t)evt->attach_buf()),
                    evt->type().c_str(),
                    buf);
            }
            else {
                APP_CTX_ERROR(ec->_app, "dispatch event: catch exception: %s!", e.what());
            }
        }
        catch(...) {
            APP_CTX_ERROR(ec->_app, "dispatch event: catch unknown exception!");
        }
    }

    static gd_tl_t createTl(Gd::App::Application & app, EventCenterImpl & self) {
        gd_tl_t tl = gd_tl_create(app.tlManager());
        if (tl == NULL) {
            APP_CTX_THROW_EXCEPTION(app, ::std::runtime_error, "create tl fail!");
        }

        gd_tl_set_opt(tl, gd_tl_set_event_dispatcher, dispatch_evt);
        gd_tl_set_opt(tl, gd_tl_set_event_op_context, &self);

        return tl;
    }

    static gd_evt_mgr_t createEvtMgr(gd_tl_t _tl, Gd::App::Application & app, Cpe::Dr::MetaLib const & metaLib) {
        gd_evt_mgr_t evm = gd_evt_mgr_create(_tl, metaLib, app.allocrator());
        if (evm == NULL) {
            APP_CTX_THROW_EXCEPTION(app, ::std::runtime_error, "create event mgr fail!");
        }
        return evm;
    }

    static gd_dp_req_t createReq(Gd::App::Application & app) {
        gd_dp_req_t r = gd_dp_req_create(app.dpManager(), req_type_name, 0);
        if (r == NULL) {
            APP_CTX_THROW_EXCEPTION(app, ::std::runtime_error, "create req fail!");
        }
        return r;
    }

    ProcessorData * getProcessor(ProcessorID processorId) {
        size_t pagePos = processorId / _processor_count_in_page;
        assert(pagePos < _processor_page_count);

        ProcessorData * processorPage = _processor_buf[pagePos];

        return &processorPage[processorId % _processor_count_in_page];
    }

    ProcessorID allocProcessor(void) {
        if (_ids.empty()) {
            if (_processor_page_count + 1 >= _processor_page_capacity) {
                size_t newProcessorPageCapacity = _processor_page_count + 128;
                ProcessorData ** newProcessorBuf = (ProcessorData **)mem_alloc(_alloc, sizeof(ProcessorData*) * newProcessorPageCapacity);
                if (newProcessorBuf == NULL) throw ::std::bad_alloc();

                bzero(newProcessorBuf, sizeof(ProcessorData *) * newProcessorPageCapacity);
                memcpy(newProcessorBuf, _processor_buf, sizeof(ProcessorData*) * _processor_page_count);

                if (_processor_buf) mem_free(_alloc, _processor_buf);

                _processor_buf = newProcessorBuf;
                _processor_page_capacity = newProcessorPageCapacity;

                if (_is_debug) {
                    APP_CTX_INFO(_app, "resize processor buf to %zd", newProcessorPageCapacity);
                }
            }

            ptr_int_t newStart = _processor_page_count * _processor_count_in_page;
            ProcessorData * newPage =
                (ProcessorData *)mem_alloc(_alloc, sizeof(ProcessorData) * _processor_count_in_page);
            if (newPage == NULL) {
                throw ::std::bad_alloc();
            }
            bzero(newPage, sizeof(ProcessorData) * _processor_count_in_page);
            for(size_t i = 0; i < _processor_count_in_page; ++i) {
                newPage[i]._id = newStart + i;
            }

            _ids.putRange(newStart, newStart + _processor_count_in_page);
            _processor_buf[_processor_page_count] = newPage;
            ++_processor_page_count;

            if (_is_debug) {
                APP_CTX_INFO(
                    _app,
                    "alloc a new processor page[%d,%d), page count is %zd",
                    (ProcessorID)newStart, (ProcessorID)(newStart + _processor_page_count),
                    _processor_page_count);
            }
        }

        return (ProcessorID)_ids.getOne();
    }

    void freeProcessorBuf(void) {
        if (_processor_buf == NULL) return;

        for(size_t i = 0; i < _processor_page_count; ++i) {
            mem_free(_alloc, _processor_buf[i]);
        }

        mem_free(_alloc, _processor_buf);
        _processor_page_count = 0;
        _processor_page_capacity = 0;
    }

    void freeProcessor(ProcessorData & data) {
        assert(data._realResponser);

        char rspNameBuf[32];
        snprintf(rspNameBuf, sizeof(rspNameBuf), "rsp.%d", data._id);
        _app.dpManager().deleteResponser(rspNameBuf);

        clearProcessorData(data);
        freeProcessorId(data._id);
    }
    
    void clearProcessorData(ProcessorData & data) {
        if (data._state == ProcessorDataState_InResponserHash) {
            cpe_hash_table_remove_by_ins(&_responser_to_processor, &data);
            data._state = ProcessorDataState_NotInResponserHash;
        }

        data._realResponser = NULL;
        data._useResponser = NULL;
        data._fun = NULL;
    }

    void freeProcessorId(ProcessorID processorId) {
        _ids.putOne(processorId);
    }

    static  uint32_t cpe_responser_hash_fun(const void * o) {
        const ProcessorData * processorData = (const ProcessorData*)o;
        return (((ptr_int_t)processorData->_realResponser) & 0xFFFFFFFF);
    }

    static int cpe_responser_cmp_fun(const void * l, const void * r) {
        const ProcessorData * lProcessorData = (const ProcessorData*)l;
        const ProcessorData * rProcessorData = (const ProcessorData*)r;

        return lProcessorData->_realResponser == rProcessorData->_realResponser;
    }

    void initProcessorHash(void) {
        int r = cpe_hash_table_init(
            &_responser_to_processor,
            _alloc,
            cpe_responser_hash_fun,
            cpe_responser_cmp_fun,
            CPE_HASH_OBJ2ENTRY(ProcessorData, _hh_for_responser_to_processor),
            _processor_count_in_page);
        if (r != 0) {
            APP_CTX_THROW_EXCEPTION(
                _app,
                ::std::runtime_error,
                "init responser_to_processor fail!");
        }
    }

    void finiProcessorHash(void) {
        cpe_hash_table_fini(&_responser_to_processor);
    }

    Gd::App::Application & _app;
    mem_allocrator_t _alloc;
    int _is_debug;
    gd_tl_t _tl;
    gd_evt_mgr_t _emm;
    gd_dp_req_t _req;
    size_t _oid_max_len;
    Cpe::Utils::MemBuffer _metaLibBuf;

    size_t _processor_count_in_page;
    size_t _processor_page_count;
    size_t _processor_page_capacity;
    ProcessorData ** _processor_buf;
    cpe_hash_table _responser_to_processor;
    Cpe::Utils::RangeMgr _ids;
};

EventCenter::~EventCenter() {
}

EventCenter & EventCenter::instance(Application & app, cpe_hash_string_t name) {
    EventCenter * r =
        dynamic_cast<EventCenter *>(
            &app.nmManager().object(name));
    if (r == NULL) {
        APP_CTX_THROW_EXCEPTION(app, ::std::runtime_error, "EventCenter cast fail!");
    }

    return *r;
}

static cpe_hash_string_buf s_Event_req_type_name_buf = CPE_HS_BUF_MAKE("app.event.req");
cpe_hash_string_t EventCenter::req_type_name = (cpe_hash_string_t)s_Event_req_type_name_buf;

static cpe_hash_string_buf s_Event_DEFAULT_NAME_buf = CPE_HS_BUF_MAKE("AppEventCenter");
cpe_hash_string_t EventCenter::DEFAULT_NAME = (cpe_hash_string_t)s_Event_DEFAULT_NAME_buf;

}}

extern "C"
EXPORT_DIRECTIVE
int AppEventCenter_app_init(Gd::App::Application & app, Gd::App::Module & module, Cpe::Cfg::Node & cfg) {
    try {
        return (new (app.nmManager(), cpe_hs_data(Gd::App::EventCenter::DEFAULT_NAME))
                Gd::App::EventCenterImpl(app, cfg))
            == NULL
            ? -1
            : 0;
    }
    APP_CTX_CATCH_EXCEPTION(app, "create AppEventCenter: ");
    return -1;
}

extern "C"
EXPORT_DIRECTIVE
void AppEventCenter_app_fini(Gd::App::Application & app, Gd::App::Module & module) {
    app.nmManager().removeObject(Gd::App::EventCenter::DEFAULT_NAME);
}
