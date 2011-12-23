#ifndef GDPP_NM_OBJECTITERATOR_H
#define GDPP_NM_OBJECTITERATOR_H
#include "gd/nm/nm_read.h"
#include "System.hpp"

namespace Gd { namespace Nm {

class ObjectIterator {
public:
    ObjectIterator() {}
    ObjectIterator(gd_nm_node_it it) : m_it(it) {}

    Object * next(void) { return (Object*)gd_nm_node_next(&m_it); }

private:
    struct gd_nm_node_it m_it;

friend class Manager;
friend class Object;
friend class Group;
};

}}

#endif
