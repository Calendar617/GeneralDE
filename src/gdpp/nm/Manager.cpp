#include <cassert>
#include <sstream>
#include <stdexcept>
#include "gd/nm/nm_manage.h"
#include "gdpp/nm/Manager.hpp"

namespace Gd { namespace Nm {

Object const & Manager::object(cpe_hash_string_t name) const {
    Object const * r = findObject(name);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "named object " << cpe_hs_data(name) << " not exist!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

Object & Manager::object(cpe_hash_string_t name) {
    Object * r = findObject(name);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "named object " << cpe_hs_data(name) << " not exist!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

void Manager::removeObject(cpe_hash_string_t name) {
    gd_nm_node_free(
        gd_nm_mgr_find_node(*this, name));
}

}}
