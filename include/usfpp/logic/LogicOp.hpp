#ifndef USFPP_LOGIC_LOGICOP_H
#define USFPP_LOGIC_LOGICOP_H
#include "gdpp/nm/Object.hpp"
#include "System.hpp"

namespace Usf { namespace Logic {

class LogicOp : public Gd::Nm::Object {
public:
    virtual void execute(
        LogicOpContext & context,
        Cpe::Cfg::Node const & cfg,
        Cpe::Utils::Random & random) const = 0;

    virtual ~LogicOp();

    static LogicOp const & get(Gd::App::Application & app, cpe_hash_string_t name);
    static LogicOp const & get(Gd::App::Application & app, const char * name);
};

}}

#endif
