#ifndef USF_BPG_INTERNAL_TYPES_H
#define USF_BPG_INTERNAL_TYPES_H
#include "cpe/utils/memory.h"
#include "cpe/utils/hash.h"
#include "cpe/utils/hash_string.h"
#include "usf/bpg/bpg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bpg_manage {
    gd_app_context_t m_app;
    logic_manage_t m_logic_mgr;
    error_monitor_t m_em;
    uint32_t m_flags;

    size_t m_ctx_capacity;
    bpg_logic_ctx_init_fun_t m_ctx_init;
    bpg_logic_ctx_fini_fun_t m_ctx_fini;
    void * m_ctx_ctx;
};

struct bpg_rsp {
    bpg_manage_t m_mgr;
    logic_executor_t m_executor;
    uint32_t m_flags;
};

#ifdef __cplusplus
}
#endif

#endif
