#ifndef CPE_DR_BUILD_INBUILD_H
#define CPE_DR_BUILD_INBUILD_H
#include <stdio.h>
#include "cpe/dr/dr_metalib_build.h"
#include "dr_inbuild_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct DRInBuildMacro * dr_inbuild_metalib_add_macro(struct DRInBuildMetaLib * inBuildMetaLib);
void dr_inbuild_metalib_remove_macro(struct DRInBuildMetaLib * inBuildMetaLib, struct DRInBuildMacro * macro);

struct DRInBuildMeta * dr_inbuild_metalib_add_meta(struct DRInBuildMetaLib * inBuildMetaLib);
void dr_inbuild_metalib_remove_meta(struct DRInBuildMetaLib * inBuildMetaLib, struct DRInBuildMeta * meta);

struct DRInBuildMetaEntry * dr_inbuild_meta_add_entry(struct DRInBuildMeta * meta);
void dr_inbuild_meta_remove_entry(struct DRInBuildMeta * meta, struct DRInBuildMetaEntry * entry);

#define DR_NOTIFY_ERROR(userData, errorProcessor, e) \
    errorProcessor(userData, NULL, 0, e, dr_error_string(e));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


