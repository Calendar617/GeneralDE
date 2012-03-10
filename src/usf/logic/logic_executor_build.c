#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "cpe/cfg/cfg_read.h"
#include "usf/logic/logic_executor_build.h"
#include "usf/logic/logic_executor.h"

logic_executor_t
logic_executor_build(
    logic_manage_t mgr,
    cfg_t cfg,
    logic_executor_build_fun_t build_fun,
    void * build_ctx,
    error_monitor_t em)
{
    assert(mgr);

    if (cfg == NULL) {
        CPE_ERROR(em, "not support create logic_executor from null cfg node");
        return NULL;
    }

    if (cfg_type(cfg) == CPE_CFG_TYPE_STRUCT) {
        cfg_t child;
        logic_executor_t executor;

        child = cfg_child_only(cfg);
        if (child == 0) {
            CPE_ERROR(em, "not support create logic_executor from struct node");
            return NULL;
        }

        executor = build_fun(mgr, cfg_name(child), build_ctx, child, em);
        if (executor) return executor;

        
        if (strcmp(cfg_name(child), "protect") == 0) {
            logic_executor_t inner;
            logic_executor_t protect;
            inner = logic_executor_build(mgr, child, build_fun, build_ctx, em);
            if (inner == NULL) return NULL;

            protect = logic_executor_protect_create(mgr, "protect", inner);
            if (protect == NULL) logic_executor_free(inner);
            return protect;
        }

        CPE_ERROR(em, "not support logic_executor %s", cfg_name(child));
        return NULL;
    }
    else if (cfg_type(cfg) == CPE_CFG_TYPE_SEQUENCE) {
        struct cfg_it childIt;
        cfg_t child;
        logic_executor_t group = logic_executor_group_create(mgr, "group");
        if (group == NULL) {
            CPE_ERROR(em, "create logic_executor_group fail!");
            return NULL;
        }

        cfg_it_init(&childIt, cfg);
        while((child = cfg_it_next(&childIt))) {
            logic_executor_t member =
                logic_executor_build(mgr, child, build_fun, build_ctx, em);
            if (member == NULL) {
                logic_executor_free(group);
                return NULL;
            }

            logic_executor_group_add(group, member);
        }

        return group;
    }
    else if (cfg_type(cfg) == CPE_CFG_TYPE_STRING) {
        return build_fun(mgr, cfg_as_string(cfg, NULL), build_ctx, NULL, em);
    }
    else {
        CPE_ERROR(em, "not support create logic_executor from cfg type %d!", cfg_type(cfg));
    }

    return NULL;
}

