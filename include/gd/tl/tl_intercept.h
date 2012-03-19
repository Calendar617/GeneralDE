#ifndef GD_TL_INTERCEPT_H
#define GD_TL_INTERCEPT_H
#include "tl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*intercept operations*/
gd_tl_intercept_t
gd_tl_intercept_create(
    gd_tl_t tl,
    const char * name,
    gd_tl_intercept_fun_t intercept_fun,
    void * intercept_ctx);

void gd_tl_intercept_free(
    gd_tl_intercept_t intercept);

gd_tl_intercept_t
gd_tl_intercept_find(gd_tl_t tl, const char * name);

#ifdef __cplusplus
}
#endif

#endif
