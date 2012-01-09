#include <stdexcept>
#include <sstream>
#include "cpepp/dr/MetaLib.hpp"

namespace Cpe { namespace Dr {

Meta const & MetaLib::meta(const char * name) {
    Meta const * r = findMeta(name);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "meta-lib : get meta (name=" << name << ") fail!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

Meta const & MetaLib::meta(int id) const {
    Meta const * r = findMeta(id);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "meta-lib : get meta (id=" << id << ") fail!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

MetaLib const &
MetaLib::_cast(const void * p) {
    if (p == NULL) {
        throw ::std::runtime_error("cast to MetaLib: input is NULL!"); 
    }
    return *(MetaLib const *)p;
}

MetaLib const & MetaLib::_cast(LPDRMETALIB ml) {
    if (ml == NULL) {
        throw ::std::runtime_error("cast to MetaLib: input is NULL!"); 
    }
    return *(MetaLib const *)ml;
}

MetaLib const &
MetaLib::_load_from_file(const char * file, ::std::vector<char> & buf) {
    throw 0;
}

}}
