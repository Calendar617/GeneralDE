#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_stdlib.h"
#include "cpe/utils/buffer.h"
#include "cpe/cfg/cfg_manage.h"
#include "gd/app/app_context.h"
#include "app_internal_types.h"

void gd_app_set_main(gd_app_context_t context, gd_app_fn_t fn_main, gd_app_fn_t fn_stop) {
    context->m_main = fn_main;
    context->m_stop = fn_stop;
}

void gd_app_set_em(gd_app_context_t context, error_monitor_t em) {
    context->m_em = em;
}

error_monitor_t gd_app_em(gd_app_context_t context) {
    return context->m_em;
}

mem_allocrator_t gd_app_alloc(gd_app_context_t context) {
    return context->m_alloc;
}

cfg_t gd_app_cfg(gd_app_context_t context) {
    return context->m_cfg;
}

tl_manage_t gd_app_tl_mgr(gd_app_context_t context) {
    return context->m_tl_mgr;
}

gd_dp_mgr_t gd_app_dp_mgr(gd_app_context_t context) {
    return context->m_dp_mgr;
}

gd_nm_mgr_t gd_app_nm_mgr(gd_app_context_t context) {
    return context->m_nm_mgr;
}

net_mgr_t gd_app_net_mgr(gd_app_context_t context) {
    return context->m_net_mgr;
}

int gd_app_argc(gd_app_context_t context) {
    return context->m_argc;
}

char ** gd_app_argv(gd_app_context_t context) {
    return context->m_argv;
}

int gd_app_add_arg(gd_app_context_t context, char * arg) {
    if (context->m_argc + 1 < GD_APP_MAX_ARGV) {
        context->m_argv[context->m_argc++] = arg;
        return 0;
    }
    else {
        return -1;
    }
}

int gd_app_context_argc(gd_app_context_t context) {
    return context->m_argc;
}

char ** gd_app_context_argv(gd_app_context_t context) {
    return context->m_argv;
}

void * gd_app_context_user_data(gd_app_context_t context) {
    return (void*)(context + 1);
}

int gd_app_set_root(gd_app_context_t context, const char * path) {
    char * buf;

    if (path == NULL || context == NULL) return -1;

    buf = strdup(path);
    if (buf == NULL) return -1;

    if (context->m_root) {
        free(context->m_root);
    }
    context->m_root = buf;
    return 0;
}

const char * gd_app_root(gd_app_context_t context) {
    return context->m_root;
}

int gd_app_cfg_reload(gd_app_context_t context) {
    int rv;
    struct mem_buffer tbuf;

    mem_buffer_init(&tbuf, context->m_alloc);

    mem_buffer_strcat(&tbuf, context->m_root);
    mem_buffer_strcat(&tbuf, "/etc");

    rv = cfg_read_dir(
        context->m_cfg,
        (char*)mem_buffer_make_continuous(&tbuf, 0),
        cfg_merge_use_new,
        context->m_em,
        context->m_alloc);

    if (rv == 0) {
        if (context->m_debug) {
            CPE_INFO(context->m_em, "load config from %s success!", (char*)mem_buffer_make_continuous(&tbuf, 0));
        }
    }

    mem_buffer_clear(&tbuf);

    return rv;
}

uint32_t gd_app_flags(gd_app_context_t app) {
    return app->m_flags;
}

void gd_app_flags_set(gd_app_context_t app, uint32_t flag) {
    app->m_flags = flag;
}

void gd_app_flag_enable(gd_app_context_t app, gd_app_flag_t flag) {
    app->m_flags |= flag;
}

void gd_app_flag_disable(gd_app_context_t app, gd_app_flag_t flag) {
    app->m_flags &= ~((uint32_t)flag);
}

int gd_app_flag_is_enable(gd_app_context_t app, gd_app_flag_t flag) {
    return app->m_flags & flag;
}

void gd_app_set_state(gd_app_context_t context, gd_app_status_t state) {
    context->m_state = state;
}

int gd_app_debug(gd_app_context_t context) {
    return context->m_debug;
}

void gd_app_set_debug(gd_app_context_t context, int level) {
    context->m_debug = level;
}

gd_app_status_t gd_app_state(gd_app_context_t context) {
    return context->m_state;
}

gd_app_context_t g_app_context = NULL;

