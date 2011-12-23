#include <cassert>
#include <stdexcept>
#include "gd/nm/nm_manage.h"
#include "gdpp/nm/Group.hpp"

namespace Gd { namespace Nm {

Group::~Group() {
}

void * Group::operator new (size_t size, gd_nm_mgr_t nmm, const char * name) {
    gd_nm_node_t node = gd_nm_group_create(nmm, name, size);
    if (node == NULL) {
        throw ::std::bad_alloc();
    }

    return gd_nm_node_data(node);
}

}}
