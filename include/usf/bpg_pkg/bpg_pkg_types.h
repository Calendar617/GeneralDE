#ifndef USF_BPG_PKG_TYPES_H
#define USF_BPG_PKG_TYPES_H
#include "cpe/utils/error.h"
#include "cpe/utils/buffer.h"
#include "cpe/dr/dr_types.h"
#include "gd/app/app_context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bpg_pkg_manage * bpg_pkg_manage_t;
typedef struct bpg_pkg * bpg_pkg_t;
typedef struct bpg_pkg_append_info * bpg_pkg_append_info_t;

#define BPG_BASEPKG_LIB_NAME "basepkg"
#define BPG_BASEPKG_META_NAME "basepkg"

#ifdef __cplusplus
}
#endif

#endif
