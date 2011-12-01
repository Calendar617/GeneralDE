#ifndef CPE_CFG_IMPL_INTERNAL_OP_H
#define CPE_CFG_IMPL_INTERNAL_OP_H
#include "cfg_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void gd_cfg_fini(gd_cfg_t cfg);

/*struct operations*/
gd_cfg_t gd_cfg_struct_item_create(struct gd_cfg_struct * s, const char * name, int type, size_t capacity);
void gd_cfg_struct_item_delete(struct gd_cfg_struct * s, gd_cfg_t cfg);

void gd_cfg_struct_fini(struct gd_cfg_struct * s);
void gd_cfg_struct_init(struct gd_cfg_struct * s);

#define gd_cfg_to_struct_item(__dp) \
    ((struct gd_cfg_struct_item *)                                      \
     (((char*)__dp) - (sizeof(struct gd_cfg_struct_item) - sizeof(struct gd_cfg))))

RB_PROTOTYPE(gd_cfg_struct_item_tree, gd_cfg_struct_item, m_linkage, gd_cfg_struct_item_cmp);

/*sequence operations*/
gd_cfg_t gd_cfg_seq_item_create(struct gd_cfg_seq * s, int type, size_t capacity);
void gd_cfg_seq_item_delete(struct gd_cfg_seq * s, gd_cfg_t cfg);
void gd_cfg_seq_init(struct gd_cfg_seq * s);
void gd_cfg_seq_fini(struct gd_cfg_seq * s);

#ifdef __cplusplus
}
#endif

#endif
