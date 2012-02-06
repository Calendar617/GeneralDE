#include <assert.h>
#include "cpe/cfg/cfg_manage.h"
#include "cpe/cfg/cfg_read.h"
#include "cfg_internal_types.h"

static void cfg_merge_i(cfg_t cfg, cfg_t input, cfg_policy_t policy, error_monitor_t em) {
    if (cfg->m_type == CPE_CFG_TYPE_STRUCT) {
        if (input->m_type == CPE_CFG_TYPE_STRUCT) {
            struct cfg_it it;
            cfg_t child;

            cfg_it_init(&it, input);
            while((child = cfg_it_next(&it))) {
                
            }
        }
        else {
        }
    }
    else if (cfg->m_type == CPE_CFG_TYPE_SEQUENCE) {
    }
    else {
    }
}

int cfg_merge(cfg_t cfg, cfg_t input, cfg_policy_t policy, error_monitor_t em) {
    assert(cfg);
    if (input == NULL) return 0;

    cfg_merge_i(cfg, input, policy, em);

    return 0;
}
