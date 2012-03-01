#ifndef USFPP_LOGIC_OPCONTEXT_H
#define USFPP_LOGIC_OPCONTEXT_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/dr/Meta.hpp"
#include "gdpp/app/Application.hpp"
#include "usf/logic/logic_context.h"
#include "usf/logic/logic_data.h"
#include "System.hpp"

namespace Usf { namespace Logic {

class LogicOpContext : public Cpe::Utils::SimulateObject  {
public:
    operator logic_context_t () const { return (logic_context_t)this; }

    LogicOpContextID id(void) const { return logic_context_id(*this); }
    bool debug(void) const;

    size_t capacity(void) const { return logic_context_capacity(*this); }
    void * data(void) { return logic_context_data(*this); }
    const void * data(void) const { return logic_context_data(*this); }

    LogicOpManager & mgr(void) { return *(LogicOpManager*)logic_context_mgr(*this); }
    LogicOpManager const & mgr(void) const { return *(LogicOpManager*)logic_context_mgr(*this); }

    Gd::App::Application & app(void) { return Gd::App::Application::_cast(logic_context_app(*this)); }
    Gd::App::Application const & app(void) const { return Gd::App::Application::_cast(logic_context_app(*this)); }

    LogicOpData & data(const char * name);
    LogicOpData const & data(const char * name) const;

    LogicOpData * findData(const char * name) { return (LogicOpData *)logic_data_find(*this, name); }
    LogicOpData const * findData(const char * name) const { return (LogicOpData *)logic_data_find(*this, name); }
    LogicOpData & checkCreateData(LPDRMETA meta, size_t capacity = 0);

    void dump(cfg_t cfg) const;
};

}}

#endif
