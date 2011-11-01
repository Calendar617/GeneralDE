#ifndef CPE_DR_METALIB_BUILD_H
#define CPE_DR_METALIB_BUILD_H
#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_external.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void (*dr_inbuild_log_fun_t)(void *, const char *, int line, int errno, const char * msg);

struct DRInBuildMetaLib;

struct DRInBuildMetaLib * dr_inbuild_create_lib(void);
void dr_inbuild_free_lib(struct DRInBuildMetaLib * ctx);

int dr_inbuild_build_lib(
    LPDRMETALIB * metaLib,
    struct DRInBuildMetaLib * inBuildMetaLib,
    void * userLogCtx,
    dr_inbuild_log_fun_t errorProcessor);


#ifdef __cplusplus
}
#endif


#endif /* CPE_DR_METALIB_MANAGE_H */
