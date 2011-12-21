#include <stdarg.h>
#include <string.h>
#include "gd/dp/dp_responser.h"
#include "dp_internal_ops.h"

gd_dp_rsp_t gd_dp_rsp_create(gd_dp_mgr_t dp, const char * name) {
    if (dp == NULL || name == NULL) return NULL;

    size_t nameLen = strlen(name);

    gd_dp_rsp_t r = mem_alloc(dp->m_alloc, sizeof(struct gd_dp_rsp) + nameLen + 1);
    if (r == NULL) return NULL;

    r->m_dp = dp;
    r->m_name = (char*)(r + 1);
    r->m_name_len = nameLen;
    r->m_bindings = NULL;
    r->m_processor = NULL;
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
    while(rsp->m_bindings) {
        gd_dp_binding_free(rsp->m_bindings);
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

int gd_dp_rsp_set_opt(gd_dp_rsp_t rsp, gd_dp_rsp_opt_t opt, ...) {
    int rv = -1;
    va_list ap;
    va_start(ap, opt);

    switch(opt) {
    case gd_dp_rsp_set_processor: {
        rsp->m_processor = va_arg(ap, gd_dp_rsp_process_fun_t);
        rv = 0;
        break;
    }
    case gd_dp_rsp_set_context: {
        rsp->m_context = va_arg(ap, void*);
        rv = 0;
        break;
    }
    default:
        rv = -1;
        break;
    }

    va_end(ap);

    return rv;
    
}

gd_dp_rsp_process_fun_t gd_dp_rsp_processor(gd_dp_rsp_t rsp) {
    return rsp->m_processor;
}

int32_t gd_dp_rsp_hash(const gd_dp_rsp_t rsp) {
    return cpe_hash_str(rsp->m_name, rsp->m_name_len);
}

int gd_dp_rsp_cmp(const gd_dp_rsp_t l, const gd_dp_rsp_t r) {
    return (l->m_name_len == r->m_name_len)
        && (strcmp(l->m_name, r->m_name) == 0);
}
