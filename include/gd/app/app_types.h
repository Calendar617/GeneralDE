#ifndef GD_APP_TYPES_H
#define GD_APP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gd_app_context * gd_app_context_t;

typedef int (*gd_app_main)(gd_app_context_t ctx);

#ifdef __cplusplus
}
#endif

#endif
