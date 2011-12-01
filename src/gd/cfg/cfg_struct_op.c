#include <assert.h>
#include <string.h>
#include "gd/cfg/cfg_manage.h"
#include "cfg_internal_types.h"
#include "cfg_struct_op.h"

static int gd_cfg_struct_item_cmp(struct gd_cfg_struct_item * l, struct gd_cfg_struct_item * r) {
    return strcmp(l->m_name, r->m_name);
}

RB_PROTOTYPE(gd_cfg_struct_item_tree, gd_cfg_struct_item, m_linkage, gd_cfg_struct_item_cmp);
RB_GENERATE(gd_cfg_struct_item_tree, gd_cfg_struct_item, m_linkage, gd_cfg_struct_item_cmp);

void gd_cfg_struct_init(struct gd_cfg_struct * s) {
    RB_INIT(&s->m_items);
}

void gd_cfg_struct_free_items(struct gd_cfg_struct * cfg) {
    while(!RB_EMPTY(&cfg->m_items)) {
        gd_cfg_struct_item_free(RB_ROOT(&cfg->m_items));
    }
}

gd_cfg_t gd_cfg_struct_item_create(struct gd_cfg_struct * s, const char * name, int type, size_t capacity) {
    assert(s);
    assert(s->m_manage);
    assert(name);

    int nameLen;
    int allocSize;
    void * data;
    struct gd_cfg_struct_item * item;

    if (s->m_type != CPE_DR_TYPE_STRUCT) return NULL;

    nameLen = strlen(name) + 1;
    allocSize = nameLen + sizeof(struct gd_cfg_struct_item) + capacity;

    data = mem_alloc(s->m_manage->m_alloc, allocSize);
    memcpy(data, name, nameLen);

    item = (struct gd_cfg_struct_item *)(((char*)data) + nameLen);

    item->m_data.m_manage = s->m_manage;
    item->m_data.m_parent = (gd_cfg_t)s;
    item->m_data.m_type = type;
    item->m_name = data;

    RB_INSERT(gd_cfg_struct_item_tree, &s->m_items, item);

    return &item->m_data;
}

void gd_cfg_struct_item_free(struct gd_cfg_struct_item * item) {
    assert(item);
    assert(item->m_data.m_parent);
    assert(item->m_data.m_manage);

    RB_REMOVE(
        gd_cfg_struct_item_tree,
        &((struct gd_cfg_struct *)item->m_data.m_parent)->m_items,
        item);

    if (item->m_data.m_type == CPE_DR_TYPE_STRUCT) {
        gd_cfg_struct_free_items((struct gd_cfg_struct *)(&item->m_data));
    }

     /*name is the alloc start adress, see gd_cfg_struct_add_item*/
    mem_free(item->m_data.m_manage->m_alloc, item->m_name);
}
