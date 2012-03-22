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

typedef TAILQ_HEAD(bpg_rsp_pkg_builder_list, bpg_rsp_pkg_builder) * bpg_rsp_pkg_builder_list_t;

struct bpg_rsp_manage {
    gd_app_context_t m_app;
    mem_allocrator_t m_alloc;
    logic_manage_t m_logic_mgr;
    error_monitor_t m_em;
    uint32_t m_flags;

    size_t m_ctx_capacity;
    bpg_logic_ctx_init_fun_t m_ctx_init;
    bpg_logic_ctx_fini_fun_t m_ctx_fini;
    bpg_logic_pkg_init_fun_t m_pkg_init;
    void * m_ctx_ctx;

    size_t m_rsp_max_size;
    bpg_pkg_t m_rsp_buf;

    struct bpg_rsp_pkg_builder_list m_pkg_builders;

    int m_debug;

    cpe_hash_string_t m_commit_to;
};

struct bpg_rsp_copy_info {
    TAILQ_ENTRY(bpg_rsp_copy_info) m_next;
};

typedef TAILQ_HEAD(bpg_rsp_copy_info_list, bpg_rsp_copy_info) * bpg_rsp_copy_info_list_t;

struct bpg_rsp {
    bpg_rsp_manage_t m_mgr;
    logic_executor_t m_executor;
    uint32_t m_flags;

    struct bpg_rsp_copy_info_list m_ctx_to_pdu;
};

struct bpg_rsp_pkg_builder {
    bpg_rsp_manage_t m_mgr;
    bpg_pkg_build_fun_t m_build_fun;
    void * m_build_ctx;

    TAILQ_ENTRY(bpg_rsp_pkg_builder) m_next;
};

#ifdef __cplusplus
}
#endif

#endif
