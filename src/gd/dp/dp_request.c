#include "gd/dp/dp_request.h"
#include "dp_internal_types.h"

void gd_dp_req_init(
    gd_dp_req_t req,
    gd_dp_mgr_t mgr, gd_dp_req_t parent,
    cpe_hash_string_t type,
    void * data, size_t capacity)
{
    req->m_mgr = mgr;
    req->m_talloc = parent ? parent->m_talloc : mgr->m_alloc;
    req->m_type = type;
    req->m_parent = parent;
    req->m_data = data;
    req->m_data_capacity = capacity;
    req->m_data_size = 0;

    TAILQ_INIT(&req->m_childs);
}

gd_dp_req_t
gd_dp_req_create(
    gd_dp_mgr_t mgr,
    cpe_hash_string_t type,
    size_t capacity)
{
    gd_dp_req_t req;

    if (mgr == NULL || type == NULL) return NULL;

    req = (gd_dp_req_t)
        mem_alloc(mgr->m_alloc, sizeof(struct gd_dp_req ) + capacity);
    if (req == NULL) return NULL;

    gd_dp_req_init(req, mgr, NULL, type, (void*)(req + 1), capacity);

    return req;
}

gd_dp_req_t
gd_dp_req_create_child(
    gd_dp_req_t parent,
    cpe_hash_string_t type,
    void * data,
    size_t capacity)
{
    gd_dp_req_t req;

    if (parent == NULL || parent == NULL) return NULL;

    req = (gd_dp_req_t)
        mem_alloc(parent->m_mgr->m_alloc, sizeof(struct gd_dp_req ));

    gd_dp_req_init(req, parent->m_mgr, parent, type, data, capacity);

    TAILQ_INSERT_TAIL(&parent->m_childs, req, m_brother);

    return req;
}

void gd_dp_req_free(gd_dp_req_t req) {
    if (req == NULL) return;

    while(!TAILQ_EMPTY(&req->m_childs)) {
        gd_dp_req_free(TAILQ_FIRST(&req->m_childs));
    }

    if (req->m_parent) {
        TAILQ_REMOVE(&req->m_parent->m_childs, req, m_brother);
    }

    mem_free(req->m_mgr->m_alloc, req);
}

cpe_hash_string_t gd_dp_req_type_hs(gd_dp_req_t req) {
    return req->m_type;
}

const char * gd_dp_req_type(gd_dp_req_t req) {
    return cpe_hs_data(req->m_type);
}

void * gd_dp_req_data(gd_dp_req_t req) {
    return req->m_data;
}

size_t gd_dp_req_capacity(gd_dp_req_t req) {
    return req->m_data_capacity;
}

size_t gd_dp_req_size(gd_dp_req_t req) {
    return req->m_data_size;
}

int gd_dp_req_set_size(gd_dp_req_t req, size_t size) {
    if (size >req->m_data_capacity) return -1;
    req->m_data_size = size;
    return 0;
}

mem_allocrator_t
gd_dp_req_talloc(gd_dp_req_t req) {
    return req->m_talloc;
}
