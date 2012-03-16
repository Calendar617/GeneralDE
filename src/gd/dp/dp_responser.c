#include <stdarg.h>
#include <string.h>
#include "gd/dp/dp_responser.h"
#include "dp_internal_ops.h"

gd_dp_rsp_t gd_dp_rsp_create(gd_dp_mgr_t dp, const char * name) {
    size_t nameLen;
    gd_dp_rsp_t r;

    if (dp == NULL || name == NULL) return NULL;

    nameLen = strlen(name);

    r = (gd_dp_rsp_t)mem_alloc(dp->m_alloc, sizeof(struct gd_dp_rsp) + nameLen + 1);
    if (r == NULL) return NULL;

    r->m_dp = dp;
    r->m_name = (char*)(r + 1);
    r->m_name_len = nameLen;
    r->m_bindings = NULL;
    r->m_processor = NULL;
    r->m_type = NULL;
    r->m_context = NULL;
    cpe_hash_entry_init(&r->m_hh);
    memcpy((char*)r->m_name, name, nameLen + 1);

    if (cpe_hash_table_insert_unique(&dp->m_rsps, r) != 0) {
        mem_free(dp->m_alloc, r);
        return NULL;
    }

    return r;
}

void gd_dp_rsp_free_i(gd_dp_rsp_t rsp) {
    gd_dp_pbuf_remove_rsp(rsp->m_dp, rsp);

    while(rsp->m_bindings) {
        gd_dp_binding_free(rsp->m_bindings);
    }

    if (rsp->m_type && rsp->m_type->destruct) {
        rsp->m_type->destruct(rsp);
    }

    mem_free(rsp->m_dp->m_alloc, rsp);
}

void gd_dp_rsp_free(gd_dp_rsp_t rsp) {
    if (rsp == NULL) return;
    cpe_hash_table_remove_by_ins(&rsp->m_dp->m_rsps, rsp);
}

const char * gd_dp_rsp_name(gd_dp_rsp_t rsp) {
    return rsp->m_name;
}

void gd_dp_rsp_set_processor(gd_dp_rsp_t rsp, gd_dp_rsp_process_fun_t process, void * ctx) {
    rsp->m_processor = process;
    rsp->m_context = ctx;
}

void gd_dp_rsp_set_type(gd_dp_rsp_t rsp, gd_dp_rsp_type_t type) {
    rsp->m_type = type;
}

gd_dp_rsp_process_fun_t gd_dp_rsp_processor(gd_dp_rsp_t rsp) {
    return rsp->m_processor;
}

gd_dp_rsp_type_t gd_dp_rsp_type(gd_dp_rsp_t rsp) {
    return rsp->m_type;
}

void * gd_dp_rsp_context(gd_dp_rsp_t rsp) {
    return rsp->m_context;
}

int32_t gd_dp_rsp_hash(const gd_dp_rsp_t rsp) {
    return cpe_hash_str(rsp->m_name, rsp->m_name_len);
}

int gd_dp_rsp_cmp(const gd_dp_rsp_t l, const gd_dp_rsp_t r) {
    return (l->m_name_len == r->m_name_len)
        && (strcmp(l->m_name, r->m_name) == 0);
}
