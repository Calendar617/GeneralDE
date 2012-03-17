#ifndef USF_DR_STORE_INTERNAL_TYPES_H
#define USF_DR_STORE_INTERNAL_TYPES_H
#include "cpe/utils/memory.h"
#include "cpe/utils/hash.h"
#include "cpe/utils/hash_string.h"
#include "usf/dr_store/dr_store_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct dr_store_manage {
    mem_allocrator_t m_alloc;
    gd_app_context_t m_app;
    mem_allocrator_t m_allc;
    error_monitor_t m_em;

    int m_debug;

    struct cpe_hash_table m_stores;
};

struct dr_store {
    dr_store_manage_t m_mgr;
    const char * m_name; 
    LPDRMETALIB m_lib;
    dr_lib_free_fun_t m_free_fun;
    void * m_free_ctx;

    struct cpe_hash_entry m_hh;
};

#ifdef __cplusplus
}
#endif

#endif
