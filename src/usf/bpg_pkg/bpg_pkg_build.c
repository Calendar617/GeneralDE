#include <assert.h>
#include "protocol/base/base_package.h"
#include "cpe/pal/pal_strings.h"
#include "cpe/utils/stream_buffer.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/dr/dr_cfg.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_pkg/bpg_pkg_manage.h"
#include "bpg_pkg_internal_types.h"

int bpg_pkg_build_from_cfg(bpg_pkg_t req, cfg_t cfg, error_monitor_t em) {
    LPDRMETA head_meta;
    LPDRMETALIB data_metalib;
    LPDRMETA main_data_meta;
    LPDRMETA append_data_meta;
    struct basepkg_head * head;
    struct cfg_it cfg_it;
    cfg_t child_cfg;
    char buf[1024];

    data_metalib = bpg_pkg_manage_data_metalib(req->m_mgr);
    if (data_metalib == NULL) {
        CPE_ERROR(
            em, "%s: build pkg from cfg: data meta lib not exist!",
            bpg_pkg_manage_name(req->m_mgr));
        return -1;
    }

    head_meta = bpg_pkg_manage_basepkg_head_meta(req->m_mgr);
    if (head_meta == NULL) {
        CPE_ERROR(
            em, "%s: build pkg from cfg: head_meta not exist!",
            bpg_pkg_manage_name(req->m_mgr));
        return -1;
    }

    bpg_pkg_init(req);
    head = (struct basepkg_head *)bpg_pkg_pkg_data(req);

    if (dr_cfg_read(head, sizeof(struct basepkg_head), cfg, head_meta, 0, NULL) < 0) {
        CPE_ERROR(
            em, "%s: build pkg from cfg: read head fail!",
            bpg_pkg_manage_name(req->m_mgr));
        return -1;
    }

    bpg_pkg_clear_data(req);

    main_data_meta = bpg_pkg_main_data_meta(req, em);
    if (main_data_meta) {
        cfg_it_init(&cfg_it, cfg);
        while((child_cfg = cfg_it_next(&cfg_it))) {
            if (cfg_type(child_cfg) != CPE_CFG_TYPE_STRUCT) continue;
            if (strcmp(cfg_name(child_cfg), dr_meta_name(main_data_meta)) != 0) continue;

            if (dr_cfg_read(buf, sizeof(buf), child_cfg, main_data_meta, 0, NULL) < 0) {
                CPE_ERROR(
                    em, "%s: build pkg from cfg: read main body %s fail!",
                    bpg_pkg_manage_name(req->m_mgr), dr_meta_name(main_data_meta));
                return -1;
            }

            if (bpg_pkg_set_main_data(req, main_data_meta, buf, sizeof(buf), NULL, NULL) != 0) {
                CPE_ERROR(
                    em, "%s: build pkg from cfg: set main body %s fail!",
                    bpg_pkg_manage_name(req->m_mgr), dr_meta_name(main_data_meta));
                return -1;
            }
        }
    }

    cfg_it_init(&cfg_it, cfg);
    while((child_cfg = cfg_it_next(&cfg_it))) {
        if (cfg_type(child_cfg) != CPE_CFG_TYPE_STRUCT) continue;
        if (strcmp(cfg_name(child_cfg), dr_meta_name(main_data_meta)) == 0) continue;

        append_data_meta = dr_lib_find_meta_by_name(data_metalib, cfg_name(child_cfg));
        if (append_data_meta == NULL) continue;

        if (dr_cfg_read(buf, sizeof(buf), child_cfg, append_data_meta, 0, NULL) < 0) {
            CPE_ERROR(
                em, "%s: build pkg from cfg: read append body %s fail!",
                bpg_pkg_manage_name(req->m_mgr), dr_meta_name(append_data_meta));
            return -1;
        }

        if (bpg_pkg_add_append_data(req, append_data_meta, buf, sizeof(buf), NULL, NULL) != 0) {
            CPE_ERROR(
                em, "%s: build pkg from cfg: add append body %s fail!",
                bpg_pkg_manage_name(req->m_mgr), dr_meta_name(append_data_meta));
            return -1;
        }
    }

    return 0;
}
