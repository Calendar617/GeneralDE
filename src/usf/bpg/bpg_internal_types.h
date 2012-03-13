#ifndef USF_BPG_INTERNAL_TYPES_H
#define USF_BPG_INTERNAL_TYPES_H
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/hash.h"
#include "cpe/utils/hash_string.h"
#include "usf/bpg/bpg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bpg_manage {
    gd_app_context_t m_app;
    mem_allocrator_t m_alloc;
    LPDRMETALIB m_metalib;
    logic_manage_t m_logic_mgr;
    error_monitor_t m_em;
    uint32_t m_flags;

    char m_request_meta_name[64];
    LPDRMETA m_request_meta;

    char m_response_meta_name[64];
    LPDRMETA m_response_meta;

    size_t m_ctx_capacity;
    bpg_logic_ctx_init_fun_t m_ctx_init;
    bpg_logic_ctx_fini_fun_t m_ctx_fini;
    void * m_ctx_ctx;

    bpg_data_convert_fun_t m_cvt_encode;
    bpg_data_convert_fun_t m_cvt_decode;
    void * m_cvt_ctx;

    size_t m_rsp_size_max;
    bpg_req_t m_rsp_buf;

    cpe_hash_string_t m_send_to;
};

struct bpg_rsp_copy_info {
    TAILQ_ENTRY(bpg_rsp_copy_info) m_next;
};

typedef TAILQ_HEAD(bpg_rsp_copy_info_list, bpg_rsp_copy_info) bpg_rsp_copy_info_list_t;

struct bpg_rsp {
    bpg_manage_t m_mgr;
    logic_executor_t m_executor;
    uint32_t m_flags;

    bpg_rsp_copy_info_list_t m_pdu_to_ctx;
    bpg_rsp_copy_info_list_t m_ctx_to_pdu;
};

struct bpg_req {
    LPDRMETA m_carry_data_meta;
    size_t m_carry_data_size;
    size_t m_carry_data_capacity;
    gd_dp_req_t m_dp_req;
};

#ifdef __cplusplus
}
#endif

#endif
