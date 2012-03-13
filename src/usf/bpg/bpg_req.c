#include "gd/dp/dp_request.h"
#include "gd/app/app_context.h"
#include "usf/bpg/bpg_req.h"
#include "protocol/base/base_package.h"
#include "bpg_internal_types.h"

bpg_req_t
bpg_req_create(
    gd_app_context_t app,
    size_t pkg_capacity,
    LPDRMETA carry_data_meta,
    size_t carry_data_capacity)
{
    gd_dp_req_t dp_req;
    bpg_req_t bpg_req;
    struct basepkg * pkg;

    if (pkg_capacity < sizeof (struct basepkg)) return NULL;

    dp_req = gd_dp_req_create(
        gd_app_dp_mgr(app),
        bpg_req_type_name,
        sizeof(struct bpg_req) + carry_data_capacity + pkg_capacity);
    if (dp_req == NULL) return NULL;

    bpg_req = (bpg_req_t)gd_dp_req_data(dp_req);

    bpg_req->m_carry_data_meta = carry_data_meta;
    bpg_req->m_carry_data_size = 0;
    bpg_req->m_carry_data_capacity = carry_data_capacity;
    bpg_req->m_dp_req = dp_req;

    bpg_req_pkg_data_set_size(bpg_req, sizeof(struct basepkg));

    pkg = (struct basepkg *)bpg_req_pkg_data(bpg_req);
    bzero(pkg, sizeof(struct basepkg));
    pkg->head.magic = BASEPKG_HEAD_MAGIC;
    pkg->head.version = 1;

    return bpg_req;
}

void bpg_req_free(bpg_req_t req) {
    gd_dp_req_free(req->m_dp_req);
}

LPDRMETA bpg_req_carry_data_meta(bpg_req_t req) {
    return req->m_carry_data_meta;
}

void * bpg_req_carry_data(bpg_req_t req) {
    return req + 1;
}

size_t bpg_req_carry_data_capacity(bpg_req_t req) {
    return req->m_carry_data_capacity;
}

size_t bpg_req_carry_data_size(bpg_req_t req) {
    return req->m_carry_data_size;
}

int bpg_req_carry_data_set_size(bpg_req_t req, size_t size) {
    if (size > req->m_carry_data_capacity) return -1;
    req->m_carry_data_size = size;
    return 0;
}

size_t bpg_req_pkg_capacity(bpg_req_t req) {
    return gd_dp_req_capacity(req->m_dp_req) - sizeof(struct bpg_req) - req->m_carry_data_capacity;
}

void * bpg_req_pkg_data(bpg_req_t req) {
    return ((char *)(req + 1)) + req->m_carry_data_capacity;
}

size_t bpg_req_pkg_data_size(bpg_req_t req) {
    return gd_dp_req_capacity(req->m_dp_req) - sizeof(struct bpg_req) - req->m_carry_data_capacity;
}

int bpg_req_pkg_data_set_size(bpg_req_t req, size_t size) {
    size_t total_size = size + sizeof(struct bpg_req) + req->m_carry_data_capacity;
    if (total_size > gd_dp_req_capacity(req->m_dp_req)) return -1;
    return gd_dp_req_set_size(req->m_dp_req, total_size);
}

gd_dp_req_t bpg_req_to_dp_req(bpg_req_t req) {
    return req->m_dp_req;
}

bpg_req_t bpg_req_from_dp_req(gd_dp_req_t req) {
    if (cpe_hs_cmp(gd_dp_req_type_hs(req), bpg_req_type_name) != 0) return NULL;
    return (bpg_req_t)gd_dp_req_data(req);
}

CPE_HS_DEF_VAR(bpg_req_type_name, "bpg_req_type");

