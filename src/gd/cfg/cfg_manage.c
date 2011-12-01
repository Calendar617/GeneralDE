#include <assert.h>
#include <strings.h>
#include "gd/cfg/cfg_manage.h"
#include "cfg_internal_types.h"
#include "cfg_struct_op.h"

gd_cfg_t gd_cfg_create(mem_allocrator_t alloc) {
    gd_cfg_manage_t cm;
    size_t allocSize = sizeof(struct gd_cfg_manage);

    cm = (gd_cfg_manage_t)mem_alloc(alloc, allocSize);
    bzero(cm, allocSize);

    cm->m_alloc = alloc;
    cm->m_root.m_type = CPE_DR_TYPE_STRUCT;
    cm->m_root.m_manage = cm;
    cm->m_root.m_parent = NULL;

    gd_cfg_struct_init(&cm->m_root);

    return (gd_cfg_t)&cm->m_root;
}

void gd_cfg_free(gd_cfg_t dp) {
    if (dp == NULL) return;

    assert(dp->m_manage);

    if ((gd_cfg_t)&dp->m_manage->m_root == dp) {
        gd_cfg_struct_free_items(&dp->m_manage->m_root);
        mem_free(
            dp->m_manage->m_alloc,
            ((char*)dp) - (sizeof(struct gd_cfg_manage) - sizeof(struct gd_cfg_struct)));
    }
    else {
        gd_cfg_struct_item_free(gd_cfg_to_struct_item(dp));
    }
}

gd_cfg_t gd_cfg_struct_add_struct(gd_cfg_t s, const char * name) {
    gd_cfg_t rv = gd_cfg_struct_item_create((struct gd_cfg_struct *)s, name, CPE_DR_TYPE_STRUCT, sizeof(struct gd_cfg_struct));
    if (rv == NULL) return NULL;
    gd_cfg_struct_init((struct gd_cfg_struct *)rv);
    return rv;
}
