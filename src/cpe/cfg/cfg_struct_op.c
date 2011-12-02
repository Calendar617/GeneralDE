#include <assert.h>
#include <string.h>
#include "cpe/cfg/cfg_manage.h"
#include "cfg_internal_types.h"
#include "cfg_internal_ops.h"

int cfg_struct_item_cmp(struct cfg_struct_item * l, struct cfg_struct_item * r) {
    return strcmp(l->m_name, r->m_name);
}

RB_GENERATE(cfg_struct_item_tree, cfg_struct_item, m_linkage, cfg_struct_item_cmp);

void cfg_struct_init(struct cfg_struct * s) {
    RB_INIT(&s->m_items);
}

void cfg_struct_fini(struct cfg_struct * cfg) {
    while(!RB_EMPTY(&cfg->m_items)) {
        cfg_struct_item_delete(cfg, &(RB_ROOT(&cfg->m_items)->m_data));
    }
}

cfg_t cfg_struct_find_cfg(cfg_t cfg, const char * name) {
    struct cfg_struct_item tmp;
    struct cfg_struct_item * found;

    assert(cfg);
    assert(name);

    if (cfg->m_type != CPE_CFG_TYPE_STRUCT) return NULL;

    tmp.m_name = name;
    found = RB_FIND(cfg_struct_item_tree, &((struct cfg_struct *)cfg)->m_items, &tmp);
    return found ? &found->m_data : NULL;
}

void cfg_struct_item_delete(struct cfg_struct * s, cfg_t cfg) {
    struct cfg_struct_item * item;

    assert(s);
    assert(s->m_manage);
    assert(cfg);

    item = cfg_to_struct_item(cfg);

    RB_REMOVE(cfg_struct_item_tree, &s->m_items, item);

    cfg_fini(&item->m_data);

    /*name is the alloc start adress, see cfg_struct_add_item*/
    mem_free(item->m_data.m_manage->m_alloc, (void*)item->m_name);
}

cfg_t cfg_struct_item_create(struct cfg_struct * s, const char * name, int type, size_t capacity) {
    assert(s);
    assert(s->m_manage);
    assert(name);

    int nameLen;
    int allocSize;
    void * data;
    struct cfg_struct_item * item;

    if (s->m_type != CPE_DR_TYPE_STRUCT) return NULL;

    nameLen = strlen(name) + 1;
    allocSize = nameLen + sizeof(struct cfg_struct_item) + capacity;

    data = mem_alloc(s->m_manage->m_alloc, allocSize);
    memcpy(data, name, nameLen);

    item = (struct cfg_struct_item *)(((char*)data) + nameLen);

    item->m_data.m_manage = s->m_manage;
    item->m_data.m_parent = (cfg_t)s;
    item->m_data.m_type = type;
    item->m_name = data;

    if (RB_INSERT(cfg_struct_item_tree, &s->m_items, item) != NULL) {
        mem_free(item->m_data.m_manage->m_alloc, data);
        return NULL;
    }
    else {
        return &item->m_data;
    }
}
