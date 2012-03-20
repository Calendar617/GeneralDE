#include <assert.h>
#include "cpe/utils/stream_buffer.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_cvt.h"
#include "gd/dp/dp_request.h"
#include "gd/app/app_context.h"
#include "usf/dr_store/dr_ref.h"
#include "usf/dr_store/dr_store_manage.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_pkg/bpg_pkg_manage.h"
#include "protocol/base/base_package.h"
#include "bpg_pkg_internal_types.h"

bpg_pkg_t
bpg_pkg_create(
    bpg_pkg_manage_t mgr,
    size_t pkg_capacity,
    LPDRMETA carry_data_meta,
    size_t carry_data_capacity)
{
    gd_dp_req_t dp_req;
    bpg_pkg_t bpg_pkg;

    if (pkg_capacity < sizeof (struct basepkg)) return NULL;

    dp_req = gd_dp_req_create(
        gd_app_dp_mgr(mgr->m_app),
        bpg_pkg_type_name,
        sizeof(struct bpg_pkg) + carry_data_capacity + pkg_capacity);
    if (dp_req == NULL) return NULL;

    bpg_pkg = (bpg_pkg_t)gd_dp_req_data(dp_req);

    bpg_pkg->m_carry_data_meta = carry_data_meta;
    bpg_pkg->m_carry_data_size = 0;
    bpg_pkg->m_carry_data_capacity = carry_data_capacity;
    bpg_pkg->m_dp_req = dp_req;

    bpg_pkg_init(bpg_pkg);

    return bpg_pkg;
}

void bpg_pkg_free(bpg_pkg_t req) {
    gd_dp_req_free(req->m_dp_req);
}

LPDRMETA bpg_pkg_carry_data_meta(bpg_pkg_t req) {
    return req->m_carry_data_meta;
}

void * bpg_pkg_carry_data(bpg_pkg_t req) {
    return req + 1;
}

size_t bpg_pkg_carry_data_capacity(bpg_pkg_t req) {
    return req->m_carry_data_capacity;
}

size_t bpg_pkg_carry_data_size(bpg_pkg_t req) {
    return req->m_carry_data_size;
}

int bpg_pkg_carry_data_set_size(bpg_pkg_t req, size_t size) {
    if (size > req->m_carry_data_capacity) return -1;
    req->m_carry_data_size = size;
    return 0;
}

bpg_pkg_manage_t bpg_pkg_mgr(bpg_pkg_t req) {
    return req->m_mgr;
}

size_t bpg_pkg_pkg_capacity(bpg_pkg_t req) {
    return gd_dp_req_capacity(req->m_dp_req) - sizeof(struct bpg_pkg) - req->m_carry_data_capacity;
}

void * bpg_pkg_pkg_data(bpg_pkg_t req) {
    return ((char *)(req + 1)) + req->m_carry_data_capacity;
}

size_t bpg_pkg_pkg_data_size(bpg_pkg_t req) {
    return gd_dp_req_capacity(req->m_dp_req) - sizeof(struct bpg_pkg) - req->m_carry_data_capacity;
}

int bpg_pkg_pkg_data_set_size(bpg_pkg_t req, size_t size) {
    size_t total_size = size + sizeof(struct bpg_pkg) + req->m_carry_data_capacity;
    if (total_size > gd_dp_req_capacity(req->m_dp_req)) return -1;
    return gd_dp_req_set_size(req->m_dp_req, total_size);
}

void bpg_pkg_init(bpg_pkg_t bpg_pkg) {
    struct basepkg_head * head;

    bpg_pkg_pkg_data_set_size(bpg_pkg, sizeof(struct basepkg_head));

    head = (struct basepkg_head *)bpg_pkg_pkg_data(bpg_pkg);
    bzero(head, sizeof(struct basepkg_head));
    head->magic = BASEPKG_HEAD_MAGIC;
    head->version = 1;
}

uint32_t bpg_pkg_cmd(bpg_pkg_t req) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_pkg_pkg_data(req);

    return head->cmd;
}

void bpg_pkg_set_cmd(bpg_pkg_t req, uint32_t cmd) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_pkg_pkg_data(req);

    head->cmd = cmd;
}

uint32_t bpg_pkg_sn(bpg_pkg_t req) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_pkg_pkg_data(req);

    return head->sn;
}

void bpg_pkg_set_sn(bpg_pkg_t req, uint32_t sn) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_pkg_pkg_data(req);

    head->sn = sn;
}

