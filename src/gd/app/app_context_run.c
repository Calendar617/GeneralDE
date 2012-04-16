#include <assert.h>
#include "cpe/pal/pal_strings.h"
#include "cpe/pal/pal_unistd.h"
#include "cpe/cfg/cfg_manage.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/net/net_manage.h"
#include "cpe/tl/tl_manage.h"
#include "gd/app/app_context.h"
#include "gd/app/app_tl.h"
#include "app_internal_ops.h"

static int gd_app_chdir_to_root(gd_app_context_t context) {
    if (context->m_root) {
        if (chdir(context->m_root) != 0) {
            CPE_ERROR(gd_app_em(context), "change root to %s fail!", context->m_root);
            return -1;
        }
    }

    return 0;
}

int gd_app_run(gd_app_context_t context) {
    int rv;

    assert(context);

    if (g_app_context != NULL) {
        CPE_ERROR(context->m_em, "app already runing!");
        return -1;
    }

    g_app_context = context;

    if (gd_app_chdir_to_root(context) != 0) {
        g_app_context = NULL;
        return -1;
    }

    if (!gd_app_flag_is_enable(context, gd_app_flag_no_auto_load)) {
        if (gd_app_cfg_reload(context) != 0) {
            g_app_context = NULL;
            return -1;
        }

        if (gd_app_modules_load(context) != 0) {
            g_app_context = NULL;
            return -1;
        }
    }

    if (context->m_main == NULL) {
        CPE_ERROR(context->m_em, "no main function to runing!");
        gd_app_modules_unload(context);
        g_app_context = NULL;
        return -1;
    }

    rv = context->m_main(context, context->m_fun_ctx);

    gd_app_modules_unload(context);
    gd_app_tick_chain_free(context);
    g_app_context = NULL;

    return rv;
}

int gd_app_stop(gd_app_context_t context) {
    if (context->m_stop) return context->m_stop(context, context->m_fun_ctx);
    CPE_ERROR(context->m_em, "app no stop function!");
    return -1;
}
