#ifndef GD_APP_TYPES_H
#define GD_APP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum gd_app_status {
    gd_app_init,
    gd_app_runing,
    gd_app_shutingdown
} gd_app_status_t;

typedef struct gd_app_context * gd_app_context_t;

typedef int (*gd_app_main)(gd_app_context_t ctx);

#ifdef __cplusplus
}
#endif

#endif
