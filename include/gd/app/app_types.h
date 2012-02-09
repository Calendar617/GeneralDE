#ifndef GD_APP_TYPES_H
#define GD_APP_TYPES_H
#include "gd/dp/dp_types.h"
#include "gd/nm/nm_types.h"
#include "gd/tl/tl_types.h"
#include "cpe/cfg/cfg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum gd_app_status {
    gd_app_init,
    gd_app_runing,
    gd_app_shutingdown
} gd_app_status_t;

typedef struct gd_app_context * gd_app_context_t;
typedef struct gd_app_lib * gd_app_lib_t;
typedef struct gd_app_module * gd_app_module_t;

typedef int (*gd_app_main)(gd_app_context_t ctx);

typedef int (*gd_app_module_global_init)(void);
typedef void (*gd_app_module_global_fini)(void);
typedef int (*gd_app_module_app_init)(gd_app_context_t context, gd_app_module_t module, cfg_t cfg);
typedef void (*gd_app_module_app_fini)(gd_app_context_t context, gd_app_module_t module);

typedef int (*gd_app_rsp_init_fun_t)(
    gd_dp_rsp_t rsp, gd_app_context_t context, gd_app_module_t module, cfg_t cfg);

typedef ptr_int_t (*gd_app_tick_fun)(void * ctx, ptr_int_t arg);

#ifdef __cplusplus
}
#endif

#endif
