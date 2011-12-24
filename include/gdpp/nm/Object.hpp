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

    const char * name(void) const { return gd_nm_node_name(*this); }
    cpe_hash_string_t name_hs(void) const { return gd_nm_node_name_hs(*this); }
    gd_nm_node_category_t category(void) { return gd_nm_node_category(*this); }

    operator gd_nm_node_t () const { return gd_nm_node_from_data((void*)this); }

    Manager & manager(void) { return *((Manager *)gd_nm_node_mgr(*this)); }
    Manager const & manager(void) const { return *((Manager *)gd_nm_node_mgr(*this)); }

    ObjectIterator groups(void);
    ConstObjectIterator groups(void) const;

    void * operator new (size_t size, gd_nm_mgr_t nmm, const char * name);

    void operator delete(void *p);

    static Object * _cast(gd_nm_node_t node);
    static Object * _cast_throw(gd_nm_node_t node);

private:
    void * operator new (size_t size);
};

}}

#endif
