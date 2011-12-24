#ifndef GDPP_NM_GROUP_H
#define GDPP_NM_GRUOP_H
#include "Object.hpp"

namespace Gd { namespace Nm {

class Group : public Object {
public:
    virtual ~Group() = 0;

    ObjectIterator members(void);
    ConstObjectIterator members(void) const;

    int memberCount(void) { return gd_nm_group_member_count(*this); }
    void destoryMembers(void);
    void addMember(Object & object);

    Object * findMember(cpe_hash_string_t name) {
        return (Object *)gd_nm_group_find_member(*this, name);
    }

    Object const * findMember(cpe_hash_string_t name) const {
        return (Object *)gd_nm_group_find_member(*this, name);
    }

    Object const & member(cpe_hash_string_t name) const;
    Object & member(cpe_hash_string_t name);

    void * operator new (size_t size, gd_nm_mgr_t nmm, const char * name);
    void * operator new (size_t size, Manager & nmm, const char * name);
};

}}

#endif
