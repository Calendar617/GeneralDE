#include <assert.h>
#include <string.h>
#include "cpe/cfg/cfg_read.h"
#include "cpe/dp/dp_responser.h"
#include "cpe/dp/dp_manage.h"
#include "dp_internal_ops.h"

int dp_binding_add(struct dp_binding * binding, dp_rsp_t rsp) {
    cpe_hash_entry_init(&binding->m_hh);

    if (cpe_hash_table_insert_unique(&rsp->m_dp->m_cmd_2_rsps, binding) != 0) {
        struct dp_binding * bindingPut = (struct dp_binding *)
            cpe_hash_table_find(&rsp->m_dp->m_cmd_2_rsps, binding);
        while(bindingPut->m_cmd_binding_next) {
            bindingPut = bindingPut->m_cmd_binding_next;
        }

        bindingPut->m_cmd_binding_next = binding;
        binding->m_cmd_binding_next = NULL;
        binding->m_cmd_binding_pre = &bindingPut->m_cmd_binding_next;
    }
    else {
        binding->m_cmd_binding_next = NULL;
        binding->m_cmd_binding_pre = NULL;
    }

    binding->m_rsp = rsp;

    binding->m_rep_binding_next = rsp->m_bindings;
    if (binding->m_rep_binding_next)
        binding->m_rep_binding_next->m_rep_binding_pre = &binding->m_rep_binding_next;
    binding->m_rep_binding_pre = &rsp->m_bindings;
    rsp->m_bindings = binding;

    return 0;
}

void dp_binding_free_i(struct dp_binding * binding) {
    dp_mgr_t dp;

    dp = binding->m_rsp->m_dp;

    *binding->m_rep_binding_pre = binding->m_rep_binding_next;
    if (binding->m_rep_binding_next) {
        binding->m_rep_binding_next->m_rep_binding_pre = binding->m_rep_binding_pre;
    }

    if (binding->m_cmd_binding_pre) {
        *binding->m_cmd_binding_pre = binding->m_cmd_binding_next;
        if (binding->m_cmd_binding_next) {
            binding->m_cmd_binding_next->m_cmd_binding_pre = binding->m_cmd_binding_pre;
        }

    }
    else {
        if (binding->m_cmd_binding_next) {
            int rv;
            binding->m_cmd_binding_next->m_cmd_binding_pre = NULL;
            rv = cpe_hash_table_insert_unique(&binding->m_rsp->m_dp->m_cmd_2_rsps, binding->m_cmd_binding_next);
            assert(rv == 0);
            (void)rv;
        }
    }

    mem_free(dp->m_alloc, binding);
}

void dp_binding_free(struct dp_binding * binding) {
    dp_mgr_t dp = binding->m_rsp->m_dp;

    if (binding->m_cmd_binding_pre) {
        dp_binding_free_i(binding);
    }
    else {
        cpe_hash_table_remove_by_ins(&dp->m_cmd_2_rsps, binding);
    }
}

int32_t dp_binding_hash(const struct dp_binding * rsp) {
    if (rsp->m_kt == dp_key_numeric) {
        return ((const struct dp_binding_numeric *)rsp)->m_value;
    }
    else {
        const struct dp_binding_string * rsp_str = (const struct dp_binding_string *)rsp;
        return cpe_hash_str(rsp_str->m_value, rsp_str->m_value_len);;
    }
}

int dp_binding_cmp(const struct dp_binding * l, const struct dp_binding * r) {
    if (l->m_kt != r->m_kt) return 0;

    if (l->m_kt == dp_key_numeric) {
        return ((const struct dp_binding_numeric *)l)->m_value
            == ((const struct dp_binding_numeric *)r)->m_value;
    }
    else {
        const struct dp_binding_string * l_str = (const struct dp_binding_string *)l;
        const struct dp_binding_string * r_str = (const struct dp_binding_string *)r;

        return (l_str->m_value_len == r_str->m_value_len)
            && strcmp(l_str->m_value, r_str->m_value) == 0;
    }
}

int dp_rsp_bind_numeric(dp_rsp_t rsp, int32_t cmd, error_monitor_t em) {
    struct dp_binding_numeric * binding;
    dp_mgr_t dp;

    if (rsp == NULL) return -1;

    dp = rsp->m_dp;

    binding = (struct dp_binding_numeric *)
        mem_alloc(dp->m_alloc, sizeof(struct dp_binding_numeric));
    if (binding == NULL) {
        CPE_ERROR(em, "binding cmd %d to %s: no memory", cmd, dp_rsp_name(rsp));
        return -1;
    }

    binding->m_head.m_kt = dp_key_numeric;
    binding->m_value = cmd;

    if (dp_binding_add((struct dp_binding *)binding, rsp) != 0) {
        CPE_ERROR(em, "binding cmd %d to %s: binding already exist", cmd, dp_rsp_name(rsp));
        mem_free(dp->m_alloc, binding);
        return -1;
    }
    else {
        return 0;
    }
}

int dp_mgr_unbind_numeric(dp_mgr_t dp, int32_t cmd) {
    int count;
    struct dp_binding * binding;
    
    struct dp_binding_numeric key;
    key.m_head.m_kt = dp_key_numeric;
    key.m_value = cmd;

    count = 0;

    binding = (struct dp_binding *)
        cpe_hash_table_find(&dp->m_cmd_2_rsps, &key);
    if (binding) {
        while(binding->m_cmd_binding_next) {
            dp_binding_free(binding->m_cmd_binding_next);
            ++count;
        }

        dp_binding_free(binding);
        ++count;
    }

    return count;
}

