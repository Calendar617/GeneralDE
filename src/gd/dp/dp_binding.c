#include <string.h>
#include "gd/dp/dp_responser.h"
#include "gd/dp/dp_manage.h"
#include "dp_internal_ops.h"

int gd_dp_binding_add(struct gd_dp_binding * binding, gd_dp_rsp_t rsp) {
    cpe_hash_entry_init(&binding->m_hh);
    if (cpe_hash_table_insert_unique(&rsp->m_dp->m_cmd_2_rsps, binding) != 0) {
        return -1;
    }

    binding->m_rsp = rsp;

    binding->m_binding_next = rsp->m_bindings;
    if (binding->m_binding_next)
        binding->m_binding_next->m_binding_pre = &binding->m_binding_next;
    binding->m_binding_pre = &rsp->m_bindings;
    rsp->m_bindings = binding;

    return 0;
}

void gd_dp_binding_free(struct gd_dp_binding * binding) {
    gd_dp_mgr_t dp = binding->m_rsp->m_dp;

    *binding->m_binding_pre = binding->m_binding_next;
    if (binding->m_binding_next) {
        binding->m_binding_next->m_binding_pre = binding->m_binding_pre;
    }
    cpe_hash_table_remove_by_ins(&dp->m_cmd_2_rsps, binding);

    mem_free(dp->m_alloc, binding);
}

int32_t gd_dp_binding_hash(const struct gd_dp_binding * rsp) {
    if (rsp->m_kt == gd_dp_key_numeric) {
        return ((const struct gd_dp_binding_numeric *)rsp)->m_value;
    }
    else {
        const struct gd_dp_binding_string * rsp_str = (const struct gd_dp_binding_string *)rsp;
        return cpe_hash_str(rsp_str->m_value, rsp_str->m_value_len);;
    }
}

int gd_dp_binding_cmp(const struct gd_dp_binding * l, const struct gd_dp_binding * r) {
    if (l->m_kt != r->m_kt) return 0;

    if (l->m_kt == gd_dp_key_numeric) {
        return ((const struct gd_dp_binding_numeric *)l)->m_value
            == ((const struct gd_dp_binding_numeric *)r)->m_value;
    }
    else {
        const struct gd_dp_binding_string * l_str = (const struct gd_dp_binding_string *)l;
        const struct gd_dp_binding_string * r_str = (const struct gd_dp_binding_string *)r;

        return (l_str->m_value_len == r_str->m_value_len)
            && strcmp(l_str->m_value, r_str->m_value) == 0;
    }
}

int gd_dp_mgr_bind_numeric(gd_dp_rsp_t rsp, int32_t cmd, error_monitor_t em) {
    struct gd_dp_binding_numeric * binding;
    gd_dp_mgr_t dp;

    if (rsp == NULL) return -1;

    dp = rsp->m_dp;

    binding = (struct gd_dp_binding_numeric *)
        mem_alloc(dp->m_alloc, sizeof(struct gd_dp_binding_numeric));
    if (binding == NULL) {
        CPE_ERROR(em, "binding cmd %d to %s: no memory", cmd, gd_dp_rsp_name(rsp));
        return -1;
    }

    binding->m_head.m_kt = gd_dp_key_numeric;
    binding->m_value = cmd;

    if (gd_dp_binding_add((struct gd_dp_binding *)binding, rsp) != 0) {
        CPE_ERROR(em, "binding cmd %d to %s: binding already exist", cmd, gd_dp_rsp_name(rsp));
        mem_free(dp->m_alloc, binding);
        return -1;
    }
    else {
        return 0;
    }
}

int gd_dp_mgr_unbind_numeric(gd_dp_mgr_t dp, int32_t cmd) {
    struct gd_dp_binding_numeric tmp;
    tmp.m_head.m_kt = gd_dp_key_numeric;
    tmp.m_value = cmd;

    return 
        cpe_hash_table_remove_by_key(&dp->m_cmd_2_rsps, &tmp) == 1
        ? 0
        : -1;
}

int gd_dp_mgr_bind_string(gd_dp_rsp_t rsp, const char * cmd, error_monitor_t em) {
    struct gd_dp_binding_string * binding;
    gd_dp_mgr_t dp;
    int cmdLen;

    if (rsp == NULL) return -1;

    dp = rsp->m_dp;

    cmdLen = strlen(cmd);

    binding = (struct gd_dp_binding_string *)
        mem_alloc(dp->m_alloc, sizeof(struct gd_dp_binding_string));
    if (binding == NULL) {
        CPE_ERROR(em, "binding cmd %s to %s: no memory", cmd, gd_dp_rsp_name(rsp));
        return -1;
    }

    binding->m_head.m_kt = gd_dp_key_string;
    binding->m_value = cmd;
    binding->m_value_len = cmdLen;

    if (gd_dp_binding_add((struct gd_dp_binding *)binding, rsp) != 0) {
        CPE_ERROR(em, "binding cmd %s to %s: binding already exist", cmd, gd_dp_rsp_name(rsp));
        mem_free(dp->m_alloc, binding);
        return -1;
    }
    else {
        return 0;
    }
}

int gd_dp_mgr_unbind_string(gd_dp_mgr_t dp, const char * cmd) {
    struct gd_dp_binding_string tmp;
    tmp.m_head.m_kt = gd_dp_key_numeric;
    tmp.m_value = cmd;
    tmp.m_value_len = strlen(cmd);

    return 
        cpe_hash_table_remove_by_key(&dp->m_cmd_2_rsps, &tmp) == 1
        ? 0
        : -1;
}

