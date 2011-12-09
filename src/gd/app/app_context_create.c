#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "cpe/cfg/cfg_manage.h"
#include "gd/app/app_context.h"
#include "gd/dp/dp_manage.h"
#include "gd/tl/tl_manage.h"
#include "app_internal_types.h"

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
    mem_allocrator_t alloc, size_t capacity, error_monitor_t em,
    int argc, char * argv[])
{
    gd_app_context_t context;
    size_t allocSize;

    allocSize = sizeof(struct gd_app_context) + capacity;
    context = (gd_app_context_t)mem_alloc(alloc, allocSize);
    if (context == NULL) return NULL;

    bzero(context, allocSize);

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

    context->m_tl_mgr = gd_tl_manage_create(alloc);
    if (context->m_tl_mgr == NULL) {
        gd_app_context_free(context);
        return NULL;
    }

    context->m_dp_mgr = gd_dp_mgr_create(alloc, context->m_tl_mgr);
    if (context->m_dp_mgr == NULL) {
        gd_app_context_free(context);
        return NULL;
    }

    return context;
}

void gd_app_context_free(gd_app_context_t context) {
    if (context == NULL) return;

    if (context->m_dp_mgr) {
        gd_dp_mgr_free(context->m_dp_mgr);
        context->m_dp_mgr = NULL;
    }

    if (context->m_tl_mgr) {
        gd_tl_manage_free(context->m_tl_mgr);
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

