#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_cvt.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "protocol/base/base_package.h"
#include "bpg_pkg_internal_types.h"

int bpg_pkg_set_main_data(bpg_pkg_t pkg, LPDRMETA meta, void * buf, size_t capacity, error_monitor_t em) {
    struct basepkg_head * head;
    size_t cur_size;
    size_t use_size;

    assert(pkg);
    assert(meta);
    assert(buf);

    head = (struct basepkg_head *)bpg_pkg_pkg_data(pkg);

    if (head->appendInfoCount > 0) {
        CPE_ERROR(em, "bpg_pkg_set_data: already have append info!");
        return -1;
    }

    cur_size = sizeof(struct basepkg_head);
    assert(cur_size == bpg_pkg_pkg_data_size(pkg));

    use_size = bpg_pkg_pkg_capacity(pkg) - cur_size;

    if (dr_cvt_encode(
            bpg_pkg_data_cvt(pkg),
            meta, ((char *)bpg_pkg_pkg_data(pkg)) + cur_size, &use_size,
            buf, &capacity,
            em, pkg->m_mgr->m_debug) != 0)
    {
        CPE_ERROR(em, "bpg_pkg_set_data: encode fail!");
        return -1;
    }

    bpg_pkg_pkg_data_set_size(pkg, cur_size + use_size);

    head->bodylen = use_size;
    head->originBodyLen = capacity;
    head->bodytotallen = bpg_pkg_pkg_data_size(pkg);

    return use_size;
}

int bpg_pkg_get_main_data(bpg_pkg_t pkg, LPDRMETA meta, void * buf, size_t * capacity, error_monitor_t em) {
    size_t input_size;
    size_t output_size;

    input_size = bpg_pkg_body_len(pkg);
    output_size = *capacity;

    if (dr_cvt_decode(
            bpg_pkg_data_cvt(pkg),
            meta,
            buf, &output_size,
            bpg_pkg_body_data(pkg), &input_size,
            em, pkg->m_mgr->m_debug) != 0)
    {
        CPE_ERROR(
            em, "bpg_pkg_get_data: %s decode data fail, input len is %d, output len is %d!",
            dr_meta_name(meta), bpg_pkg_body_len(pkg), (int)*capacity);
        return -1;
    }

    *capacity = output_size;

    return 0;
}

int bpg_pkg_add_append_data(bpg_pkg_t pkg, LPDRMETA meta, void * buf, size_t capacity, error_monitor_t em) {
    struct basepkg * basepkg;
    struct AppendInfo * appendInfo;
    size_t cur_size;
    size_t use_size;

    assert(pkg);

    basepkg = (struct basepkg *)bpg_pkg_pkg_data(pkg);

    if (basepkg->head.appendInfoCount >= APPEND_INFO_MAX_COUNT) {
        CPE_ERROR(em, "bpg_pkg_pkg_data: max append info reached!");
        return -1;
    }

    cur_size = bpg_pkg_pkg_data_size(pkg);
    use_size = bpg_pkg_pkg_capacity(pkg) - cur_size;

    if (dr_cvt_encode(
            bpg_pkg_data_cvt(pkg),
            meta,
            ((char *)bpg_pkg_pkg_data(pkg)) + cur_size, &use_size,
            buf, &capacity,
            em, pkg->m_mgr->m_debug) != 0)
    {
        CPE_ERROR(em, "bpg_pkg_pkg_data: encode fail!");
        return -1;
    }

    appendInfo = &basepkg->head.appendInfos[basepkg->head.appendInfoCount++];
    appendInfo->id = dr_meta_id(meta);
    appendInfo->size = use_size;
    appendInfo->originSize = capacity;

    bpg_pkg_pkg_data_set_size(pkg, cur_size + use_size);

    basepkg->head.bodytotallen = bpg_pkg_pkg_data_size(pkg);

    return use_size;
}

int bpg_pkg_get_append_data(
    bpg_pkg_t pkg, bpg_pkg_append_info_t append_inf,
    LPDRMETA meta, void * buf, size_t * capacity, error_monitor_t em)
{
    size_t input_size;
    size_t output_size;

    input_size = bpg_pkg_append_info_size(append_inf);
    output_size = *capacity;

    if (dr_cvt_decode(
            bpg_pkg_data_cvt(pkg),
            meta,
            buf, &output_size,
            bpg_pkg_append_info_data(pkg, append_inf), &input_size,
            em, pkg->m_mgr->m_debug) != 0)
    {
        CPE_ERROR(
            em, "bpg_pkg_get_append_info: %s decode data fail, input len is %d, output len is %d!",
            dr_meta_name(meta), bpg_pkg_append_info_size(append_inf), (int)*capacity);
        return -1;
    }

    *capacity = output_size;

    return 0;
}

