#include "cpe/pal/pal_external.h"
#include "cpe/utils/file.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/dr/dr_metalib_builder.h"
#include "cpe/dr/dr_metalib_build.h"
#include "gd/app/app_log.h"
#include "gd/app/app_library.h"
#include "gd/app/app_module.h"
#include "gd/app/app_library.h"
#include "gd/app/app_context.h"
#include "usf/dr_store/dr_store.h"
#include "usf/dr_store/dr_store_manage.h"
#include "dr_store_internal_types.h"

static int dr_store_loader_load_from_symbol(
    gd_app_context_t app, gd_app_module_t module, dr_store_manage_t mgr, const char * arg)
{
    int rv;
    LPDRMETALIB metalib;
    
    metalib = gd_app_lib_sym(NULL, arg, gd_app_em(app));
    if (metalib == NULL) {
        APP_CTX_ERROR(app, "%s: read load-from-symbol %s: error!", gd_app_module_name(module), arg);
        return -1;
    }

    rv = dr_store_add_lib(mgr, gd_app_module_name(module), metalib, NULL, NULL);

    if (rv == 0) {
        if (mgr->m_debug) {
            APP_CTX_INFO(app, "%s: metalib load from symbol %s: success!", gd_app_module_name(module), arg);
        }
    }

    return rv;
}

static int dr_store_loader_add_file_to_builder(
    gd_app_context_t app, gd_app_module_t module, dr_store_manage_t mgr, dr_metalib_builder_t builder,
    const char * name, const char * file)
{
    if (file == NULL) {
        APP_CTX_ERROR(
            app, "%s: load from file: file is NULL!",
            gd_app_module_name(module));
        return -1;
    }

    if (dr_metalib_builder_add_file(builder, name, file) == NULL) {
        if (name) {
            APP_CTX_ERROR(
                app, "%s: load from file: add source %s[%s] fail!",
                gd_app_module_name(module), file, name);
        }
        else {
            APP_CTX_ERROR(
                app, "%s: load from file: add source %s fail!",
                gd_app_module_name(module), file);
        }
        return -1;
    }

    if (mgr->m_debug) {
        if (name) {
            APP_CTX_INFO(
                app, "%s: load from file: add source %s[%s] success!",
                gd_app_module_name(module), file, name);
        }
        else {
            APP_CTX_INFO(
                app, "%s: load from file: add source %s success!",
                gd_app_module_name(module), file);
        }
    }

    return 0;    
}

static void dr_store_loader_lib_destory(LPDRMETALIB lib, void * ctx) {
    mem_free((mem_allocrator_t)ctx, lib);
}

static int dr_store_loader_load_from_file(
    gd_app_context_t app, gd_app_module_t module, dr_store_manage_t mgr, cfg_t cfg)
{
    struct cfg_it child_it;
    cfg_t child;
    dr_metalib_builder_t builder = NULL;
    void * buf = NULL;
    int rv;
    struct mem_buffer buffer;

    builder = dr_metalib_builder_create(gd_app_alloc(app), gd_app_em(app));
    if (builder == NULL) {
        APP_CTX_ERROR(app, "%s: load from file: create dr_metalib_builder fail!", gd_app_module_name(module));
        return -1;
    }

    mem_buffer_init(&buffer, 0);

    rv = 0;
    if (cfg_type(cfg) == CPE_CFG_TYPE_STRING) {
        rv = dr_store_loader_add_file_to_builder(app, module, mgr, builder, NULL, cfg_as_string(cfg, NULL));
    }
    else if (cfg_type(cfg) == CPE_CFG_TYPE_SEQUENCE) {
        cfg_it_init(&child_it, cfg);
        while((child = cfg_it_next(&child_it))) {
            if (dr_store_loader_add_file_to_builder(
                    app, module, mgr, builder, NULL, cfg_as_string(child, NULL)) != 0)
            {
                rv = -1;
            }
        }
    }
    else if (cfg_type(cfg) == CPE_CFG_TYPE_STRUCT) {
        cfg_it_init(&child_it, cfg);
        while((child = cfg_it_next(&child_it))) {
            if (dr_store_loader_add_file_to_builder(
                    app, module, mgr, builder, cfg_name(child), cfg_as_string(child, NULL)) != 0)
            {
                rv = -1;
            }
        }
    }
    else {
        APP_CTX_ERROR(
            app, "%s: load from file: not support cfg type %d!",
            gd_app_module_name(module), cfg_type(cfg));
        rv = -1;
    }

    if (rv != 0) goto FROM_FILE_COMPLETE;

    dr_metalib_builder_analize(builder);
    if (dr_inbuild_build_lib(
            &buffer,
            dr_metalib_bilder_lib(builder),
            gd_app_em(app)) != 0)
    {
        APP_CTX_ERROR(
            app, "%s: load from file: build metalib fail!",
            gd_app_module_name(module));
        rv = -1;
        goto FROM_FILE_COMPLETE;
    }

    buf = mem_alloc(gd_app_alloc(app), mem_buffer_size(&buffer));
    if (buf == NULL) {
        APP_CTX_ERROR(
            app, "%s: load from file: alloc buf to store metalib fail, size is %d!",
            gd_app_module_name(module), (int)mem_buffer_size(&buffer));
        rv = -1;
        goto FROM_FILE_COMPLETE;
    }

    if (mem_buffer_read(buf, mem_buffer_size(&buffer), &buffer) != mem_buffer_size(&buffer)) {
        APP_CTX_ERROR(
            app, "%s: load from file: copy to save buffer fail!",
            gd_app_module_name(module));
        rv = -1;
        goto FROM_FILE_COMPLETE;
    }

    if (dr_store_add_lib(
            mgr, gd_app_module_name(module),
            (LPDRMETALIB)buf, dr_store_loader_lib_destory, gd_app_alloc(app)) != 0)
    {
        rv = -1;
        goto FROM_FILE_COMPLETE;
    }

    buf = NULL;

FROM_FILE_COMPLETE:
    if (rv == 0) {
        if (mgr->m_debug) {
            APP_CTX_INFO(app, "%s: metalib load from files: success!", gd_app_module_name(module));
        }
    }

    if (buf) mem_free(gd_app_alloc(app), buf);

    mem_buffer_clear(&buffer);
    dr_metalib_builder_free(builder);

    return rv;
}

