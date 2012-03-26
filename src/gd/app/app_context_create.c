#include "cpe/pal/pal_stdlib.h"
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_strings.h"
#include "cpe/pal/pal_unistd.h"
#include "cpe/cfg/cfg_manage.h"
#include "cpe/net/net_manage.h"
#include "gd/app/app_context.h"
#include "cpe/nm/nm_manage.h"
#include "cpe/dp/dp_manage.h"
#include "cpe/tl/tl_manage.h"
#include "app_internal_ops.h"

static int gd_app_parse_args(gd_app_context_t context, int argc, char * argv[]) {
    int i;

    for(i = 0; i < argc;) {
        char * arg = argv[i];

        if (strstr(arg, "--root=") == arg) {
            context->m_root = strdup(arg + strlen("--root="));
            ++i;
        }
        else {
            if (gd_app_add_arg(context, arg) != 0) return -1;
            ++i;
        }
    }

    return 0;
}

gd_app_context_t
gd_app_context_create(
    mem_allocrator_t alloc, size_t capacity,
    int argc, char * argv[])
{
    gd_app_context_t context;
    size_t allocSize;

    allocSize = sizeof(struct gd_app_context) + capacity;
    context = (gd_app_context_t)mem_alloc(alloc, allocSize);
    if (context == NULL) return NULL;

    bzero(context, allocSize);

    context->m_state = gd_app_init;

    cpe_error_monitor_init(&context->m_em_print, cpe_error_log_to_consol, NULL);
    context->m_em = &context->m_em_print;

    context->m_alloc = alloc;
    context->m_capacity = capacity;

    if (gd_app_parse_args(context, argc, argv) != 0) {
        gd_app_context_free(context);
        return NULL;
    }

    if (context->m_root == NULL) {
        context->m_root = getcwd(NULL, 0);
        if(context->m_root == NULL) {
            gd_app_context_free(context);
            return NULL;
        }
    }

    context->m_cfg = cfg_create(alloc);
    if (context->m_cfg == NULL) {
        gd_app_context_free(context);
        return NULL;
    }

    context->m_tl_mgr = tl_manage_create(alloc);
    if (context->m_tl_mgr == NULL) {
        gd_app_context_free(context);
        return NULL;
    }

    context->m_nm_mgr = nm_mgr_create(alloc);
    if (context->m_nm_mgr == NULL) {
        gd_app_context_free(context);
        return NULL;
    }

    context->m_dp_mgr = dp_mgr_create(alloc);
    if (context->m_dp_mgr == NULL) {
        gd_app_context_free(context);
        return NULL;
    }

    context->m_net_mgr = net_mgr_create(alloc, context->m_em);
    if (context->m_net_mgr == NULL) {
        gd_app_context_free(context);
        return NULL;
    }

    TAILQ_INIT(&context->m_runing_modules);
    TAILQ_INIT(&context->m_tick_chain);

    return context;
}

void gd_app_context_free(gd_app_context_t context) {
    if (context == NULL) return;

    gd_app_modules_unload(context);
    gd_app_tick_chain_free(context);

    if (context->m_net_mgr) {
        net_mgr_free(context->m_net_mgr);
        context->m_net_mgr = NULL;
    }

    if (context->m_nm_mgr) {
        nm_mgr_free(context->m_nm_mgr);
        context->m_nm_mgr = NULL;
    }

    if (context->m_dp_mgr) {
        dp_mgr_free(context->m_dp_mgr);
        context->m_dp_mgr = NULL;
    }

    if (context->m_tl_mgr) {
        tl_manage_free(context->m_tl_mgr);
        context->m_tl_mgr = NULL;
    }

    if (context->m_cfg) {
        cfg_free(context->m_cfg);
        context->m_cfg = NULL;
    }

    if (context->m_root) {
        free(context->m_root);
        context->m_root = NULL;
    }

    mem_free(context->m_alloc, context);
}
