#include <cassert>
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

static struct gd_nm_node_type g_object_type = {
    "Gd::Nm::Object",
    object_destruct
};

Object::Object() {
    gd_nm_node_set_type(_to_node(), &g_object_type);
}

Object::~Object() {
    gd_nm_node_set_type(_to_node(), NULL);
}

void * Object::operator new (size_t size, gd_nm_mgr_t nmm, const char * name) {
    printf("name=%s, size=%d, nmm=%p\n", name, size, nmm);
    gd_nm_node_t node = gd_nm_instance_create(nmm, name, size);
    if (node == NULL) {
        throw ::std::bad_alloc();
    }

    return gd_nm_node_data(node);
}

void Object::operator delete (void *p) {
    if (p == NULL) return;
    gd_nm_node_free(gd_nm_node_from_data(p));
}

}}
