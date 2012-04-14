#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/pal/pal_stdio.h"
#include "cpe/utils/stream_error.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/nm/nm_manage.h"
#include "cpe/nm/nm_read.h"
#include "cpe/tl/tl_manage.h"
#include "cpe/tl/tl_action.h"
#include "gd/app/app_log.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "gd/app/app_tl.h"
#include "gd/timer/timer_manage.h"
#include "timer_internal_ops.h"

cpe_hash_string_t s_gd_timer_mgr_default_name;
struct nm_node_type s_nm_node_type_gd_timer_mgr;
static void gd_timer_mgr_dispatch_timer(tl_event_t input, void * context);
static void gd_timer_mgr_destory_timer(tl_event_t event, void * context);

gd_timer_mgr_t
gd_timer_mgr_create(
    gd_app_context_t app,
    const char * name,
    const char * tl_name,
    mem_allocrator_t alloc,
    error_monitor_t em)
{
    gd_timer_mgr_t mgr;
    tl_manage_t tl;
    nm_node_t mgr_node;

    if (name == 0) name = cpe_hs_data(s_gd_timer_mgr_default_name);

    if (em == NULL) em = gd_app_em(app);

    mgr_node = nm_instance_create(gd_app_nm_mgr(app), name, sizeof(struct gd_timer_mgr));
    if (mgr_node == NULL) return NULL;

    mgr = (gd_timer_mgr_t)nm_node_data(mgr_node);
    mgr->m_alloc = alloc;
    mgr->m_app = app;
    mgr->m_em = em;
    mgr->m_debug = 0;
    mgr->m_timer_count_in_page = 2048;
    mgr->m_timer_page_count = 0;
    mgr->m_timer_page_capacity = 0;
    mgr->m_timer_buf = NULL;

    if (cpe_range_mgr_init(&mgr->m_ids, alloc) != 0) {
        CPE_ERROR(em, "gd_timer_mgr_create: init range mgr!");
        nm_node_free(mgr_node);
        return NULL;
    }

    tl = app_tl_manage_find(app, tl_name);
    if (tl == NULL) {
        CPE_ERROR(em, "gd_timer_mgr_create: tl %s not exist!", tl_name ? tl_name : "default");
        cpe_range_mgr_fini(&mgr->m_ids);
        nm_node_free(mgr_node);
        return NULL;
    }

    mgr->m_tl = tl_create(tl);
    if (mgr->m_tl == NULL) {
        CPE_ERROR(em, "gd_timer_mgr_create: create tl fail!");
        cpe_range_mgr_fini(&mgr->m_ids);
        nm_node_free(mgr_node);
        return NULL;
    }

    tl_set_opt(mgr->m_tl, tl_set_event_dispatcher, gd_timer_mgr_dispatch_timer);
    tl_set_opt(mgr->m_tl, tl_set_event_op_context, mgr);
    tl_set_opt(mgr->m_tl, tl_set_event_destory, gd_timer_mgr_destory_timer);

    if (cpe_hash_table_init(
            &mgr->m_responser_to_processor,
            alloc,
            (cpe_hash_fun_t) gd_timer_processor_hash_fun,
            (cpe_hash_cmp_t) gd_timer_processor_cmp_fun,
            CPE_HASH_OBJ2ENTRY(gd_timer_processor, m_hh_for_responser_to_processor),
            -1) != 0)
    {
        CPE_ERROR(em, "gd_timer_mgr_create: init responser hash table fail!");
        tl_free(mgr->m_tl);
        cpe_range_mgr_fini(&mgr->m_ids);
        nm_node_free(mgr_node);
        return NULL;
    }

    nm_node_set_type(mgr_node, &s_nm_node_type_gd_timer_mgr);

    return mgr;
}

static void gd_timer_mgr_clear(nm_node_t node) {
    gd_timer_mgr_t mgr;
    mgr = (gd_timer_mgr_t)nm_node_data(node);

    tl_free(mgr->m_tl);

    gd_timer_mgr_free_processor_buf(mgr);

    cpe_range_mgr_fini(&mgr->m_ids);

    cpe_hash_table_fini(&mgr->m_responser_to_processor);
}

void gd_timer_mgr_free(gd_timer_mgr_t mgr) {
    nm_node_t mgr_node;
    assert(mgr);

    mgr_node = nm_node_from_data(mgr);
    if (nm_node_type(mgr_node) != &s_nm_node_type_gd_timer_mgr) return;
    nm_node_free(mgr_node);
}

