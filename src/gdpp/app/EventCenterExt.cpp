#include <cassert>
#include <stdexcept>
#include <vector>
#include <map>
#include <sstream>
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_strings.h"
#include "cpe/pal/pal_external.h"
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
    struct ProcessorNode {
        ProcessorNode() : _processor(NULL), _process_fun(NULL) {}

        EventResponser * _processor;
        EventProcessFun _process_fun;
    };
    typedef ::std::vector<ProcessorNode> ProcessorNodeContainer;

    typedef ::std::vector<int> IdVector;
    typedef ::std::map< ptr_int_t, IdVector> IdRspMap;

    EventCenterImpl(
        Gd::App::Application & app, Cpe::Cfg::Node & cfg)
        : _app(app)
        , _is_debug(cfg["debug"].dft(0))
        , _tl(NULL)
        , _emm(NULL)
        , _req(NULL)
        , _oid_max_len(cfg["oid-max-len"].dft(32))
        , _metaLibBuf(app.allocrator())
        , _ids(app.allocrator())
    {
        Cpe::Utils::OpGuard opGuard;

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

    /*恶心的代码，具体参看头文�*/
	virtual void registerResponser(const char * oid, EventResponser& realResponser, EventResponser & useResponser, EventProcessFun fun) {
        IdVector & ids = _idRsps[(ptr_int_t)&realResponser];

        ptr_int_t newId = allocProcessor();

        ids.reserve(ids.size() + 1);

        ::std::ostringstream os;
        os << "rsp." << newId;

        Gd::Dp::Responser & rsp = _app.dpManager().createResponser(os.str().c_str());
        rsp.setProcessor(apply_evt);

        ProcessorNode & pn = _processors[newId];
        pn._processor = &useResponser;
        pn._process_fun = fun;

        rsp.setContext(&pn);

        _app.dpManager().bind(rsp, oid);

        ids.push_back(newId);
    }

	virtual void unregisterResponser(EventResponser & r) {
        ptr_int_t rId = (ptr_int_t)&r;

        IdRspMap::iterator pos = _idRsps.find(rId);
        if (pos == _idRsps.end()) return;

        IdVector & ids = pos->second;
        for(IdVector::iterator idIt = ids.begin(); idIt != ids.end(); ++idIt) {
            ::std::ostringstream os;
            os << "rsp." << *idIt;
            _app.dpManager().deleteResponser(os.str().c_str());
        }
    }

private:
    Cpe::Dr::MetaLib const &  loadMetaLib(Cpe::Utils::MemBuffer & buf, Cpe::Cfg::Node & cfg) {
        const char * file = cfg["laod-from"];
        return Cpe::Dr::MetaLib::_load_from_xml_file(file, buf);
    }

    Gd::Evt::EventCenter & ec(void) {
        return Gd::Evt::EventCenter::_cast(_emm);
    }

    void freeTl(void) {
        if (_tl) {
            _tl = NULL;
        }
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
        ProcessorNode * pn = (ProcessorNode*)ctx;

        try {
            gd_tl_event_t input = (gd_tl_event_t)gd_dp_req_data(req);
            Event & e = Event::_cast(input);

            (pn->_processor->*(pn->_process_fun))(cpe_hs_data((cpe_hash_string_t)e.attach_buf()), e);
        }
        catch(...) {
        }

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
                    ec->_app, "dispatch event: catch exception: %s, oid=%s, event=%s",
                    e.what(),
                    cpe_hs_data((cpe_hash_string_t)evt->attach_buf()),
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

    ptr_int_t allocProcessor(void) {
        if (_ids.empty()) {
            _processors.resize(_processors.size() + 128);
            _ids.putRange(_processors.size() - 128, _processors.size());
        }

        return _ids.getOne();
    }

    void releaseProcessor(int id) {
        assert(id < (int)_processors.size());
        _processors[id]._processor = NULL;
        _processors[id]._process_fun = NULL;
        _ids.putOne(id);
    }

    Gd::App::Application & _app;
    int _is_debug;
    gd_tl_t _tl;
    gd_evt_mgr_t _emm;
    gd_dp_req_t _req;
    size_t _oid_max_len;
    Cpe::Utils::MemBuffer _metaLibBuf;

    Cpe::Utils::RangeMgr _ids;
    ProcessorNodeContainer _processors;
    IdRspMap _idRsps;
};

EventCenter::~EventCenter() {
}

EventCenter & EventCenter::instance(Application & app, cpe_hash_string_t name) {
    EventCenter * r =
        dynamic_cast<EventCenter *>(
            &app.nmManager().object(name));
    if (r == NULL) {
        APP_THROW_EXCEPTION(
            ::std::runtime_error, "EventCenter cast fail!");
    }

    return *r;
}

static cpe_hash_string_buf s_Event_req_type_name_buf = CPE_HS_BUF_MAKE("app.event.req");
cpe_hash_string_t EventCenter::req_type_name = (cpe_hash_string_t)s_Event_req_type_name_buf;

static cpe_hash_string_buf s_Event_DEFAULT_NAME_buf = CPE_HS_BUF_MAKE("app.event.req");
cpe_hash_string_t EventCenter::DEFAULT_NAME = (cpe_hash_string_t)s_Event_DEFAULT_NAME_buf;

}}

extern "C"
EXPORT_DIRECTIVE
int AppEventCenter_app_init(Gd::App::Application & app, Gd::App::Module & module, Cpe::Cfg::Node & cfg) {
    return (new (app.nmManager(), cpe_hs_data(Gd::App::EventCenter::DEFAULT_NAME))
            Gd::App::EventCenterImpl(app, cfg))
        == NULL
        ? -1
        : 0;
}

extern "C"
EXPORT_DIRECTIVE
void AppEventCenter_app_fini(Gd::App::Application & app, Gd::App::Module & module) {
    app.nmManager().removeObject(Gd::App::EventCenter::DEFAULT_NAME);
}
