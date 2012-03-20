#include <assert.h>
#include "gd/dp/dp_manage.h"
#include "usf/bpg/bpg_pkg.h"
#include "usf/bpg/bpg_manage.h"
#include "usf/bpg/bpg_running_req.h"
#include "bpg_internal_ops.h"

bpg_running_req_t
bpg_running_req_create(bpg_manage_t mgr, bpg_pkg_t req) {
    bpg_running_req_t running_req;

    running_req = (bpg_running_req_t)mem_alloc(mgr->m_alloc, sizeof(struct bpg_running_req));
    if (running_req == NULL) return NULL;

    running_req->m_mgr = mgr;
    running_req->m_sn = ++mgr->m_running_req_sn;
    running_req->m_dispatch_way = bpg_running_response_way_dispatch_none;
    running_req->m_dispatch_target.m_response_to_str = NULL;
    running_req->m_logic_req = 0;

    cpe_hash_entry_init(&running_req->m_hh);

    if (cpe_hash_table_insert_unique(&mgr->m_running_reqs, running_req) != 0) {
        mem_free(mgr->m_alloc, running_req);
        return NULL;
    }

    if (gd_dp_dispatch_by_string(mgr->m_send_to, bpg_pkg_to_dp_req(req), mgr->m_em) != 0) {
        CPE_ERROR(mgr->m_em, "%s: bpg_running_req_create: dispatch fail!", bpg_manage_name(mgr));
        bpg_running_req_free(running_req);
        return NULL;
    }
    
    return running_req;
}

void bpg_running_req_free(bpg_running_req_t running_req) {
    cpe_hash_table_remove_by_ins(&running_req->m_mgr->m_running_reqs, running_req);
    mem_free(running_req->m_mgr->m_alloc, running_req);
}

bpg_pkg_sn_t bpg_running_req_sn(bpg_running_req_t running_req) {
    return running_req->m_sn;
}

void bpg_running_req_set_response_to_str(bpg_running_req_t running_req, const char * response_to) {
    running_req->m_dispatch_way = bpg_running_response_req_dispatch_str;
    running_req->m_dispatch_target.m_response_to_str = response_to;
}

const char *  bpg_running_req_response_to_str(bpg_running_req_t running_req) {
    return running_req->m_dispatch_way == bpg_running_response_req_dispatch_str
        ? running_req->m_dispatch_target.m_response_to_str
        : NULL;
}

void bpg_running_req_set_response_to_int(bpg_running_req_t running_req, int32_t response_to) {
    running_req->m_dispatch_way = bpg_running_response_req_dispatch_int;
    running_req->m_dispatch_target.m_response_to_int = response_to;
}

int32_t bpg_running_req_response_to_int(bpg_running_req_t running_req) {
    return running_req->m_dispatch_way == bpg_running_response_req_dispatch_str
        ? running_req->m_dispatch_target.m_response_to_int
        : 0;
}

void bpg_running_req_set_logic_request(bpg_running_req_t running_req, logic_require_id_t logic_req_id) {
    running_req->m_logic_req = logic_req_id;
}

logic_require_id_t bpg_running_req_logic_request(bpg_running_req_t running_req) {
    return running_req->m_logic_req;
}

uint32_t bpg_running_req_hash(const struct bpg_running_req * running_req) {
    return running_req->m_sn;
}

int bpg_running_req_cmp(const struct bpg_running_req * l, const struct bpg_running_req * r) {
    return l->m_sn == r->m_sn;
}

void bpg_running_req_free_all(bpg_manage_t mgr) {
    struct cpe_hash_it req_it;
    bpg_running_req_t req;

    cpe_hash_it_init(&req_it, &mgr->m_running_reqs);

    req = cpe_hash_it_next(&req_it);
    while(req) {
        bpg_running_req_t next = cpe_hash_it_next(&req_it);
        bpg_running_req_free(req);
        req = next;
    }
}

