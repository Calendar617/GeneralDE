#ifndef GD_DP_IMPL_INTERNAL_TYPES_H
#define GD_DP_IMPL_INTERNAL_TYPES_H
#include "cpe/pal/queue.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/hash_string.h"
#include "cpe/utils/hash.h"
#include "gd/dp/dp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_dp_mgr {
    mem_allocrator_t m_alloc;
    struct cpe_hash_table m_rsps;
    struct cpe_hash_table m_cmd_2_rsps;
};

typedef enum gd_dp_key_type {
    gd_dp_key_numeric
    , gd_dp_key_string
} gd_dp_key_type_t;

struct gd_dp_binding {
    struct cpe_hash_entry m_hh;
    gd_dp_rsp_t m_rsp;
    gd_dp_key_type_t m_kt;

    /*make a list to rsp*/
    struct gd_dp_binding * m_binding_next;
    struct gd_dp_binding ** m_binding_pre;
};

struct gd_dp_binding_numeric {
    struct gd_dp_binding m_head;
    int32_t m_value;
};

struct gd_dp_binding_string {
    struct gd_dp_binding m_head;
    const char * m_value;
    int m_value_len;
};

struct gd_dp_rsp {
    gd_dp_mgr_t m_dp;
    const char * m_name;
    size_t m_name_len;
    struct gd_dp_binding * m_bindings;
    struct cpe_hash_entry m_hh;
};

typedef TAILQ_HEAD(gd_dp_rsp_list, gd_dp_req) gd_dp_rsp_list_t;

struct gd_dp_req {
    gd_dp_mgr_t m_mgr;
    mem_allocrator_t m_talloc;
    cpe_hash_string_t m_type;
    gd_dp_req_t m_parent;
    void * m_data;
    size_t m_data_capacity;
    size_t m_data_size;

    gd_dp_rsp_list_t m_childs;
    TAILQ_ENTRY(gd_dp_req) m_brother;
};

#ifdef __cplusplus
}
#endif

#endif
