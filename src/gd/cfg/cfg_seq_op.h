#ifndef GD_CFG_IMPL_SEQUENCEOP_H
#define GD_CFG_IMPL_SEQUENCEOP_H
#include "cfg_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_cfg_t gd_cfg_seq_item_create(struct gd_cfg_struct * s, const char * name, int type, size_t capacity);
void gd_cfg_seq_free_items(struct gd_cfg_struct * s);
void gd_cfg_seq_init(struct gd_cfg_seq * s);

#ifdef __cplusplus
}
#endif

#endif