int dp_rsp_unbind_numeric(dp_rsp_t rsp, int32_t cmd) {
    int count;
    struct dp_binding * binding;
    
    struct dp_binding_numeric key;
    key.m_head.m_kt = dp_key_numeric;
    key.m_value = cmd;

    count = 0;

    binding = (struct dp_binding *)
        cpe_hash_table_find(&rsp->m_dp->m_cmd_2_rsps, &key);
    while(binding) {
        struct dp_binding * check = binding;
        binding = binding->m_cmd_binding_next;
        
        if (check->m_rsp == rsp) {
            dp_binding_free(check);
            ++count;
        }
    }

    return count;
}


int dp_rsp_bind_string(dp_rsp_t rsp, const char * cmd, error_monitor_t em) {
    struct dp_binding_string * binding;
    dp_mgr_t dp;
    int cmdLen;

    if (rsp == NULL) return -1;

    dp = rsp->m_dp;

    cmdLen = strlen(cmd);

    binding = (struct dp_binding_string *)
        mem_alloc(dp->m_alloc, sizeof(struct dp_binding_string));
    if (binding == NULL) {
        CPE_ERROR(em, "binding cmd %s to %s: no memory", cmd, dp_rsp_name(rsp));
        return -1;
    }

    binding->m_head.m_kt = dp_key_string;
    binding->m_value = cmd;
    binding->m_value_len = cmdLen;

    if (dp_binding_add((struct dp_binding *)binding, rsp) != 0) {
        CPE_ERROR(em, "binding cmd %s to %s: binding already exist", cmd, dp_rsp_name(rsp));
        mem_free(dp->m_alloc, binding);
        return -1;
    }
    else {
        return 0;
    }
}

int dp_mgr_unbind_string(dp_mgr_t dp, const char * cmd) {
    int count;
    struct dp_binding * binding;
    
    struct dp_binding_string key;
    key.m_head.m_kt = dp_key_string;
    key.m_value = cmd;
    key.m_value_len = strlen(cmd);

    count = 0;

    binding = (struct dp_binding *)
        cpe_hash_table_find(&dp->m_cmd_2_rsps, &key);
    if (binding) {
        while(binding->m_cmd_binding_next) {
            dp_binding_free(binding->m_cmd_binding_next);
            ++count;
        }

        dp_binding_free(binding);
        ++count;
    }

    return count;
}

int dp_rsp_unbind_string(dp_rsp_t rsp, const char * cmd) {
    int count;
    struct dp_binding * binding;
    
    struct dp_binding_string key;
    key.m_head.m_kt = dp_key_string;
    key.m_value = cmd;
    key.m_value_len = strlen(cmd);

    count = 0;

    binding = (struct dp_binding *)
        cpe_hash_table_find(&rsp->m_dp->m_cmd_2_rsps, &key);
    while(binding) {
        struct dp_binding * check = binding;
        binding = binding->m_cmd_binding_next;
        
        if (check->m_rsp == rsp) {
            dp_binding_free(check);
            ++count;
        }
    }

    return count;
}

int dp_rsp_bind_by_cfg(dp_rsp_t dp_rsp, cfg_t cfg_respons, error_monitor_t em) {
    int rv = 0;

    if (cfg_respons == NULL) return 0;

    switch(cfg_type(cfg_respons)) {
    case CPE_CFG_TYPE_SEQUENCE: {
        struct cfg_it cfg_it;
        cfg_t cfg_sub;
        cfg_it_init(&cfg_it, cfg_respons);
        while((cfg_sub = cfg_it_next(&cfg_it))) {
            if (dp_rsp_bind_by_cfg(dp_rsp, cfg_sub, em) != 0) {
                rv = -1;
            }
        }
        break;
    }
    case CPE_CFG_TYPE_STRING: {
        const char * cmd = cfg_as_string(cfg_respons, NULL);
        if (cmd == NULL) {
            CPE_ERROR(
                em,
                "%s: dp_rsp_bind_by_cfg: not support bind to str cmd NULL!",
                dp_rsp_name(dp_rsp));
            return -1;
        }

        if (dp_rsp_bind_string(dp_rsp, cmd, em) != 0) {
            CPE_ERROR(
                em,
                "%s: dp_rsp_bind_by_cfg: not support bind to str cmd %s fail!",
                dp_rsp_name(dp_rsp), cmd);
            return -1;
        }

        break;
    }
    case CPE_CFG_TYPE_INT8:
    case CPE_CFG_TYPE_UINT8:
    case CPE_CFG_TYPE_INT16:
    case CPE_CFG_TYPE_UINT16:
    case CPE_CFG_TYPE_INT32:
    case CPE_CFG_TYPE_UINT32:
    case CPE_CFG_TYPE_INT64:
    case CPE_CFG_TYPE_UINT64:
    {
        int32_t cmd = cfg_as_int32(cfg_respons, -1);
        if (cmd == -1) {
            CPE_ERROR(
                em,
                "%s: dp_rsp_bind_by_cfg: read bind numeric cmd fail!",
                dp_rsp_name(dp_rsp));
            return -1;
        }

        if (dp_rsp_bind_numeric(dp_rsp, cmd, em) != 0) {
            CPE_ERROR(
                em,
                "%s: dp_rsp_bind_by_cfg: bind numeric cmd %d fail!",
                dp_rsp_name(dp_rsp), cmd);
            return -1;
        }

        break;
    }
    default:
        CPE_ERROR(
            em,
            "%s: dp_rsp_bind_by_cfg: not support bind to type %d!",
            dp_rsp_name(dp_rsp), cfg_type(cfg_respons));
        return -1;
    }

    return rv;
}
