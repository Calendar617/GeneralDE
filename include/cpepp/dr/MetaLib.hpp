#ifndef CPEPP_DR_METALIB_H
#define CPEPP_DR_METALIB_H
#include <vector>
#include "cpepp/utils/ClassCategory.hpp"
#include "cpe/dr/dr_metalib_manage.h"
#include "System.hpp"

namespace Cpe { namespace Dr {

class MetaLib : public Cpe::Utils::SimulateObject {
public:
    operator LPDRMETALIB (void) const { return (LPDRMETALIB)this; }

    static MetaLib const & _cast(const void * p, size_t size);
    static MetaLib const & _cast(LPDRMETALIB ml);

    static MetaLib const & _load_from_file(const char * file, ::std::vector<char> & buf);
};

}}

#endif
