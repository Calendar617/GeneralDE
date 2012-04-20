#ifndef USF_BPG_PKG_INTERNAL_OPS_H
#define USF_BPG_PKG_INTERNAL_OPS_H
#include "bpg_pkg_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t bpg_pkg_debug_info_hash(const struct bpg_pkg_debug_info * o);
int bpg_pkg_debug_info_cmp(const struct bpg_pkg_debug_info * l, const struct bpg_pkg_debug_info * r);
void bpg_pkg_debug_info_free_all(bpg_pkg_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif
