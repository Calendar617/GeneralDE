#include <cassert>
#include <sstream>
#include <stdexcept>
#include "gd/nm/nm_manage.h"
#include "gdpp/nm/Object.hpp"

namespace Gd { namespace Nm {

static void object_destruct(gd_nm_node_t node) {
    Object * object = (Object*)gd_nm_node_data(node);

    try {
        object->~Object();
    }
    catch(...) {
    }
}

struct gd_nm_node_type g_object_type = {
    "Gd::Nm::Object",
    object_destruct
};

Object::Object() {
    gd_nm_node_set_type(*this, &g_object_type);
}

Object::~Object() {
    gd_nm_node_set_type(*this, NULL);
}

void * Object::operator new (size_t size, gd_nm_mgr_t nmm, const char * name) {
    gd_nm_node_t node = gd_nm_instance_create(nmm, name, size);
    if (node == NULL) {
        if (gd_nm_mgr_find_node_nc(nmm, name)) {
            ::std::ostringstream os;
            os << "named object " << name << " already exist!";
            throw ::std::runtime_error(os.str());
        }
        else {
            throw ::std::bad_alloc();
        }
    }

    return gd_nm_node_data(node);
}

void Object::operator delete (void *p) {
    if (p == NULL) return;
    gd_nm_node_free(gd_nm_node_from_data(p));
}

Object * Object::_cast(gd_nm_node_t node) {
    if (node == NULL || gd_nm_node_type(node) != &g_object_type) {
        return NULL;
    }
    else {
        return (Object*)gd_nm_node_data(node);
    }
}

ObjectIterator
Object::groups(void) {
    ObjectIterator it;
    if (gd_nm_node_groups(&it._it, *this) != 0) {
        ::std::ostringstream os;
        os << "get groups of named object " << name() << " fail!";
        throw ::std::runtime_error(os.str());
    }
    it._next = it.next_i();
    return it;
}

ConstObjectIterator
Object::groups(void) const {
    ConstObjectIterator it;
    if (gd_nm_node_groups(&it._it, *this) != 0) {
        ::std::ostringstream os;
        os << "get groups of named object " << name() << " fail!";
        throw ::std::runtime_error(os.str());
    }
    it._next = it.next_i();
    return it;
}

Object const *
ConstObjectIterator::next_i(void) {
    return Object::_cast(gd_nm_node_next(&_it));
}

Object *
ObjectIterator::next_i(void) {
    return Object::_cast(gd_nm_node_next(&_it));
}

}}
