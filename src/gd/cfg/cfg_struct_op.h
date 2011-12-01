#ifndef GD_CFG_IMPL_STRUCTOP_H
#define GD_CFG_IMPL_STRUCTOP_H
#include "cfg_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void gd_cfg_struct_item_free(struct gd_cfg_struct_item * item);
gd_cfg_t gd_cfg_struct_item_create(struct gd_cfg_struct * s, const char * name, int type, size_t capacity);

void gd_cfg_struct_free_items(struct gd_cfg_struct * s);

void gd_cfg_struct_init(struct gd_cfg_struct * s);

#define gd_cfg_to_struct_item(__dp) \
    ((struct gd_cfg_struct_item *)                                      \
     ((char*)__dp) - (sizeof(struct gd_cfg_struct_item) - sizeof(struct gd_cfg)))

#ifdef __cplusplus
}
#endif

#endif
