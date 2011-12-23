#ifndef GDPP_NM_OBJECT_H
#define GDPP_NM_OBJECT_H
#include "gd/nm/nm_read.h"
#include "System.hpp"
#include "ObjectIterator.hpp"

namespace Gd { namespace Nm {

class Object {
public:
    Object();
    virtual ~Object() = 0;

    const char * name(void) const { return gd_nm_node_name(_to_node()); }
    cpe_hash_string_t name_hs(void) const { return gd_nm_node_name_hs(_to_node()); }

    operator gd_nm_node_t () { return gd_nm_node_from_data((void*)this); }

    Manager & manager(void) { return *((Manager *)_mgr()); }
    Manager const & manager(void) const { return *((Manager *)_mgr()); }

    ObjectIterator groups(void) {
        ObjectIterator it;
        gd_nm_node_groups(&it.m_it, *this);
        return it;
    }

    gd_nm_mgr_t _mgr(void) const { return gd_nm_node_mgr(_to_node()); }
    gd_nm_node_t _to_node(void) const { return gd_nm_node_from_data((void*)this); }

    void * operator new (size_t size, gd_nm_mgr_t nmm, const char * name);

    void operator delete(void *p);
private:
    void * operator new (size_t size);
};

}}

#endif
