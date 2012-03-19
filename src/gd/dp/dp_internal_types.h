#ifndef GD_DP_IMPL_INTERNAL_TYPES_H
#define GD_DP_IMPL_INTERNAL_TYPES_H
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/hash_string.h"
#include "cpe/utils/hash.h"
#include "gd/nm/nm_types.h"
#include "gd/dp/dp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_dp_processing_rsp_buf;
typedef TAILQ_HEAD(gd_dp_processing_rsp_buf_list, gd_dp_processing_rsp_buf) gd_dp_processing_rsp_buf_list_t;

struct gd_dp_mgr {
    mem_allocrator_t m_alloc;
    struct cpe_hash_table m_rsps;
    struct cpe_hash_table m_cmd_2_rsps;
    gd_dp_processing_rsp_buf_list_t m_processiong_rsps;
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
    struct gd_dp_binding * m_rep_binding_next;
    struct gd_dp_binding ** m_rep_binding_pre;

    /*make a list to cmd*/
    struct gd_dp_binding * m_cmd_binding_next;
    struct gd_dp_binding ** m_cmd_binding_pre;
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

    gd_dp_rsp_type_t m_type;
    gd_dp_rsp_process_fun_t m_processor;
    void * m_context;

    struct gd_dp_binding * m_bindings;
    struct cpe_hash_entry m_hh;
};

typedef TAILQ_HEAD(gd_dp_req_list, gd_dp_req) gd_dp_req_list_t;

struct gd_dp_req {
    gd_dp_mgr_t m_mgr;
    mem_allocrator_t m_talloc;
    cpe_hash_string_t m_type;
    gd_dp_req_t m_parent;
    gd_dp_node_t m_from;
    gd_dp_node_t m_to;
    void * m_data;
    size_t m_data_capacity;
    size_t m_data_size;

    gd_dp_req_list_t m_childs;
    TAILQ_ENTRY(gd_dp_req) m_brother;
};

struct gd_dp_node {
    gd_nm_node_t m_nm_node;
    gd_dp_node_type_t m_type;
};

#define PROCESSING_BUF_RSP_COUNT (128)

typedef TAILQ_HEAD(gd_dp_processing_rsp_block_list, gd_dp_processing_rsp_block) gd_dp_processing_rsp_block_list_t;

struct gd_dp_processing_rsp_block {
    size_t m_write_pos;
    size_t m_read_pos;
    gd_dp_rsp_t m_rsps[PROCESSING_BUF_RSP_COUNT];
    TAILQ_ENTRY(gd_dp_processing_rsp_block) m_next;
};

struct gd_dp_processing_rsp_buf {
    mem_allocrator_t m_alloc;
    gd_dp_processing_rsp_block_list_t m_blocks;
    TAILQ_ENTRY(gd_dp_processing_rsp_buf) m_sh_other;
    struct gd_dp_processing_rsp_block m_first_block;
};

#ifdef __cplusplus
}
#endif

#endif