gd_timer_mgr_t gd_timer_mgr_find(gd_app_context_t app, cpe_hash_string_t name) {
    nm_node_t node;

    if (name == NULL) name = s_gd_timer_mgr_default_name;

    node = nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || nm_node_type(node) != &s_nm_node_type_gd_timer_mgr) return NULL;
    return (gd_timer_mgr_t)nm_node_data(node);
}

gd_timer_mgr_t gd_timer_mgr_find_nc(gd_app_context_t app, const char * name) {
    nm_node_t node;
    if (name == NULL) return gd_timer_mgr_default(app);

    node = nm_mgr_find_node_nc(gd_app_nm_mgr(app), name);
    if (node == NULL || nm_node_type(node) != &s_nm_node_type_gd_timer_mgr) return NULL;
    return (gd_timer_mgr_t)nm_node_data(node);
}

gd_timer_mgr_t gd_timer_mgr_default(gd_app_context_t app) {
    return gd_timer_mgr_find(app, s_gd_timer_mgr_default_name);
}

gd_app_context_t gd_timer_mgr_app(gd_timer_mgr_t mgr) {
    return mgr->m_app;
}

const char * gd_timer_mgr_name(gd_timer_mgr_t mgr) {
    return nm_node_name(nm_node_from_data(mgr));
}

cpe_hash_string_t
gd_timer_mgr_name_hs(gd_timer_mgr_t mgr) {
    return nm_node_name_hs(nm_node_from_data(mgr));
}

tl_t gd_timer_mgr_tl(gd_timer_mgr_t mgr) {
    return mgr->m_tl;
}

int gd_timer_mgr_regist_timer(
    gd_timer_mgr_t mgr, 
    gd_timer_id_t * id,
    gd_timer_process_fun_t fun, void * ctx,
    void * arg, void (*arg_fini)(void *),
    tl_time_span_t delay, tl_time_span_t span, int repeatCount)
{
    gd_timer_id_t newProcessorId;
    struct gd_timer_processor * newProcessorData;

    if (gd_timer_processor_alloc(mgr, &newProcessorId) != 0) {
        return -1;
    }

    newProcessorData = gd_timer_processor_get(mgr, newProcessorId);
    assert(newProcessorData);
    assert(newProcessorData->m_process_ctx == NULL);
    assert(newProcessorData->m_state == timer_processor_state_NotInResponserHash);

    newProcessorData->m_process_ctx = ctx;
    newProcessorData->m_process_arg = arg;
    newProcessorData->m_process_arg_free = arg_fini;
    newProcessorData->m_process_fun = fun;

    newProcessorData->m_tl_event = tl_event_create(mgr->m_tl, sizeof(gd_timer_id_t));
    if (newProcessorData->m_tl_event == NULL) {
        gd_timer_processor_free_basic(mgr, newProcessorData);
        gd_timer_processor_free_id(mgr, newProcessorId);
        CPE_ERROR(
            mgr->m_em, "%s: regist processor: create tl_event fail!",
            gd_timer_mgr_name(mgr));
        return -1;
    }
    *(gd_timer_id_t*)tl_event_data(newProcessorData->m_tl_event) = newProcessorId;

    if (cpe_hash_table_insert(&mgr->m_responser_to_processor, newProcessorData) != 0) {
        gd_timer_processor_free_basic(mgr, newProcessorData);
        gd_timer_processor_free_id(mgr, newProcessorId);
        CPE_ERROR(
            mgr->m_em, "%s: regist processor: insert to responser processor list fail!",
            gd_timer_mgr_name(mgr));
        return -1;
    }
    newProcessorData->m_state = timer_processor_state_InResponserHash;

    if (tl_event_send_ex(newProcessorData->m_tl_event, delay, span, repeatCount) != 0) {
        gd_timer_processor_free_basic(mgr, newProcessorData);
        gd_timer_processor_free_id(mgr, newProcessorId);
        CPE_ERROR(
            mgr->m_em, "%s: regist processor: send event to tl fail!",
            gd_timer_mgr_name(mgr));
        return -1;
    }

    if (id) *id = newProcessorId;

    return 0;
}

void gd_timer_mgr_unregist_timer_by_ctx(gd_timer_mgr_t mgr, void * ctx) {
    struct gd_timer_processor key;
    struct gd_timer_processor * node;

    key.m_process_ctx = ctx;

    node = (struct gd_timer_processor *)cpe_hash_table_find(&mgr->m_responser_to_processor, &key);
    while(node) {
        struct gd_timer_processor * next = 
            cpe_hash_table_find_next(&mgr->m_responser_to_processor, node);
        assert(node->m_process_ctx);

        gd_timer_processor_free_id(mgr, node->m_id);
        gd_timer_processor_free_basic(mgr, node);

        node = next;
    }
}

