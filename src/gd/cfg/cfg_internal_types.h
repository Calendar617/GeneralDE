#ifndef GD_CFG_IMPL_INTERNAL_TYPES_H
#define GD_CFG_IMPL_INTERNAL_TYPES_H
#include "cpe/pal/tree.h"
#include "cpe/utils/memory.h"
#include "gd/cfg/cfg_types.h"

typedef struct gd_cfg_manage * gd_cfg_manage_t;

#define GD_CFG_HEAD_DATA           \
    gd_cfg_manage_t m_manage;      \
    int m_type;                    \
    gd_cfg_t m_parent;

struct gd_cfg {
    GD_CFG_HEAD_DATA
};

struct gd_cfg_struct_item {
    RB_ENTRY(gd_cfg_struct_item) m_linkage;
    char * m_name;
    struct gd_cfg m_data;
};

RB_HEAD(gd_cfg_struct_item_tree, gd_cfg_struct_item);

struct gd_cfg_struct {
    GD_CFG_HEAD_DATA
    struct gd_cfg_struct_item_tree m_items;
};

#define GD_CFG_SEQ_BLOCK_ITEM_COUNT 64

struct gd_cfg_seq_block {
    struct gd_cfg_seq_block * m_next;
    gd_cfg_t m_items[GD_CFG_SEQ_BLOCK_ITEM_COUNT];
};

struct gd_cfg_seq {
    GD_CFG_HEAD_DATA
    int m_count;
    struct gd_cfg_seq_block m_block_head; 
};

struct gd_cfg_manage {
    mem_allocrator_t m_alloc;
    struct gd_cfg_struct m_root;
};

#endif
