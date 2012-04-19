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
int dr_inbuild_metalib_add_macro_to_index(struct DRInBuildMetaLib * inBuildMetaLib, struct DRInBuildMacro * macro);
struct DRInBuildMacro * dr_inbuild_metalib_find_macro(struct DRInBuildMetaLib * inBuildMetaLib, const char * macro_name);

struct DRInBuildMeta * dr_inbuild_metalib_add_meta(struct DRInBuildMetaLib * inBuildMetaLib);
void dr_inbuild_metalib_remove_meta(struct DRInBuildMetaLib * inBuildMetaLib, struct DRInBuildMeta * meta);

struct DRInBuildMetaEntry * dr_inbuild_meta_add_entry(struct DRInBuildMeta * meta);
void dr_inbuild_meta_remove_entry(struct DRInBuildMeta * meta, struct DRInBuildMetaEntry * entry);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


