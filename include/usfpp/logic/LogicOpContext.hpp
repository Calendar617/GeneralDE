#ifndef USFPP_LOGIC_OPCONTEXT_H
#define USFPP_LOGIC_OPCONTEXT_H
#include "cpe/utils/hash_string.h"
#include "cpepp/utils/System.hpp"
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/dr/Meta.hpp"
#include "gdpp/app/Application.hpp"
#include "usf/logic/logic_context.h"
#include "usf/logic/logic_data.h"
#include "LogicOpData.hpp"

namespace Usf { namespace Logic {

class LogicOpContext : public Cpe::Utils::SimulateObject  {
public:
    operator logic_context_t () const { return (logic_context_t)this; }

    LogicOpContextID id(void) const { return logic_context_id(*this); }

    logic_context_state_t state(void) const { return logic_context_state(*this); }

    size_t capacity(void) const { return logic_context_capacity(*this); }
    void * data(void) { return logic_context_data(*this); }
    const void * data(void) const { return logic_context_data(*this); }

    uint32_t flags(void) const { return logic_context_flags(*this); }
    void setFlags(uint32_t flag) { logic_context_flags_set(*this, flag); }
    void enableFlag(logic_context_flag_t flag) { logic_context_flag_enable(*this, flag); }
    void disableFlag(logic_context_flag_t flag) { logic_context_flag_disable(*this, flag); }
    bool isFlagEnable(logic_context_flag_t flag) const { return logic_context_flag_is_enable(*this, flag) ? true : false; }

    bool debug(void) const  { return logic_context_flag_is_enable(*this, logic_context_flag_debug) ? true : false; }
    void setErrno(int32_t err) { logic_context_errno_set(*this, err); }

    LogicOpManager & mgr(void) { return *(LogicOpManager*)logic_context_mgr(*this); }
    LogicOpManager const & mgr(void) const { return *(LogicOpManager*)logic_context_mgr(*this); }

    Cpe::Utils::Random & random(cpe_hash_string_t name = 0);

    void bind(logic_executor_t executor);
    void execute(void) { logic_context_execute(*this); }
    void execute(logic_executor_t execute) { bind(execute); logic_context_execute(*this); }

    void cancel(void) { logic_context_cancel(*this); }
    void timeout(void) { logic_context_timeout(*this); }

    Gd::App::Application & app(void) { return Gd::App::Application::_cast(logic_context_app(*this)); }
    Gd::App::Application const & app(void) const { return Gd::App::Application::_cast(logic_context_app(*this)); }

    LogicOpData & data(const char * name);
    LogicOpData const & data(const char * name) const;

    LogicOpData * findData(const char * name) { return (LogicOpData *)logic_data_find(*this, name); }
    LogicOpData const * findData(const char * name) const { return (LogicOpData *)logic_data_find(*this, name); }
    LogicOpData & checkCreateData(LPDRMETA meta, size_t capacity = 0);

    template<typename T>
    T & data(void) { return *(T*)data(); }

    template<typename T>
    T const & data(void) const { return *(T const *)data(); }

    template<typename T>
    T & data(const char * name) { return data(name).as<T>(); }

    template<typename T>
    T const & data(const char * name) const { return data(name).as<T>(); }

    void dump_data(cfg_t cfg) const { logic_context_data_dump_to_cfg(*this, cfg); }

    void destory(void) { logic_context_free(*this); }
};

}}

#endif
