#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "cpe/cfg/cfg_read.h"
#include "usf/logic/logic_executor_build.h"
#include "usf/logic/logic_executor_type.h"
#include "usf/logic/logic_executor.h"
#include "logic_internal_ops.h"

logic_executor_t
logic_executor_build(
    logic_manage_t mgr,
    cfg_t cfg,
    logic_executor_type_group_t type_group,
    error_monitor_t em)
{
    logic_executor_type_t type;

    assert(mgr);
    assert(type_group);

    if (cfg == NULL) {
        CPE_ERROR(em, "not support create logic_executor from null cfg node");
        return NULL;
    }

    if (cfg_type(cfg) == CPE_CFG_TYPE_STRUCT) {
        cfg_t child;

        child = cfg_child_only(cfg);
        if (child == 0) {
            CPE_ERROR(em, "not support create logic_executor from struct node");
            return NULL;
        }

        type = logic_executor_type_find(type_group, cfg_name(child));
        if (type == 0) {
            CPE_ERROR(em, "not support logic_executor type %s", cfg_name(child));
            return NULL;
        }

        switch(type->m_category) {
        case logic_executor_category_basic: {
            return logic_executor_basic_create(mgr, type, child);
        }
        case logic_executor_category_group: {
            struct cfg_it childIt;
            cfg_t child;
            logic_executor_t group = logic_executor_group_create(mgr, type);
            if (group == NULL) {
                CPE_ERROR(em, "create logic_executor_group fail!");
                return NULL;
            }

            cfg_it_init(&childIt, cfg);
            while((child = cfg_it_next(&childIt))) {
                logic_executor_t member = logic_executor_build(mgr, child, type_group, em);
                if (member == NULL) {
                    logic_executor_free(group);
                    return NULL;
                }

                logic_executor_group_add(group, member);
            }

            return group;
        }
        case logic_executor_category_decorate: {
            logic_executor_t inner;
            logic_executor_t protect;
            inner = logic_executor_build(mgr, child, type_group, em);
            if (inner == NULL) return NULL;

            protect = logic_executor_protect_create(mgr, type, inner);
            if (protect == NULL) logic_executor_free(inner);
            return protect;
        }
        default: {
            CPE_ERROR(em, "%s: not support logic_executor category %d", cfg_name(child), type->m_category);
            return NULL;
        }
        }
    }
    else if (cfg_type(cfg) == CPE_CFG_TYPE_SEQUENCE) {
        struct cfg_it childIt;
        cfg_t child;
        logic_executor_t group;

        type = logic_executor_type_find(type_group, "group");
        if (type == 0) {
            CPE_ERROR(em, "not support logic_executor type group");
            return NULL;
        }

        group = logic_executor_group_create(mgr, type);
        if (group == NULL) {
            CPE_ERROR(em, "create logic_executor_group fail!");
            return NULL;
        }

        cfg_it_init(&childIt, cfg);
        while((child = cfg_it_next(&childIt))) {
            logic_executor_t member =
                logic_executor_build(mgr, child, type_group, em);
            if (member == NULL) {
                logic_executor_free(group);
                return NULL;
            }

            logic_executor_group_add(group, member);
        }

        return group;
    }
    else if (cfg_type(cfg) == CPE_CFG_TYPE_STRING) {
        const char * name = cfg_as_string(cfg, "unknown-string-type-name");
        type = logic_executor_type_find(type_group, name);
        if (type == 0) {
            CPE_ERROR(em, "not support logic_executor type %s", name);
            return NULL;
        }

        if (type->m_category != logic_executor_category_basic) {
            CPE_ERROR(em, "logic_executor type %s is not basic, input format error!", name);
            return NULL;
        }

        return logic_executor_basic_create(mgr, type, NULL);
    }
    else {
        CPE_ERROR(em, "not support create logic_executor from cfg type %d!", cfg_type(cfg));
    }

    return NULL;
}

