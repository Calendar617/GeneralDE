#ifndef GDPP_NM_GROUP_H
#define GDPP_NM_GRUOP_H
#include "Object.hpp"

namespace Gd { namespace Nm {

class Group : public Object {
public:
    virtual ~Group() = 0;

    ObjectIterator members(void) {
        ObjectIterator it;
        gd_nm_group_members(&it.m_it, *this);
        return it;
    }

    void * operator new (size_t size, gd_nm_mgr_t nmm, const char * name);
    void * operator new (size_t size, Manager & nmm, const char * name);
};

}}

#endif
