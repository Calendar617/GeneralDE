#ifndef USF_LOGIC_SYSTEM_H
#define USF_LOGIC_SYSTEM_H
#include <memory>
#include "cpe/pal/pal_types.h"
#include "gdpp/app/System.hpp"
#include "cpepp/cfg/System.hpp"
#include "cpepp/utils/System.hpp"
#include "usf/logic/logic_types.h"

namespace Usf { namespace Logic {

class LogicOpData;
class LogicOpContext;
class LogicOpManager;

typedef logic_context_id_t LogicOpContextID;

}}

#endif
