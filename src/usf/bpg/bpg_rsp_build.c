#include "cpe/utils/buffer.h"
#include "cpe/cfg/cfg_read.h"
#include "usf/bpg/bpg_manage.h"
#include "usf/bpg/bpg_rsp.h"

static int bpg_rsp_build_i(bpg_manage_t mgr, cfg_t cfg, mem_buffer_t buf, cfg_t root, error_monitor_t em) {
    struct cfg_it child_it;
    cfg_t child;
    int rv;

    rv = 0;

    if (cfg_type(cfg) == CPE_CFG_TYPE_SEQUENCE) {
        cfg_it_init(&child_it, cfg);
        while((child = cfg_it_next(&child_it))) {
            bpg_rsp_t rsp = bpg_rsp_create(mgr, child);
            if (rsp == NULL) {
                CPE_ERROR(
                    em, "%s: %s: create fail",
                    bpg_manage_name(mgr), cfg_path(buf, cfg, root));
                rv = -1;
            }
        }
    }
    else if (cfg_type(cfg) == CPE_CFG_TYPE_STRUCT) {
        cfg_it_init(&child_it, cfg);
        while((child = cfg_it_next(&child_it))) {
            if (bpg_rsp_build_i(mgr, child, buf, root, em) != 0) {
                rv = -1;
            }
        } 
    }
    else {
        CPE_ERROR(
            em, "%s: %s: not support build rsp from type %d",
            bpg_manage_name(mgr), cfg_path(buf, cfg, root), cfg_type(cfg));
        rv = -1;
    }

    return rv;
}

int bpg_rsp_build(bpg_manage_t mgr, cfg_t cfg, error_monitor_t em) {
    struct mem_buffer buffer;
    int r;

    mem_buffer_init(&buffer, NULL);

    r = bpg_rsp_build_i(mgr, cfg, &buffer, cfg, em);

    mem_buffer_clear(&buffer);

    return r;
}



