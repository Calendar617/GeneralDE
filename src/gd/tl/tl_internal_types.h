#ifndef GD_TL_INTERNAL_TYPES_H
#define GD_TL_INTERNAL_TYPES_H
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/memory.h"
#include "gd/tl/tl_types.h"

#define GD_TL_TL_MAX 10
#define GD_TL_ACTION_MAX (128)
#define GD_TL_ACTION_SIZE (128)

#ifdef __cplusplus
extern "C" {
#endif

struct gd_tl {
    gd_tl_manage_t m_manage;

    gd_tl_event_enqueue_t m_event_enqueue;
    gd_tl_event_process_t m_event_dispatcher;
    gd_tl_event_process_t m_event_construct;
    gd_tl_event_process_t m_event_destory;
    void * m_event_op_context;
};

struct gd_tl_event {
    gd_tl_t m_tl;
    size_t m_capacity;
};

struct gd_tl_free_event {
    mem_allocrator_t m_alloc;
    struct gd_tl_event m_event;
};

struct gd_tl_event_node {
    TAILQ_ENTRY(gd_tl_event_node) m_next;

    gd_tl_time_t m_execute_time;
    gd_tl_time_span_t m_span;
    int m_repeatCount;

    struct gd_tl_event m_event;
};

TAILQ_HEAD(gd_tl_event_node_queue, gd_tl_event_node);

union gd_tl_action {
    struct gd_tl_event m_event;
    char m_reserve[GD_TL_ACTION_SIZE];
};

struct gd_tl_manage {
    mem_allocrator_t m_alloc;

    /*time*/
    gd_tl_time_fun_t m_time_get;
    gd_tl_time_cvt_fun_t m_time_cvt;
    void * m_time_ctx;
    gd_tl_time_t m_time_current;

    /*tl*/
    int m_tl_count;
    struct gd_tl m_tls[GD_TL_TL_MAX];

    /*action*/
    int m_action_begin_pos;
    int m_action_end_pos;
    union gd_tl_action m_action_queue[GD_TL_ACTION_MAX];

    /*event*/
    struct gd_tl_event_node_queue m_event_queue;
    struct gd_tl_event_node_queue m_event_building_queue;
};

#ifdef __cplusplus
}
#endif

#endif
