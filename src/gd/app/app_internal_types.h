#ifndef GD_APP_SVR_CONTEXT_H
#define GD_APP_SVR_CONTEXT_H
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/error.h"
#include "cpe/utils/hash_string.h"
#include "gd/app/app_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GD_APP_MAX_ARGV 128

struct gd_app_lib;
typedef TAILQ_HEAD(gd_app_lib_list, gd_app_lib) gd_app_lib_list_t;

struct gd_app_module;
typedef TAILQ_HEAD(gd_app_module_list, gd_app_module) gd_app_module_list_t;

struct gd_app_runing_module;
typedef TAILQ_HEAD(gd_app_runing_module_list, gd_app_runing_module) gd_app_runing_module_list_t;

struct gd_app_context {
    gd_app_status_t m_state;
    int m_argc;
    char * m_argv[GD_APP_MAX_ARGV];
    mem_allocrator_t m_alloc;
    size_t m_capacity;

    error_monitor_t m_em;
    struct error_monitor m_em_print;

    char * m_root;
    cfg_t m_cfg;
    gd_tl_manage_t m_tl_mgr;
    gd_dp_mgr_t m_dp_mgr;
    gd_nm_mgr_t m_nm_mgr;
    gd_app_main m_main;

    gd_app_runing_module_list_t m_runing_modules;
};

struct gd_app_module {
    cpe_hash_string_t m_name;
    struct gd_app_lib * m_lib;
    gd_app_module_global_init m_global_init;
    gd_app_module_global_fini m_global_fini;
    gd_app_module_app_init m_app_init;
    gd_app_module_app_fini m_app_fini;

    TAILQ_ENTRY(gd_app_module) m_next;
    TAILQ_ENTRY(gd_app_module) m_qh_for_lib;

    gd_app_runing_module_list_t m_runing_modules;
};

extern cpe_hash_string_t gd_app_module_root_group_name;

#ifdef __cplusplus
}
#endif

#endif
