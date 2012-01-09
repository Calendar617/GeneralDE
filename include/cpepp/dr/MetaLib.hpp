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

    Meta const * findMeta(const char * name) const { return (Meta const *)dr_lib_find_meta_by_name(*this, name); }
    Meta const & meta(const char * name);

    Meta const * findMeta(int id) const  { return (Meta const *)dr_lib_find_meta_by_id(*this, id); }
    Meta const & meta(int id) const;

    static MetaLib const & _cast(const void * p);
    static MetaLib const & _cast(LPDRMETALIB ml);

    static MetaLib const & _load_from_file(const char * file, ::std::vector<char> & buf);
};

}}

#endif
