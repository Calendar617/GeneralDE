#ifndef GD_APP_SVR_CONTEXT_H
#define GD_APP_SVR_CONTEXT_H
#include "cpe/utils/memory.h"
#include "gd/app/app_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GD_APP_MAX_ARGV 128

struct gd_app_context {
    int m_argc;
    char * m_argv[GD_APP_MAX_ARGV];
    mem_allocrator_t m_alloc;
    size_t m_capacity;
    error_monitor_t m_em;
    char * m_root;
    cfg_t m_cfg;
    gd_tl_manage_t m_tl_mgr;
    gd_dp_mgr_t m_dp_mgr;
    gd_app_main m_main;
};

#ifdef __cplusplus
}
#endif

#endif

