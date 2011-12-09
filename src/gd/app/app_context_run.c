#include <strings.h>
#include <unistd.h>
#include "cpe/cfg/cfg_manage.h"
#include "gd/app/app_context.h"
#include "app_internal_types.h"

static int gd_app_chdir_to_root(gd_app_context_t context) {
    if (context->m_root) {
        if (chdir(context->m_root) != 0) {
            CPE_ERROR(gd_app_em(context), "change root to %s fail!", context->m_root);
            return -1;
        }
    }

    return 0;
}

static int gd_app_run_i(gd_app_context_t context) {
    int rv;

    if (gd_app_chdir_to_root(context) != 0) return -1;
    if (gd_app_cfg_reload(context) != 0) return -1;

    rv = context->m_main(context);

    return -1;
}

int gd_app_run(gd_app_context_t context) {
    int rv;

    if (context == NULL || context->m_main == NULL) return -1;

    if (g_app_context != NULL) {
        CPE_ERROR(context->m_em, "app already runing!");
    }

    g_app_context = context;
    rv = gd_app_run_i(context);
    g_app_context = NULL;

    return rv;
}
