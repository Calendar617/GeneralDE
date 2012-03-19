#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "usf/room/room_manage.h"
#include "room_internal_ops.h"

static void usf_room_manage_clear(gd_nm_node_t node);

static cpe_hash_string_buf s_usf_room_manage_default_name = CPE_HS_BUF_MAKE("usf_room_manage");

struct gd_nm_node_type s_nm_node_type_usf_room_manage = {
    "usf_room_manage",
    usf_room_manage_clear
};

usf_room_manage_t
usf_room_manage_create(
    gd_app_context_t app,
    const char * name,
    mem_allocrator_t alloc)
{
    usf_room_manage_t mgr;
    gd_nm_node_t mgr_node;

    if (name == 0) name = cpe_hs_data((cpe_hash_string_t)&s_usf_room_manage_default_name);

    mgr_node = gd_nm_instance_create(gd_app_nm_mgr(app), name, sizeof(struct usf_room_manage));
    if (mgr_node == NULL) return NULL;

    mgr = (usf_room_manage_t)gd_nm_node_data(mgr_node);
    mgr->m_alloc = alloc;
    mgr->m_app = app;

    if (cpe_hash_table_init(
            &mgr->m_room_types,
            alloc,
            (cpe_hash_fun_t) usf_room_type_hash,
            (cpe_hash_cmp_t) usf_room_type_cmp,
            CPE_HASH_OBJ2ENTRY(usf_room_type, m_hh),
            -1) != 0)
    {
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    if (cpe_hash_table_init(
            &mgr->m_rooms,
            alloc,
            (cpe_hash_fun_t) usf_room_hash,
            (cpe_hash_cmp_t) usf_room_cmp,
            CPE_HASH_OBJ2ENTRY(usf_room, m_hh),
            -1) != 0)
    {
        cpe_hash_table_fini(&mgr->m_room_types);
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    if (cpe_hash_table_init(
            &mgr->m_users,
            alloc,
            (cpe_hash_fun_t) usf_room_user_hash,
            (cpe_hash_cmp_t) usf_room_user_cmp,
            CPE_HASH_OBJ2ENTRY(usf_room_user, m_hh),
            -1) != 0)
    {
        cpe_hash_table_fini(&mgr->m_room_types);
        cpe_hash_table_fini(&mgr->m_rooms);
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    if (cpe_hash_table_init(
            &mgr->m_monitors,
            alloc,
            (cpe_hash_fun_t) usf_room_monitor_hash,
            (cpe_hash_cmp_t) usf_room_monitor_cmp,
            CPE_HASH_OBJ2ENTRY(usf_room_monitor, m_hh),
            -1) != 0)
    {
        cpe_hash_table_fini(&mgr->m_room_types);
        cpe_hash_table_fini(&mgr->m_rooms);
        cpe_hash_table_fini(&mgr->m_users);
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    gd_nm_node_set_type(mgr_node, &s_nm_node_type_usf_room_manage);

    return mgr;
}

static void usf_room_manage_clear(gd_nm_node_t node) {
    usf_room_manage_t mgr;
    mgr = (usf_room_manage_t)gd_nm_node_data(node);

    usf_room_user_free_all(mgr);
    usf_room_free_all(mgr);
    usf_room_type_free_all(mgr);
    usf_room_monitor_free_all(mgr);

    cpe_hash_table_fini(&mgr->m_room_types);
    cpe_hash_table_fini(&mgr->m_rooms);
    cpe_hash_table_fini(&mgr->m_users);
    cpe_hash_table_fini(&mgr->m_monitors);
}

void usf_room_manage_free(usf_room_manage_t mgr) {
    gd_nm_node_t mgr_node;
    assert(mgr);

    mgr_node = gd_nm_node_from_data(mgr);
    if (gd_nm_node_type(mgr_node) != &s_nm_node_type_usf_room_manage) return;
    gd_nm_node_free(mgr_node);
}

usf_room_manage_t
usf_room_manage_find(
    gd_app_context_t app,
    cpe_hash_string_t name)
{
    if (name == NULL) name = (cpe_hash_string_t)&s_usf_room_manage_default_name;

    gd_nm_node_t node = gd_nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_usf_room_manage) return NULL;
    return (usf_room_manage_t)gd_nm_node_data(node);
}

usf_room_manage_t
usf_room_manage_find_nc(
    gd_app_context_t app,
    const char * name)
{
    if (name == NULL) return usf_room_manage_default(app);

    gd_nm_node_t node = gd_nm_mgr_find_node_nc(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_usf_room_manage) return NULL;
    return (usf_room_manage_t)gd_nm_node_data(node);
}

usf_room_manage_t
usf_room_manage_default(
    gd_app_context_t app)
{
    return usf_room_manage_find(app, (cpe_hash_string_t)&s_usf_room_manage_default_name);
}

gd_app_context_t usf_room_manage_app(usf_room_manage_t mgr) {
    return mgr->m_app;
}

const char * usf_room_manage_name(usf_room_manage_t mgr) {
    return gd_nm_node_name(gd_nm_node_from_data(mgr));
}

cpe_hash_string_t
usf_room_manage_name_hs(usf_room_manage_t mgr) {
    return gd_nm_node_name_hs(gd_nm_node_from_data(mgr));
}

EXPORT_DIRECTIVE
int usf_room_manage_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    usf_room_manage_t usf_room_manage;

    usf_room_manage = usf_room_manage_create(app, gd_app_module_name(module), gd_app_alloc(app));
    if (usf_room_manage == NULL) return -1;

    return 0;
}

EXPORT_DIRECTIVE
void usf_room_manage_app_fini(gd_app_context_t app, gd_app_module_t module) {
    usf_room_manage_t usf_room_manage;

    usf_room_manage = usf_room_manage_find_nc(app, gd_app_module_name(module));
    if (usf_room_manage) {
        usf_room_manage_free(usf_room_manage);
    }
}

