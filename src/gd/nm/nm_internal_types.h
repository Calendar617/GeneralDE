#ifndef GD_NM_INTERNAL_TYPES_H
#define GD_NM_INTERNAL_TYPES_H
#include "cpe/pal/queue.h"
#include "cpe/utils/hash.h"
#include "cpe/utils/buffer.h"
#include "cpe/utils/hash_string.h"
#include "cpe/utils/memory.h"
#include "gd/nm/nm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_nm_group;
struct gd_nm_binding;
typedef TAILQ_HEAD(gd_nm_binding_list, gd_nm_binding) gd_nm_binding_list_t;

struct gd_nm_mgr {
    mem_allocrator_t m_alloc;
    struct cpe_hash_table m_nodes;

    struct mem_buffer m_binding_buffer;
    gd_nm_binding_list_t m_binding_free_list;
};

#define GD_NM_NODE_HEAD()                       \
    gd_nm_mgr_t m_mgr;                          \
    gd_nm_node_category_t m_category;           \
    gd_nm_node_type_t m_type;                   \
    cpe_hash_string_t m_name;                   \
    size_t m_data_capacity;                     \
                                                \
    struct cpe_hash_entry m_hh_for_mgr;         \
    gd_nm_binding_list_t m_to_group_bindings

struct gd_nm_node {
    GD_NM_NODE_HEAD();
};

struct gd_nm_binding {
    struct gd_nm_group * m_group;
    gd_nm_node_t m_node;

    struct cpe_hash_entry m_hh_for_group;
    TAILQ_ENTRY(gd_nm_binding) m_qh; /*for free list or node*/
};

struct gd_nm_group {
    GD_NM_NODE_HEAD();
    struct cpe_hash_table m_members;
};

struct gd_nm_instance {
    GD_NM_NODE_HEAD();
};

struct gd_nm_node_groups_it {
    gd_nm_it_next_fun m_next_fun;
    struct gd_nm_binding * m_curent;
};
extern char check_node_groups_it_size[sizeof(struct gd_nm_node_it) - sizeof(struct gd_nm_node_groups_it)];

struct gd_nm_node_in_mgr_it {
    gd_nm_it_next_fun m_next_fun;
    struct cpe_hash_it m_hash_it;
};
extern char check_node_in_mgr_it_size[sizeof(struct gd_nm_node_it) - sizeof(struct gd_nm_node_in_mgr_it)];

struct gd_nm_node_in_group_it {
    gd_nm_it_next_fun m_next_fun;
    struct cpe_hash_it m_hash_it;
};
extern char check_node_in_group_it_size[sizeof(struct gd_nm_node_it) - sizeof(struct gd_nm_node_in_group_it)];

#ifdef __cplusplus
}
#endif

#endif
