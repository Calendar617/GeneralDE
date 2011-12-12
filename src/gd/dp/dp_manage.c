#include <string.h>
#include <strings.h>
#include "gd/dp/dp_manage.h"
#include "gd/dp/dp_responser.h"
#include "dp_internal_ops.h"

gd_dp_mgr_t gd_dp_mgr_create(mem_allocrator_t alloc) {
    gd_dp_mgr_t dpm;
    size_t allocSize = sizeof(struct gd_dp_mgr);

    dpm = (gd_dp_mgr_t)mem_alloc(alloc, allocSize);
    bzero(dpm, allocSize);

    dpm->m_alloc = alloc;

    cpe_hash_table_init(
        &dpm->m_rsps,
        alloc,
        (cpe_hash_fun_t) gd_dp_rsp_hash,
        (cpe_hash_cmp_t) gd_dp_rsp_cmp,
        CPE_HASH_OBJ2ENTRY(gd_dp_rsp, m_hh),
        -1);

    cpe_hash_table_set_destory_fun(
        &dpm->m_rsps,
        (cpe_hash_destory_t)gd_dp_rsp_free_i,
        NULL);

    cpe_hash_table_init(
        &dpm->m_cmd_2_rsps,
        alloc,
        (cpe_hash_fun_t) gd_dp_binding_hash,
        (cpe_hash_cmp_t) gd_dp_binding_cmp,
        CPE_HASH_OBJ2ENTRY(gd_dp_binding, m_hh),
        -1);

    cpe_hash_table_set_destory_fun(
        &dpm->m_cmd_2_rsps,
        (cpe_hash_destory_t)gd_dp_binding_free_i,
        NULL);

    return dpm;
}

void gd_dp_mgr_free(gd_dp_mgr_t dp) {
    if (dp == NULL) return;

    cpe_hash_table_fini(&dp->m_rsps);

    cpe_hash_table_fini(&dp->m_cmd_2_rsps);

    mem_free(dp->m_alloc, dp);
}

gd_dp_rsp_t gd_dp_rsp_find_by_name(gd_dp_mgr_t dp, const char * name) {
    struct gd_dp_rsp rspBuf;

    rspBuf.m_name_len = strlen(name);
    rspBuf.m_name = name;

    return (gd_dp_rsp_t)cpe_hash_table_find(&dp->m_rsps, &rspBuf);
}

gd_dp_rsp_t gd_dp_rsp_find_by_numeric(gd_dp_mgr_t dp, int32_t cmd) {
    struct gd_dp_binding * r;
    struct gd_dp_binding_numeric buf;
    buf.m_head.m_kt = gd_dp_key_numeric;
    buf.m_value = cmd;

    r = (struct gd_dp_binding *)cpe_hash_table_find(&dp->m_cmd_2_rsps, &buf);

    return r
        ? r->m_rsp
        : NULL;
}

gd_dp_rsp_t gd_dp_rsp_find_by_string(gd_dp_mgr_t dp, const char * cmd) {
    struct gd_dp_binding * r;
    struct gd_dp_binding_string buf;
    buf.m_head.m_kt = gd_dp_key_string;
    buf.m_value = cmd;
    buf.m_value_len = strlen(cmd);

    r = (struct gd_dp_binding *)cpe_hash_table_find(&dp->m_cmd_2_rsps, &buf);

    return r
        ? r->m_rsp
        : NULL;
}
