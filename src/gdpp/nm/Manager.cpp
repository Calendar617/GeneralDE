#include <cassert>
#include <sstream>
#include <stdexcept>
#include "gd/nm/nm_manage.h"
#include "gdpp/nm/Manager.hpp"

namespace Gd { namespace Nm {

extern struct gd_nm_node_type g_object_type;

ObjectIterator Manager::objects(void) {
    ObjectIterator it;
    if (gd_nm_mgr_nodes(&it._it, *this) != 0) {
        throw ::std::runtime_error("get named objects of Manager fail!");
    }
    it._next = it.next_i();
    return it;
}

ConstObjectIterator Manager::objects(void) const {
    ConstObjectIterator it;
    if (gd_nm_mgr_nodes(&it._it, *this) != 0) {
        throw ::std::runtime_error("get named objects of Manager fail!");
    }
    it._next = it.next_i();
    return it;
}

Object const *
Manager::findObject(cpe_hash_string_t name) const {
    gd_nm_node_t node = gd_nm_mgr_find_node(*this, name);
    if (node == NULL
        || gd_nm_node_type(node) != &g_object_type)
    {
        return NULL;
    }

    return (Object*)node;
}

Object *
Manager::findObject(cpe_hash_string_t name) {
    gd_nm_node_t node = gd_nm_mgr_find_node(*this, name);
    if (node == NULL
        || gd_nm_node_type(node) != &g_object_type)
    {
        return NULL;
    }

    return (Object*)node;
}

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
