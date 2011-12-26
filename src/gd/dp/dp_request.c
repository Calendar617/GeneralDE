#include "gd/dp/dp_manage.h"
#include "gd/dp/dp_request.h"
#include "gd/dp/dp_node.h"
#include "dp_internal_types.h"

void gd_dp_req_init(
    gd_dp_req_t req,
    gd_dp_mgr_t mgr, gd_dp_req_t parent,
    cpe_hash_string_t type,
    gd_dp_node_t from, gd_dp_node_t to,
    void * data, size_t capacity)
{
    req->m_mgr = mgr;
    req->m_talloc = parent ? parent->m_talloc : mgr->m_alloc;
    req->m_type = type;
    req->m_parent = parent;
    req->m_from = from;
    req->m_to = to;
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

    gd_dp_req_init(req, mgr, NULL, type, NULL, NULL, (void*)(req + 1), capacity);

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

    gd_dp_req_init(req, parent->m_mgr, parent, type, parent->m_from, parent->m_to, data, capacity);

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

gd_dp_req_t gd_dp_req_parent(gd_dp_req_t req) {
    return req->m_parent;
}

gd_dp_req_t gd_dp_req_parent_find(gd_dp_req_t req, cpe_hash_string_t typeName) {
    while(req && cpe_hs_cmp(req->m_type, typeName) != 0) {
        req = req->m_parent;
    }

    return req;
}

gd_dp_req_t gd_dp_req_child_find(gd_dp_req_t req, cpe_hash_string_t typeName) {
    return NULL;
}

gd_dp_req_t gd_dp_req_brother_find(gd_dp_req_t req, cpe_hash_string_t typeName) {
    gd_dp_req_t parent = req->m_parent;
    gd_dp_req_t self = req;

    while(parent) {
        gd_dp_req_t brother;

        TAILQ_FOREACH(brother, &parent->m_childs, m_brother) {
            if (brother == self) continue;

            if (cpe_hs_cmp(brother->m_type, typeName) == 0) {
                return brother;
            }
        }

        self = parent;
        parent = parent->m_parent;
    }

    return NULL;
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

gd_dp_node_t gd_dp_req_from(gd_dp_req_t req) {
    return req->m_from;
}

void gd_dp_req_set_from(gd_dp_req_t req, gd_dp_node_t from) {
    req->m_from = from;
}

gd_dp_node_t gd_dp_req_to(gd_dp_req_t req) {
    return req->m_to;
}

void gd_dp_req_set_to(gd_dp_req_t req, gd_dp_node_t to) {
    req->m_to = to;
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

int gd_dp_req_send(gd_dp_req_t req, error_monitor_t em) {
    if (req == NULL) return -1;

    if (req->m_to == NULL) {
        CPE_ERROR(em, "requesnt not set send to!");
        return -1;
    }

    if (req->m_to->m_type == NULL) {
        CPE_ERROR(em, "requesnt send to %s have type!", gd_dp_node_name(req->m_to));
        return -1;
    }

    if (req->m_to->m_type->send == NULL) {
        CPE_ERROR(em, "requesnt send to %s have replay!", gd_dp_node_name(req->m_to));
        return -1;
    }

    return req->m_to->m_type->send(req->m_to, req, em);
}

int gd_dp_req_replay(gd_dp_req_t req, void * buf, size_t size, error_monitor_t em) {
    gd_dp_req_t replayReq;
    int rv;

    if (req == NULL || buf == NULL) return -1;

    if (req->m_from == NULL) {
        CPE_ERROR(em, "req have no from field, can`t replay!");
        return -1;
    }

    if (req->m_from->m_type && req->m_from->m_type->replay) {
        return req->m_from->m_type->replay(req->m_from, req, buf, size, em);
    }

    replayReq = gd_dp_req_create_child(req, gd_dp_req_type_replay, buf, size);
    if (replayReq == NULL) {
        CPE_ERROR(em, "create replay requesnt fail!");
        return -1;
    }

    gd_dp_req_set_to(replayReq, gd_dp_req_from(req));
    gd_dp_req_set_from(replayReq, gd_dp_req_to(req));
    gd_dp_req_set_size(replayReq, size);

    rv = gd_dp_req_send(replayReq, em);
    gd_dp_req_free(replayReq);
    return rv;
}

CPE_HS_DEF_VAR(gd_dp_req_type_replay, "req.dp.replay");
