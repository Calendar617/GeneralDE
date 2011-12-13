#ifndef GD_NM_INTERNAL_TYPES_H
#define GD_NM_INTERNAL_TYPES_H
#include "cpe/pal/queue.h"
#include "cpe/utils/hash.h"
#include "cpe/utils/memory.h"
#include "gd/nm/nm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_nm_group;
struct gd_nm_binding;

struct gd_nm_mgr {
    mem_allocrator_t m_alloc;
    size_t m_maxGroupCount;
};

typedef TAILQ_HEAD(gd_nm_binding_list, gd_nm_binding) gd_nm_binding_list_t;

#define GD_NM_NODE_HEAD()                       \
    gd_nm_mgr_t m_mgr;                          \
    gd_nm_node_type_t m_type;                   \
    cpe_hash_string_t m_name;                   \
    size_t m_data_capacity;                     \
                                                \
    struct cpe_hash_entry m_hh_for_mgr;                     \
    gd_nm_binding_list_t m_groups

struct gd_nm_node {
    GD_NM_NODE_HEAD();
};

struct gd_nm_binding {
    struct gd_nm_group * m_group;
    gd_nm_node_t m_node;

    struct cpe_hash_entry m_hh_for_group;
    TAILQ_ENTRY(gd_nm_binding) m_qh_for_node;
};

struct gd_nm_group {
    GD_NM_NODE_HEAD();
    struct cpe_hash_table m_subs;
};

struct gd_nm_instance {
    GD_NM_NODE_HEAD();
    struct gd_nm_node m_node;
};

#ifdef __cplusplus
}
#endif

#endif
