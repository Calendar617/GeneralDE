#ifndef USF_LOGIC_OPCONTEXT_H
#define USF_LOGIC_OPCONTEXT_H
#include "cpepp/utils/ClassCategory.hpp"
#include "usf/logic/logic_context.h"
#include "usf/logic/logic_data.h"
#include "System.hpp"

namespace Usf { namespace Logic {

class LogicOpContext : public Cpe::Utils::SimulateObject  {
public:
    operator logic_context_t () const { return (logic_context_t)this; }

    LogicOpContextID id(void) const { return logic_context_id(*this); }
    bool debug(void) const;

    LogicOpData & data(const char * name);
    LogicOpData const & data(const char * name) const;

    LogicOpData * findData(const char * name) { return (LogicOpData *)logic_data_find(*this, name); }
    LogicOpData const * findData(const char * name) const { return (LogicOpData *)logic_data_find(*this, name); }
    LogicOpData & checkCreateData(const char * name, size_t capacity = 0);
    LogicOpData & checkCreateData(Cpe::Dr::Meta const & meta, size_t capacity = 0);

    void dump(cfg_t cfg) const;
};

}}

#endif
