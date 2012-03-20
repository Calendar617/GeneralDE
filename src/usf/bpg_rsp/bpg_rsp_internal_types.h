#ifndef USF_BPG_RSP_INTERNAL_TYPES_H
#define USF_BPG_RSP_INTERNAL_TYPES_H
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/hash.h"
#include "cpe/utils/hash_string.h"
#include "usf/dr_store/dr_store_types.h"
#include "usf/bpg_rsp/bpg_rsp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bpg_rsp_manage {
    gd_app_context_t m_app;
    mem_allocrator_t m_alloc;
    logic_manage_t m_logic_mgr;
    error_monitor_t m_em;
    uint32_t m_flags;

    size_t m_ctx_capacity;
    bpg_logic_ctx_init_fun_t m_ctx_init;
    bpg_logic_ctx_fini_fun_t m_ctx_fini;
    void * m_ctx_ctx;

    size_t m_rsp_max_size;
    bpg_pkg_t m_rsp_buf;

    int m_debug;

    cpe_hash_string_t m_send_to;
};

struct bpg_rsp_copy_info {
    TAILQ_ENTRY(bpg_rsp_copy_info) m_next;
};

typedef TAILQ_HEAD(bpg_rsp_copy_info_list, bpg_rsp_copy_info) bpg_rsp_copy_info_list_t;

struct bpg_rsp {
    bpg_rsp_manage_t m_mgr;
    logic_executor_t m_executor;
    uint32_t m_flags;

    bpg_rsp_copy_info_list_t m_ctx_to_pdu;
};

#ifdef __cplusplus
}
#endif

#endif
