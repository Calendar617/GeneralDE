#include <assert.h>
#include "protocol/base/base_package.h"
#include "cpe/pal/pal_strings.h"
#include "cpe/utils/stream_buffer.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/dr/dr_cfg.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_pkg/bpg_pkg_manage.h"
#include "bpg_pkg_internal_types.h"

int bpg_pkg_build_from_cfg(bpg_pkg_t req, cfg_t cfg, error_monitor_t em) {
    LPDRMETA head_meta;
    //LPDRMETALIB data_metalib;
    LPDRMETA meta;
    struct basepkg_head * head;
    //struct cfg_it cfg_it;

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
    }

    bpg_pkg_clear_data(req);

    meta = bpg_pkg_main_data_meta(req, em);
    if (meta == NULL) {
        
    }

    return 0;
}
