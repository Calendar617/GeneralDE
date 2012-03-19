#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_cvt.h"
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

    if (pkg_capacity < sizeof (struct basepkg)) return NULL;

    dp_req = gd_dp_req_create(
        gd_app_dp_mgr(app),
        bpg_req_type_name,
        sizeof(struct bpg_req) + carry_data_capacity + pkg_capacity);
    if (dp_req == NULL) return NULL;

    bpg_req = (bpg_req_t)gd_dp_req_data(dp_req);

    bpg_req->m_cvt = NULL;

    bpg_req->m_carry_data_meta = carry_data_meta;
    bpg_req->m_carry_data_size = 0;
    bpg_req->m_carry_data_capacity = carry_data_capacity;
    bpg_req->m_dp_req = dp_req;

    bpg_req_pkg_init(bpg_req);

    return bpg_req;
}

void bpg_req_free(bpg_req_t req) {
    if (req->m_cvt) {
        dr_cvt_free(req->m_cvt);
        req->m_cvt = NULL;
    }

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

void bpg_req_pkg_init(bpg_req_t bpg_req) {
    struct basepkg_head * head;

    bpg_req_pkg_data_set_size(bpg_req, sizeof(struct basepkg_head));

    head = (struct basepkg_head *)bpg_req_pkg_data(bpg_req);
    bzero(head, sizeof(struct basepkg_head));
    head->magic = BASEPKG_HEAD_MAGIC;
    head->version = 1;
}

int bpg_req_set_cvt(bpg_req_t req, const char * cvt) {
    dr_cvt_t new_cvt;

    if (req->m_cvt && strcmp(dr_cvt_name(req->m_cvt), cvt) == 0) return 0;

    new_cvt = dr_cvt_create(cvt);
    if (new_cvt == NULL) return -1;

    if (req->m_cvt) dr_cvt_free(req->m_cvt);

    req->m_cvt = new_cvt;
    return 0;
}

uint32_t bpg_req_cmd(bpg_req_t req) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_req_pkg_data(req);

    return head->cmd;
}

void bpg_req_set_cmd(bpg_req_t req, uint32_t cmd) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_req_pkg_data(req);

    head->cmd = cmd;
}

uint32_t bpg_req_sn(bpg_req_t req) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_req_pkg_data(req);

    return head->sn;
}

void bpg_req_set_sn(bpg_req_t req, uint32_t sn) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_req_pkg_data(req);

    head->sn = sn;
}

dr_cvt_t bpg_req_cvt(bpg_req_t req) {
    return req->m_cvt;
}

const char * bpg_req_cvt_name(bpg_req_t req) {
    return req->m_cvt ? dr_cvt_name(req->m_cvt) : "";
}

gd_dp_req_t bpg_req_to_dp_req(bpg_req_t req) {
    return req->m_dp_req;
}

bpg_req_t bpg_req_from_dp_req(gd_dp_req_t req) {
    if (cpe_hs_cmp(gd_dp_req_type_hs(req), bpg_req_type_name) != 0) return NULL;
    return (bpg_req_t)gd_dp_req_data(req);
}

int bpg_req_set_data(bpg_req_t req, LPDRMETA meta, void * buf, size_t capacity, error_monitor_t em) {
    struct basepkg_head * head;
    size_t cur_size;
    size_t use_size;

    assert(req);
    assert(meta);
    assert(buf);

    head = (struct basepkg_head *)bpg_req_pkg_data(req);

    if (head->appendInfoCount > 0) {
        CPE_ERROR(em, "bpg_req_set_data: already have append info!");
        return -1;
    }

    cur_size = sizeof(struct basepkg_head);
    use_size = bpg_req_pkg_capacity(req) - cur_size;

    if (dr_cvt_encode(
            bpg_req_cvt(req),
            meta, ((char *)bpg_req_pkg_data(req)) + cur_size, &use_size,
            buf, &capacity,
            em, 0) != 0)
    {
        CPE_ERROR(em, "bpg_req_set_data: encode fail!");
        return -1;
    }

    bpg_req_pkg_data_set_size(req, cur_size + use_size);

    head->bodylen = use_size;
    head->originBodyLen = capacity;
    head->bodytotallen = bpg_req_pkg_data_size(req);

    return use_size;
}

int bpg_req_append_data(bpg_req_t req, LPDRMETA meta, void * buf, size_t capacity, error_monitor_t em) {
    struct basepkg * pkg;
    struct AppendInfo * appendInfo;

    size_t cur_size;
    size_t use_size;

    assert(req);

    pkg = (struct basepkg *)bpg_req_pkg_data(req);

    if (pkg->head.appendInfoCount >= APPEND_INFO_MAX_COUNT) {
        CPE_ERROR(em, "bpg_req_pkg_data: max append info reached!");
        return -1;
    }

    cur_size = bpg_req_pkg_data_size(req);
    use_size = bpg_req_pkg_capacity(req) - cur_size;

    if (dr_cvt_encode(
            bpg_req_cvt(req), meta,
            ((char *)bpg_req_pkg_data(req)) + cur_size, &use_size,
            buf, &capacity,
            em, 0) != 0)
    {
        CPE_ERROR(em, "bpg_req_pkg_data: encode fail!");
        return -1;
    }

    appendInfo = &pkg->head.appendInfos[pkg->head.appendInfoCount++];
    appendInfo->id = dr_meta_id(meta);
    appendInfo->size = use_size;
    appendInfo->originSize = capacity;

    bpg_req_pkg_data_set_size(req, cur_size + use_size);

    pkg->head.bodytotallen = bpg_req_pkg_data_size(req);

    return use_size;
}

CPE_HS_DEF_VAR(bpg_req_type_name, "bpg_req_type");

