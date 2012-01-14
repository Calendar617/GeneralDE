#ifndef GDPP_APP_SYSTEM_H
#define GDPP_APP_SYSTEM_H
#include "cpepp/cfg/System.hpp"
#include "gd/app/app_types.h"
#include "gdpp/nm/System.hpp"
#include "gdpp/tl/System.hpp"
#include "gdpp/dp/System.hpp"
#include "gdpp/evt/System.hpp"

namespace Gd { namespace App {

class Application;
class Module;
class Library;
class Responser;

typedef ::Gd::Evt::Event Event;
class EventCenter;
class EventResponser;
class EventResponserBase;
typedef void (EventResponser::*EventProcessFun)(const char * oid, Event const & e);

}}

#endif
