#include <cassert>
#include <sstream>
#include <stdexcept>
#include "gd/nm/nm_manage.h"
#include "gdpp/nm/Object.hpp"
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

    return Object::_cast(node);
}

Object *
Manager::findObject(cpe_hash_string_t name) {
    gd_nm_node_t node = gd_nm_mgr_find_node(*this, name);
    if (node == NULL
        || gd_nm_node_type(node) != &g_object_type)
    {
        return NULL;
    }

    return Object::_cast(node);
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

bool Manager::removeObject(cpe_hash_string_t name) {
    gd_nm_node_t node = gd_nm_mgr_find_node(*this, name);
    if (node) {
        gd_nm_node_free(node);
        return true;
    }
    else {
        return false;
    }
}

Object const * Manager::findObjectNc(const char * name) const {
    gd_nm_node_t node = gd_nm_mgr_find_node_nc(*this, name);
    if (node == NULL
        || gd_nm_node_type(node) != &g_object_type)
    {
        return NULL;
    }

    return Object::_cast(node);
}

Object * Manager::findObjectNc(const char * name) {
    gd_nm_node_t node = gd_nm_mgr_find_node_nc(*this, name);
    if (node == NULL
        || gd_nm_node_type(node) != &g_object_type)
    {
        return NULL;
    }

    return Object::_cast(node);
}

Object const & Manager::objectNc(const char * name) const {
    Object const * r = findObjectNc(name);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "named object " << name << " not exist!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

Object & Manager::objectNc(const char * name) {
    Object * r = findObjectNc(name);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "named object " << name << " not exist!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

}}