void gd_timer_mgr_unregist_timer_by_id(gd_timer_mgr_t mgr, gd_timer_id_t timer_id) {
    struct gd_timer_processor * timer;
    timer = gd_timer_processor_get(mgr, timer_id);

    if (timer == NULL) {
        CPE_ERROR(
            mgr->m_em, "%s: unregister timer by id: %d not a valid timer!",
            gd_timer_mgr_name(mgr), timer_id);
    }
    else if (timer->m_process_ctx == NULL) {
        CPE_ERROR(
            mgr->m_em, "%s: unregister timer by id: %d not a allocked timer!",
            gd_timer_mgr_name(mgr), timer_id);
    }
    else {
        assert(timer->m_process_ctx);
        gd_timer_processor_free_id(mgr, timer->m_id);
        gd_timer_processor_free_basic(mgr, timer);
    }
}

int gd_timer_mgr_have_timer(gd_timer_mgr_t mgr, gd_timer_id_t timer_id) {
    struct gd_timer_processor * timerPage;
    int pagePos;

    pagePos = timer_id / mgr->m_timer_count_in_page;
    if (pagePos >= (int)mgr->m_timer_page_count) return 0;

    timerPage = mgr->m_timer_buf[pagePos];

    return timerPage[timer_id % mgr->m_timer_count_in_page].m_process_ctx ? 1 : 0;
}

static void gd_timer_mgr_destory_timer(tl_event_t event, void * context) {
    gd_timer_mgr_t mgr;
    gd_timer_id_t timerId;
    struct gd_timer_processor * timer;

    mgr = (gd_timer_mgr_t)context;

    timerId = *(gd_timer_id_t*)tl_event_data(event);

    timer = gd_timer_processor_get(mgr, timerId);
    if (timer == NULL) {
        CPE_ERROR(mgr->m_em, "%s: destory timer: timer(id=%d) not exist!", gd_timer_mgr_name(mgr), timerId);
        return;
    }

    if (timer->m_tl_event != event) {
        CPE_ERROR(mgr->m_em, "%s: destory timer: timer(id=%d) tl_event mismatch!", gd_timer_mgr_name(mgr), timerId);
        return;
    }

    timer->m_tl_event = NULL;
    gd_timer_processor_free_basic(mgr, timer);
    gd_timer_processor_free_id(mgr, timerId);
}

static void gd_timer_mgr_dispatch_timer(tl_event_t input, void * context) {
    gd_timer_mgr_t mgr;
    gd_timer_id_t timerId;
    struct gd_timer_processor * timer;

    mgr = (gd_timer_mgr_t)context;
    assert(mgr);

    timerId = *(gd_timer_id_t*)tl_event_data(input);

    timer = gd_timer_processor_get(mgr, timerId);
    if (timer == NULL) {
        CPE_ERROR(mgr->m_em, "%s: dispatch timer: get timer(id=%d) fail!", gd_timer_mgr_name(mgr), timerId);
        return;
    }

    timer->m_process_fun(timer->m_process_ctx, timerId, timer->m_process_arg);
}

CPE_HS_DEF_VAR(s_gd_timer_mgr_default_name, "gd_timer_mgr");

CPE_HS_DEF_VAR(gd_timer_req_type_name, "app.event.req");

struct nm_node_type s_nm_node_type_gd_timer_mgr = {
    "gd_timer_mgr",
    gd_timer_mgr_clear
};

EXPORT_DIRECTIVE
int gd_timer_mgr_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    gd_timer_mgr_t gd_timer_mgr;

    gd_timer_mgr =
        gd_timer_mgr_create(
            app,
            gd_app_module_name(module),
            cfg_get_string(cfg, "tl", NULL),
            gd_app_alloc(app),
            gd_app_em(app));
    if (gd_timer_mgr == NULL) return -1;

    gd_timer_mgr->m_debug = cfg_get_int32(cfg, "debug", 0);

    if (gd_timer_mgr->m_debug) {
        CPE_INFO(
            gd_app_em(app), "%s: create: done",
            gd_timer_mgr_name(gd_timer_mgr));
    }

    return 0;
}

EXPORT_DIRECTIVE
void gd_timer_mgr_app_fini(gd_app_context_t app, gd_app_module_t module) {
    gd_timer_mgr_t gd_timer_mgr;

    gd_timer_mgr = gd_timer_mgr_find_nc(app, gd_app_module_name(module));
    if (gd_timer_mgr) {
        gd_timer_mgr_free(gd_timer_mgr);
    }
}

