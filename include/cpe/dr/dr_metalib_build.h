#ifndef CPE_DR_METALIB_BUILD_H
#define CPE_DR_METALIB_BUILD_H
#include "cpe/utils/error.h"
#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_external.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct DRInBuildMetaLib;

struct DRInBuildMetaLib * dr_inbuild_create_lib(void);
void dr_inbuild_free_lib(struct DRInBuildMetaLib * ctx);

int dr_inbuild_build_lib(
    LPDRMETALIB * metaLib,
    struct DRInBuildMetaLib * inBuildMetaLib,
    error_monitor_t er);


#ifdef __cplusplus
}
#endif


#endif /* CPE_DR_METALIB_MANAGE_H */