uint32_t bpg_pkg_errno(bpg_pkg_t req) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_pkg_pkg_data(req);

    return head->errno;
}

void bpg_pkg_set_errno(bpg_pkg_t req, uint32_t errno) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_pkg_pkg_data(req);

    head->errno = errno;
}

gd_dp_req_t bpg_pkg_to_dp_req(bpg_pkg_t req) {
    return req->m_dp_req;
}

uint32_t bpg_pkg_client_id(bpg_pkg_t req) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_pkg_pkg_data(req);

    return head->clientId;
}

void bpg_pkg_set_client_id(bpg_pkg_t req, uint32_t client_id) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_pkg_pkg_data(req);

    head->clientId = client_id;
}


bpg_pkg_t bpg_pkg_from_dp_req(gd_dp_req_t req) {
    if (cpe_hs_cmp(gd_dp_req_type_hs(req), bpg_pkg_type_name) != 0) return NULL;
    return (bpg_pkg_t)gd_dp_req_data(req);
}

dr_cvt_t bpg_pkg_data_cvt(bpg_pkg_t pkg) {
    return pkg->m_mgr->m_data_cvt;
}

dr_cvt_t bpg_pkg_base_cvt(bpg_pkg_t pkg) {
    return pkg->m_mgr->m_base_cvt;
}

LPDRMETA bpg_pkg_base_meta(bpg_pkg_t pkg) {
    LPDRMETALIB metalib;

    metalib = dr_ref_lib(pkg->m_mgr->m_metalib_basepkg_ref);
    return metalib ? dr_lib_find_meta_by_name(metalib, "basepkg") : NULL;
}

LPDRMETA bpg_pkg_cmd_meta(bpg_pkg_t pkg) {
    return bpg_pkg_manage_cmd_meta(pkg->m_mgr);
}

LPDRMETA bpg_pkg_main_data_meta(bpg_pkg_t pkg, error_monitor_t em) {
    LPDRMETA cmd_meta;
    int cmd_entry_idx;
    LPDRMETA data_meta;

    cmd_meta = bpg_pkg_cmd_meta(pkg);
    if (cmd_meta == NULL) {
        CPE_ERROR(
            em, "%s: bpg_pkg_main_data_meta: cmd meta \"%s\" not exist!",
            bpg_pkg_manage_name(pkg->m_mgr), pkg->m_mgr->m_cmd_meta_name);
        return NULL;
    }

    cmd_entry_idx = dr_meta_find_entry_idx_by_id(cmd_meta, bpg_pkg_cmd(pkg));
    if (cmd_entry_idx < 0) {
        CPE_ERROR(
            em, "%s: bpg_pkg_main_data_meta:  meta \"%s\" have no entry associate with cmd %d!",
            bpg_pkg_manage_name(pkg->m_mgr), dr_meta_name(cmd_meta), bpg_pkg_cmd(pkg));
        return NULL;
    }

    data_meta = dr_entry_self_meta(dr_meta_entry_at(cmd_meta, cmd_entry_idx));
    if (data_meta == NULL) {
        CPE_ERROR(
            em, "%s: bpg_pkg_main_data_meta:  %s[%d] have no associate meta!",
            bpg_pkg_manage_name(pkg->m_mgr), dr_meta_name(cmd_meta), cmd_entry_idx);
        return NULL;
    }

    return data_meta;
}

LPDRMETA bpg_pkg_append_data_meta(bpg_pkg_t pkg, bpg_pkg_append_info_t append_info, error_monitor_t em) {
    LPDRMETALIB metalib;
    LPDRMETA data_meta;

    metalib = bpg_pkg_manage_data_metalib(pkg->m_mgr);
    if (metalib == NULL) {
        CPE_ERROR(
            em, "%s: bpg_pkg_append_data_meta:  data meta \"%s\" not exist!",
            bpg_pkg_manage_name(pkg->m_mgr), pkg->m_mgr->m_cmd_meta_name);
        return NULL;
    }

    data_meta = dr_lib_find_meta_by_id(metalib, bpg_pkg_append_info_id(append_info));
    if (data_meta == NULL) {
        CPE_ERROR(
            em, "%s: bpg_pkg_append_data_meta:  meta of id %d not exist in lib %s!",
            bpg_pkg_manage_name(pkg->m_mgr), bpg_pkg_append_info_id(append_info), pkg->m_mgr->m_cmd_meta_name);
        return NULL;
    }

    return data_meta;
}