static int dr_store_loader_load_from_bin(
    gd_app_context_t app, gd_app_module_t module, dr_store_manage_t mgr, const char * arg)
{
    ssize_t buf_size, load_size;
    char * buf = NULL;
    FILE * fp = NULL;
    int rv = 0;

    fp = file_stream_open(arg, "r", gd_app_em(app));
    if (fp == NULL) {
        APP_CTX_ERROR(
            app, "%s: read load-from-bin %s: open file error!",
            gd_app_module_name(module), arg);
        rv = -1;
        goto FROM_BIN_COMPLETE;
    }

    buf_size = file_stream_size(fp, gd_app_em(app));
    if (buf_size <= 0) {
        APP_CTX_ERROR(
            app, "%s: read load-from-bin %s: read size error!",
            gd_app_module_name(module), arg);
        rv = -1;
        goto FROM_BIN_COMPLETE;
    }

    buf = mem_alloc(gd_app_alloc(app), buf_size);
    if (buf == NULL) {
        APP_CTX_ERROR(
            app, "%s: load from bin %s: alloc buf to store metalib fail, size is %d!",
            gd_app_module_name(module), arg, (int)buf_size);
        rv = -1;
        goto FROM_BIN_COMPLETE;
    }

    load_size = file_stream_load_to_buf(buf, buf_size, fp, gd_app_em(app));
    if (load_size != buf_size) {
        APP_CTX_ERROR(
            app, "%s: load from bin %s: load data to buf fail, reque %d, but load %d!",
            gd_app_module_name(module), arg, (int)buf_size, (int)load_size);
        rv = -1;
        goto FROM_BIN_COMPLETE;
    }

    if (dr_store_add_lib(
            mgr, gd_app_module_name(module),
            (LPDRMETALIB)buf, dr_store_loader_lib_destory, gd_app_alloc(app)) != 0)
    {
        rv = -1;
        goto FROM_BIN_COMPLETE;
    }

    buf = NULL;

FROM_BIN_COMPLETE:
    if (fp) file_stream_close(fp, gd_app_em(app));
    if (buf) mem_free(gd_app_alloc(app), buf);

    if (rv == 0) {
        if (mgr->m_debug) {
            APP_CTX_INFO(app, "%s: metalib load from bin %s success!", gd_app_module_name(module), arg);
        }
    }

    return rv;
}

EXPORT_DIRECTIVE
int dr_store_loader_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    const char * arg;
    cfg_t child_cfg;
    dr_store_manage_t mgr = NULL;

    mgr = dr_store_manage_default(app);
    if (mgr == NULL) {
        APP_CTX_ERROR(app, "%s: dr_store_manage_default not exist!", gd_app_module_name(module));
        return -1;
    }

    if ((arg = cfg_get_string(cfg, "load-from-symbol", NULL))) {
        return dr_store_loader_load_from_symbol(app, module, mgr, arg);
    }

    if ((arg = cfg_get_string(cfg, "load-from-bin", NULL))) {
        return dr_store_loader_load_from_bin(app, module, mgr, arg);
    }

    if ((child_cfg = cfg_find_cfg(cfg, "load-from-file"))) {
        return dr_store_loader_load_from_file(app, module, mgr, child_cfg);
    }

    APP_CTX_ERROR(app, "%s: no any load way!", gd_app_module_name(module));
    return -1;
}

EXPORT_DIRECTIVE
void dr_store_loader_app_fini(gd_app_context_t app, gd_app_module_t module) {
    dr_store_manage_t mgr;

    mgr = dr_store_manage_default(app);
    if (mgr) {
        dr_store_t store;
        store = dr_store_find(mgr, gd_app_module_name(module));
        if (store) {
            dr_store_free(store);
        }
    }
}

