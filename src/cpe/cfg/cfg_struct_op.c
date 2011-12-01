#include <assert.h>
#include <string.h>
#include "cpe/cfg/cfg_manage.h"
#include "cfg_internal_types.h"
#include "cfg_internal_ops.h"

int gd_cfg_struct_item_cmp(struct gd_cfg_struct_item * l, struct gd_cfg_struct_item * r) {
    return strcmp(l->m_name, r->m_name);
}

RB_GENERATE(gd_cfg_struct_item_tree, gd_cfg_struct_item, m_linkage, gd_cfg_struct_item_cmp);

void gd_cfg_struct_init(struct gd_cfg_struct * s) {
    RB_INIT(&s->m_items);
}

void gd_cfg_struct_fini(struct gd_cfg_struct * cfg) {
    while(!RB_EMPTY(&cfg->m_items)) {
        gd_cfg_struct_item_delete(cfg, &(RB_ROOT(&cfg->m_items)->m_data));
    }
}

gd_cfg_t gd_cfg_struct_find_cfg(gd_cfg_t cfg, const char * name) {
    struct gd_cfg_struct_item tmp;
    struct gd_cfg_struct_item * found;

    assert(cfg);
    assert(name);

    if (cfg->m_type != CPE_CFG_TYPE_STRUCT) return NULL;

    tmp.m_name = name;
    found = RB_FIND(gd_cfg_struct_item_tree, &((struct gd_cfg_struct *)cfg)->m_items, &tmp);
    return found ? &found->m_data : NULL;
}

void gd_cfg_struct_item_delete(struct gd_cfg_struct * s, gd_cfg_t cfg) {
    struct gd_cfg_struct_item * item;

    assert(s);
    assert(s->m_manage);
    assert(cfg);

    item = gd_cfg_to_struct_item(cfg);

    RB_REMOVE(gd_cfg_struct_item_tree, &s->m_items, item);

    gd_cfg_fini(&item->m_data);

    /*name is the alloc start adress, see gd_cfg_struct_add_item*/
    mem_free(item->m_data.m_manage->m_alloc, (void*)item->m_name);
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
