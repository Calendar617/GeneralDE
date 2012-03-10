#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "usf/logic/logic_executor_type.h"
#include "logic_internal_ops.h"

static void logic_executor_type_group_clear(gd_nm_node_t node);

static cpe_hash_string_buf s_logic_executor_type_groupr_default_name = CPE_HS_BUF_MAKE("logic_executor_type_groupr");

struct gd_nm_node_type s_nm_node_type_logic_executor_type_group = {
    "usf_logic_executor_type_group",
    logic_executor_type_group_clear
};

logic_executor_type_group_t
logic_executor_type_group_create(
    gd_app_context_t app,
    const char * name,
    mem_allocrator_t alloc)
{
    logic_executor_type_group_t group;
    gd_nm_node_t group_node;

    if (name == 0) name = cpe_hs_data((cpe_hash_string_t)&s_logic_executor_type_groupr_default_name);

    group_node = gd_nm_instance_create(gd_app_nm_mgr(app), name, sizeof(struct logic_executor_type_group));
    if (group_node == NULL) return NULL;

    group = (logic_executor_type_group_t)gd_nm_node_data(group_node);
    group->m_alloc = alloc;
    group->m_app = app;
    gd_nm_node_set_type(group_node, &s_nm_node_type_logic_executor_type_group);

    return group;
}

static void logic_executor_type_group_clear(gd_nm_node_t node) {
    logic_executor_type_group_t group;
    group = (logic_executor_type_group_t)gd_nm_node_data(node);
}

void logic_executor_type_group_free(logic_executor_type_group_t group) {
    gd_nm_node_t group_node;
    assert(group);

    group_node = gd_nm_node_from_data(group);
    if (gd_nm_node_type(group_node) != &s_nm_node_type_logic_executor_type_group) return;
    gd_nm_node_free(group_node);
}

logic_executor_type_group_t
logic_executor_type_group_find(
    gd_app_context_t app,
    cpe_hash_string_t name)
{
    gd_nm_node_t node = gd_nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_logic_executor_type_group) return NULL;
    return (logic_executor_type_group_t)gd_nm_node_data(node);
}

logic_executor_type_group_t
logic_executor_type_group_default(
    gd_app_context_t app)
{
    return logic_executor_type_group_find(app, (cpe_hash_string_t)&s_logic_executor_type_groupr_default_name);
}

gd_app_context_t logic_executor_type_group_app(logic_executor_type_group_t group) {
    return group->m_app;
}

const char * logic_executor_type_group_name(logic_executor_type_group_t group) {
    return gd_nm_node_name(gd_nm_node_from_data(group));
}

cpe_hash_string_t
logic_executor_type_group_name_hs(logic_executor_type_group_t group) {
    return gd_nm_node_name_hs(gd_nm_node_from_data(group));
}

EXPORT_DIRECTIVE
int logic_executor_type_groupr_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    logic_executor_type_group_t logic_executor_type_groupr;

    logic_executor_type_groupr = logic_executor_type_group_create(app, gd_app_module_name(module), gd_app_alloc(app));
    if (logic_executor_type_groupr == NULL) return -1;

    return 0;
}

EXPORT_DIRECTIVE
void logic_executor_type_groupr_app_fini(gd_app_context_t app, gd_app_module_t module) {
    logic_executor_type_group_t logic_executor_type_groupr;

    logic_executor_type_groupr = logic_executor_type_group_find(app, gd_app_module_name_hs(module));
    if (logic_executor_type_groupr) {
        logic_executor_type_group_free(logic_executor_type_groupr);
    }
}

