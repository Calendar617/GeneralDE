#ifndef GDPP_NM_OBJECTITERATOR_H
#define GDPP_NM_OBJECTITERATOR_H
#include "gd/nm/nm_read.h"
#include "System.hpp"

namespace Gd { namespace Nm {

class ObjectIterator {
public:
    ObjectIterator() {}
    ObjectIterator(const ObjectIterator & o) : m_it(o.m_it) {}
    ObjectIterator(gd_nm_node_it it) : m_it(it) {}

    Object const * next(void) { 
        gd_nm_node_t node = gd_nm_node_next(&m_it);
        return (Object*)(node == NULL ? NULL : gd_nm_node_data(node));
    }

private:
    struct gd_nm_node_it m_it;

friend class Manager;
friend class Object;
friend class Group;
friend class ConstObjectIterator;
};

class ConstObjectIterator {
public:
    ConstObjectIterator() {}
    ConstObjectIterator(ConstObjectIterator const & o) : m_it(o.m_it) {}
    ConstObjectIterator(ObjectIterator const & o) : m_it(o.m_it) {}
    ConstObjectIterator(gd_nm_node_it it) : m_it(it) {}

    Object const * next(void) {
        gd_nm_node_t node = gd_nm_node_next(&m_it);
        return (Object*)(node == NULL ? NULL : gd_nm_node_data(node));
    }

private:
    struct gd_nm_node_it m_it;

friend class Manager;
friend class Object;
friend class Group;
};

}}

#endif
