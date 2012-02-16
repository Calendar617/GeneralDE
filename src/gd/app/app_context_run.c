#include <assert.h>
#include "cpe/cfg/cfg_manage.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/pal/pal_strings.h"
#include "cpe/pal/pal_unistd.h"
#include "gd/tl/tl_manage.h"
#include "gd/net/net_manage.h"
#include "gd/app/app_context.h"
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

static int gd_app_build_default_tickers(gd_app_context_t context) {
    struct cfg_it cfgs;
    cfg_t ticker_cfg;

    cfg_it_init(&cfgs, cfg_find_cfg(context->m_cfg, "tickers"));

    while((ticker_cfg = cfg_it_next(&cfgs))) {
        const char * ticker_name = NULL;
        cfg_t ticker_args = NULL;

        if (cfg_type(ticker_cfg) == CPE_CFG_TYPE_STRING) {
            ticker_name = cfg_as_string(ticker_cfg, NULL);
        }
        else if (cfg_type(ticker_cfg) == CPE_CFG_TYPE_STRUCT) {
            struct cfg_it childs;

            if (cfg_child_count(ticker_cfg) != 1) {
                CPE_WARNING(context->m_em, "read ticker: struct cfg too many child(%d)", cfg_child_count(ticker_cfg));
                continue;
            }

            cfg_it_init(&childs, ticker_cfg);
            ticker_cfg = cfg_it_next(&childs);
            assert(ticker_cfg);

            ticker_name = cfg_name(ticker_cfg);
            ticker_args = ticker_cfg;
        }
        else {
            CPE_WARNING(context->m_em, "read ticker: not support cfg type %d", cfg_type(ticker_cfg));
            continue;
        }

        assert(ticker_name);

        if (strcmp(ticker_name, "tl") == 0) {
            if (gd_app_tick_add(
                    context,
                    (gd_app_tick_fun)gd_tl_manage_tick,
                    context->m_tl_mgr,
                    cfg_get_int32(ticker_cfg, "count", 500)) != 0)
            {
                CPE_ERROR(context->m_em, "read ticker: add tl ticker fail!");
                return -1;
            }
        }
        else if (strcmp(ticker_name, "net") == 0) {
            if (gd_app_tick_add(
                    context,
                    (gd_app_tick_fun)gd_net_mgr_tick,
                    context->m_net_mgr,
                    0) != 0)
            {
                CPE_ERROR(context->m_em, "read ticker: add net ticker fail!");
                return -1;
            }
        }
        else {
            CPE_ERROR(context->m_em, "read ticker: unknown ticker type %s!", ticker_name);
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

    if (gd_app_cfg_reload(context) != 0) {
        g_app_context = NULL;
        return -1;
    }

    if (gd_app_build_default_tickers(context) != 0) {
        g_app_context = NULL;
        return -1;
    }

    if (gd_app_modules_load(context) != 0) {
        g_app_context = NULL;
        return -1;
    }

    if (context->m_main == NULL) {
        CPE_ERROR(context->m_em, "no main function to runing!");
        gd_app_modules_unload(context);
        g_app_context = NULL;
        return -1;
    }

    rv = context->m_main(context);

    gd_app_modules_unload(context);
    gd_app_tick_chain_free(context);
    g_app_context = NULL;

    return rv;
}
