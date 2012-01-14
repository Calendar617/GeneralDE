#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_strings.h"
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

static gd_dp_rsp_t gd_dp_rsp_binding_cmd_next(gd_dp_rsp_it_t it) {
    struct gd_dp_binding * r;
    
    if (it->m_context == NULL) return NULL;

    r = (struct gd_dp_binding *)it->m_context;
    it->m_context = r->m_cmd_binding_next;

    return r->m_rsp;
}

void gd_dp_rsp_find_by_numeric(gd_dp_rsp_it_t it, gd_dp_mgr_t dp, int32_t cmd) {
    struct gd_dp_binding_numeric buf;

    assert(it);
    assert(dp);

    buf.m_head.m_kt = gd_dp_key_numeric;
    buf.m_value = cmd;

    it->m_context = (struct gd_dp_binding *)cpe_hash_table_find(&dp->m_cmd_2_rsps, &buf);
    it->m_next_fun = gd_dp_rsp_binding_cmd_next;
}

void gd_dp_rsp_find_by_string(gd_dp_rsp_it_t it, gd_dp_mgr_t dp, const char * cmd) {
    struct gd_dp_binding_string buf;
    buf.m_head.m_kt = gd_dp_key_string;
    buf.m_value = cmd;
    buf.m_value_len = strlen(cmd);

    it->m_context = (struct gd_dp_binding *)cpe_hash_table_find(&dp->m_cmd_2_rsps, &buf);
    it->m_next_fun = gd_dp_rsp_binding_cmd_next;
}

gd_dp_rsp_t gd_dp_rsp_find_first_by_numeric(gd_dp_mgr_t dp, int32_t cmd) {
    struct gd_dp_rsp_it it;
    gd_dp_rsp_find_by_numeric(&it, dp, cmd);
    return gd_dp_rsp_next(&it);
}

gd_dp_rsp_t gd_dp_rsp_find_first_by_string(gd_dp_mgr_t dp, const char * cmd) {
    struct gd_dp_rsp_it it;
    gd_dp_rsp_find_by_string(&it, dp, cmd);
    return gd_dp_rsp_next(&it);
}

int gd_dp_dispatch_by_string(cpe_hash_string_t cmd, gd_dp_req_t req, error_monitor_t em) {
    gd_dp_rsp_t rsp;
    struct gd_dp_rsp_it rspIt;
    int rv;
    int count;

    count = 0;

    gd_dp_rsp_find_by_string(&rspIt, req->m_mgr, cpe_hs_data(cmd));

    while((rsp = gd_dp_rsp_next(&rspIt))) {
        ++count;

        if (rsp->m_processor == NULL) {
            CPE_ERROR(em, "responser %s have no processor", rsp->m_name);
            rv = -1;
            continue;
        }

        if (rsp->m_processor(req, rsp->m_context, em) != 0) {
            rv = -1;
            continue;
        }
    }

    if (count == 0) {
        CPE_ERROR(em, "no responser to process %s", cpe_hs_data(cmd));
        return -1;
    }

    return rv;
}

int gd_dp_dispatch_by_numeric(int32_t cmd, gd_dp_req_t req, error_monitor_t em) {
    gd_dp_rsp_t rsp;
    struct gd_dp_rsp_it rspIt;
    int rv;
    int count;

    count = 0;

    gd_dp_rsp_find_by_numeric(&rspIt, req->m_mgr, cmd);

    while((rsp = gd_dp_rsp_next(&rspIt))) {
        ++count;

        if (rsp->m_processor == NULL) {
            CPE_ERROR(em, "responser %s have no processor", rsp->m_name);
            rv = -1;
            continue;
        }

        if (rsp->m_processor(req, rsp->m_context, em) != 0) {
            rv = -1;
            continue;
        }
    }

    if (count == 0) {
        CPE_INFO(em, "no responser to process %d", cmd);
    }

    return rv;
}

int gd_dp_dispatch_by_name(const char * name, gd_dp_req_t req, error_monitor_t em) {
    gd_dp_rsp_t rsp;
    rsp = gd_dp_rsp_find_by_name(req->m_mgr, name);

    if (rsp == 0) {
        CPE_ERROR(em, "no responser name %s", name);
        return -1;
    }
    
    if (rsp->m_processor == NULL) {
        CPE_ERROR(em, "responser %s have no processor", rsp->m_name);
        return -1;
    }

    return rsp->m_processor(req, rsp->m_context, em);
}
