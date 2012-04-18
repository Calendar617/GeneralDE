#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/pal/pal_stdio.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "gd/app/app_context.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_pkg/bpg_pkg_dsp.h"
#include "usf/bpg_pkg/bpg_pkg_manage.h"
#include "usf/bpg_use/bpg_use_sp.h"
#include "bpg_use_internal_types.h"

bpg_use_sp_t
bpg_use_sp_create(gd_app_context_t app, cfg_t cfg, error_monitor_t em) {
    char * buf;
    const char * name;
    size_t name_len;
    bpg_use_sp_t sp;
    const char * pkg_manage_name;
    bpg_pkg_manage_t pkg_manage;

    assert(app);

    if (em  == NULL) em = gd_app_em(app);

    name = cfg_get_string(cfg, "name", 0);
    if (name == NULL) {
        CPE_ERROR(em, "bpg_use_sp_create: no name configured");
        return NULL;
    }
    name_len = strlen(name) + 1 + 3/*.sp*/;

    pkg_manage_name = cfg_get_string(cfg, "pkg-manage", 0);
    if (pkg_manage_name == NULL) {
        CPE_ERROR(em, "bpg_use_sp_create: pkg-manage not configured");
        return NULL;
    }
    pkg_manage = bpg_pkg_manage_find_nc(app, pkg_manage_name);
    if (pkg_manage == NULL) {
        CPE_ERROR(em, "bpg_use_sp_create: pkg-manage %s not exist", pkg_manage_name);
        return NULL;
    }

    buf = mem_alloc(gd_app_alloc(app), sizeof(struct bpg_use_sp) + name_len);
    if (buf == NULL) return NULL;

    sp = (bpg_use_sp_t)(buf);

    snprintf((char*)(sp + 1), name_len, "%s.sp", name);

    sp->m_pkg_manage = pkg_manage;
    sp->m_app = app;
    sp->m_alloc = gd_app_alloc(app);
    sp->m_em = em;

    sp->m_pkg_buf_size = cfg_get_uint32(cfg, "pkg-buf-size", 4 * 1024);
    sp->m_pkg_buf = NULL;

    mem_buffer_init(&sp->m_data_buf, gd_app_alloc(app));

    sp->m_dsp = bpg_pkg_dsp_create(gd_app_alloc(app));
    if (sp->m_dsp == NULL) {
        CPE_ERROR(em, "%s: bpg_use_sp_create: create dsp fail!", bpg_use_sp_name(sp));
        mem_buffer_clear(&sp->m_data_buf);
        mem_free(sp->m_alloc, sp);
        return NULL;
    }

    if (bpg_pkg_dsp_load(sp->m_dsp, cfg_find_cfg(cfg, "send-to"), em) != 0) {
        CPE_ERROR(em, "%s: bpg_use_sp_create: load dsp fail!", bpg_use_sp_name(sp));
        bpg_pkg_dsp_free(sp->m_dsp);
        mem_buffer_clear(&sp->m_data_buf);
        mem_free(sp->m_alloc, sp);
        return NULL;
    }
    
    return sp;
}

void bpg_use_sp_free(bpg_use_sp_t sp) {
    if (sp->m_pkg_buf) {
        bpg_pkg_free(sp->m_pkg_buf);
        sp->m_pkg_buf = NULL;
    }

    if (sp->m_dsp != NULL) {
        bpg_pkg_dsp_free(sp->m_dsp);
        sp->m_dsp = NULL;
    }

    mem_buffer_clear(&sp->m_data_buf);

    mem_free(sp->m_alloc, sp);
}

const char * bpg_use_sp_name(bpg_use_sp_t sp) {
    return (const char *)(sp + 1);
}

bpg_pkg_t
bpg_use_sp_pkg_buf(bpg_use_sp_t sp) {
    if (sp->m_pkg_buf &&
        (bpg_pkg_pkg_capacity(sp->m_pkg_buf) < sp->m_pkg_buf_size))
    {
        bpg_pkg_free(sp->m_pkg_buf);
        sp->m_pkg_buf = NULL;
    }

    if (sp->m_pkg_buf == NULL) {
        sp->m_pkg_buf = bpg_pkg_create(sp->m_pkg_manage, sp->m_pkg_buf_size, NULL, 0);
    }

    return sp->m_pkg_buf;
}

int bpg_use_sp_send(bpg_use_sp_t sp, bpg_pkg_t pkg) {
    return bpg_pkg_dsp_dispatch(sp->m_dsp, pkg, sp->m_em);
}

size_t bpg_use_sp_buf_capacity(bpg_use_sp_t sp) {
    return sp->m_pkg_buf_size;
}

bpg_pkg_manage_t bpg_use_sp_pkg_manage(bpg_use_sp_t sp) {
    return sp->m_pkg_manage;
}

gd_app_context_t bpg_use_sp_app(bpg_use_sp_t sp) {
    return sp->m_app;
}

LPDRMETALIB bpg_use_sp_metalib(bpg_use_sp_t sp) {
    return bpg_pkg_manage_data_metalib(sp->m_pkg_manage);
}

LPDRMETA bpg_use_sp_meta(bpg_use_sp_t sp, const char * name) {
    LPDRMETALIB metalib = bpg_pkg_manage_data_metalib(sp->m_pkg_manage);
    return metalib == NULL
        ? NULL
        : dr_lib_find_meta_by_name(metalib, name);
}

void * bpg_use_sp_data_buf(bpg_use_sp_t sp) {
    mem_buffer_set_size(&sp->m_data_buf, sp->m_pkg_buf_size);
    return mem_buffer_make_continuous(&sp->m_data_buf, 0);
}