void * bpg_pkg_body_data(bpg_pkg_t pkg) {
    struct basepkg * basepkg;

    basepkg = (struct basepkg *)bpg_pkg_pkg_data(pkg);

    return basepkg->body;
}
    
uint32_t bpg_pkg_body_len(bpg_pkg_t pkg) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_pkg_pkg_data(pkg);

    return head->bodylen;
}

uint32_t bpg_pkg_body_origin_len(bpg_pkg_t pkg) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_pkg_pkg_data(pkg);
    return head->bodylen;
}

int32_t bpg_pkg_append_info_count(bpg_pkg_t pkg) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_pkg_pkg_data(pkg);
    return head->appendInfoCount;
}

bpg_pkg_append_info_t bpg_pkg_append_info_at(bpg_pkg_t pkg, int32_t pos) {
    struct basepkg_head * head;
    head = (struct basepkg_head *)bpg_pkg_pkg_data(pkg);

    return (pos >= 0 && pos < head->appendInfoCount)
        ? (bpg_pkg_append_info_t)(&head->appendInfos[pos])
        : NULL;
}

uint32_t bpg_pkg_append_info_id(bpg_pkg_append_info_t append_info) {
    return ((struct AppendInfo *)append_info)->id;
}

void * bpg_pkg_append_info_data(bpg_pkg_t pkg, bpg_pkg_append_info_t append_info) {
    int pos;
    struct basepkg * basepkg;
    char * buf;
    int i;

    basepkg = (struct basepkg *)bpg_pkg_pkg_data(pkg);

    pos = (((struct AppendInfo *)append_info) - basepkg->head.appendInfos);

    if (pos < 0 || pos > basepkg->head.appendInfoCount) return NULL;

    buf = (char*)basepkg->body;
    buf += basepkg->head.bodylen;

    for(i = 0; i < pos; ++i) {
        buf += basepkg->head.appendInfos[i].size;
    }

    return buf;
}

uint32_t bpg_pkg_append_info_size(bpg_pkg_append_info_t append_info) {
    return ((struct AppendInfo *)append_info)->size;
}

uint32_t bpg_pkg_append_info_origin_size(bpg_pkg_append_info_t append_info) {
    return ((struct AppendInfo *)append_info)->originSize;
}

const char * bpg_pkg_dump(bpg_pkg_t req, mem_buffer_t buffer) {
//    char decode_buf[4 * 1024];
    struct write_stream_buffer stream = CPE_WRITE_STREAM_BUFFER_INITIALIZER(buffer);
    LPDRMETA meta;

    mem_buffer_clear_data(buffer);

    /* static Tsf4g::Tdr::Meta const & s_basePkgHeadMeta = metaInBasePkg("basepkg_head"); */

    /* pkg = (struct basepkg *)bpg_pkg_pkg_data(req); */

    /* stream_printf(((write_stream_t)&stream), "head: "); */
    /* s_basePkgHeadMeta.dump_data((write_stream_t)&stream, &bq); */

    /* stream_printf(((write_stream_t)&stream), "\nbody: "); */

    /* if (Tsf4g::Tdr::Meta const * bodyMeta = metaOfCmd(bq.head.cmd)) { */
    /*     stream_printf(((write_stream_t)&stream), " %s", bodyMeta->name().c_str()); */

    /*     bodyMeta->decode(decode_buf, sizeof(decode_buf), bq.body, bq.head.bodylen); */
    /*     bodyMeta->dump_data((write_stream_t)&stream, decode_buf); */
    /* } */

    /* const char * appendBuf = ((const char *)bq.body) + bq.head.bodylen; */
    /* for(int i = 0; i < bq.head.appendInfoCount; ++i) { */
    /*     Tsf4g::Tdr::Meta const & meta = metaInNetData(bq.head.appendInfos[i].id); */
    /*     stream_printf(((write_stream_t)&stream), "\nappend %d(%s): ", meta.id(), meta.name().c_str()); */

    /*     meta.decode(decode_buf, sizeof(decode_buf), appendBuf, bq.head.appendInfos[i].size); */
    /*     meta.dump_data((write_stream_t)&stream, decode_buf); */
    /*     appendBuf += bq.head.appendInfos[i].size; */
    /* } */

    stream_putc((write_stream_t)&stream, 0);

    return (const char *)mem_buffer_make_continuous(buffer, 0);
}

CPE_HS_DEF_VAR(bpg_pkg_type_name, "bpg_pkg_type");

