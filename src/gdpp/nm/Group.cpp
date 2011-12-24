#include <cassert>
#include <sstream>
#include <stdexcept>
#include "gd/nm/nm_manage.h"
#include "gdpp/nm/Group.hpp"

namespace Gd { namespace Nm {

Group::~Group() {
}

void * Group::operator new (size_t size, gd_nm_mgr_t nmm, const char * name) {
    gd_nm_node_t node = gd_nm_group_create(nmm, name, size);
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

Object const & Group::member(cpe_hash_string_t name) const {
    Object const * r = findMember(name);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "named object " << cpe_hs_data(name)
           << " not exist in group " << this->name() << "!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

Object & Group::member(cpe_hash_string_t name) {
    Object * r = findMember(name);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "named object " << cpe_hs_data(name)
           << " not exist in group " << this->name() << "!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

void Group::destoryMembers(void) {
    gd_nm_group_free_members(*this);
}

void Group::addMember(Object & object) {
    if (gd_nm_group_add_member(*this, object) != 0) {
        ::std::ostringstream os;
        os << "add object " << object.name() << " to group " << name() << " fail!";
        throw ::std::runtime_error(os.str());
    }
}

ObjectIterator Group::members(void) {
    ObjectIterator it;
    if (gd_nm_group_members(&it._it, *this) != 0) {
        ::std::ostringstream os;
        os << "get members fail! named group(?) " << name() << " is not group?";
        throw ::std::runtime_error(os.str());
    }
    it._next = it.next_i();
    return it;
}

ConstObjectIterator Group::members(void) const {
    ConstObjectIterator it;
    if (gd_nm_group_members(&it._it, *this) != 0) {
        ::std::ostringstream os;
        os << "get members fail! named group(?) " << name() << " is not group?";
        throw ::std::runtime_error(os.str());
    }
    it._next = it.next_i();
    return it;
}

}}
