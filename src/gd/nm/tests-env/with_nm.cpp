#include <sstream>
#include "gd/nm/tests-env/with_nm.hpp"

namespace gd { namespace nm { namespace testenv {

with_nm::with_nm()
    : m_nm(NULL)
{
}

void with_nm::SetUp() {
    m_nm = gd_nm_mgr_create(t_allocrator());
}


void with_nm::TearDown() {
    gd_nm_mgr_free(m_nm);
    m_nm = NULL;
}

gd_nm_mgr_t with_nm::t_nm() {
    return m_nm;
}

gd_nm_node_t
with_nm::t_nm_add_instance(const char * name, size_t capacity) {
    return gd_nm_instance_create(
        t_nm(),
        cpe_hs_create(t_tmp_allocrator(), name),
        capacity);
}

gd_nm_node_t
with_nm::t_nm_add_group(const char * name, size_t capacity) {
    return gd_nm_group_create(
        t_nm(),
        cpe_hs_create(t_tmp_allocrator(), name),
        capacity);
}

gd_nm_node_t with_nm::t_nm_find(const char * name) {
    return gd_nm_node_find(t_nm(), cpe_hs_create(t_tmp_allocrator(), name));
}

int with_nm::t_nm_bind(const char * groupName, const char * instanceName) {
    return gd_nm_group_add_member(
        t_nm_find(groupName),
        t_nm_find(instanceName));
}

::std::string
with_nm::t_nm_it_dump(gd_nm_node_it_t it) {
    ::std::ostringstream os;
    while(gd_nm_node_t node = gd_nm_node_next(it)) {
        os << gd_nm_node_name(node) << ":";
    }

    return os.str();
}

::std::string
with_nm::t_nm_groups_of_node(const char * nodeName) {
    struct gd_nm_node_it it;
    gd_nm_node_groups(&it, t_nm_find(nodeName));
    return t_nm_it_dump(&it);
}

}}}

