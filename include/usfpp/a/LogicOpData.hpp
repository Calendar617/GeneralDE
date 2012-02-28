#ifndef USF_LOGIC_DATA_H
#define USF_LOGIC_DATA_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/dr/Meta.hpp"
#include "usf/logic/logic_data.h"
#include "System.hpp"

namespace Usf { namespace Logic {

class LogicOpData : public Cpe::Utils::SimulateObject {
public:
    operator logic_data_t () const { return (logic_data_t)this; }

    Cpe::Dr::Meta const & meta(void) const { return *(Cpe::Dr::Meta const *)logic_data_meta(*this); }
    void * data(void) { return logic_data_data(*this); }
    const void * data(void) const { return logic_data_data(*this); }
    size_t capacity(void) const { return logic_data_data(*this); }

    template<typename T>
    T & as(void) { return *(T *)data(); }

    template<typename T>
    T const & as(void) const { return *(T const *)data(); }
};

}}

#endif
