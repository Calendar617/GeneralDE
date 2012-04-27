#ifndef GD_TIMER_INTERNAL_TYPES_H
#define GD_TIMER_INTERNAL_TYPES_H
#include "cpe/utils/hash.h"
#include "cpe/utils/range.h"
#include "cpe/tl/tl_types.h"
#include "gd/timer/timer_types.h"

enum timer_processor_state {
    timer_processor_state_NotInResponserHash = 0
    , timer_processor_state_InResponserHash
};

struct gd_timer_processor {
    gd_timer_id_t m_id;
    enum timer_processor_state m_state;
    tl_event_t m_tl_event;
    void * m_process_ctx;
    void * m_process_arg;
    void (*m_process_arg_free)(void *);
    gd_timer_process_fun_t m_process_fun;
    struct cpe_hash_entry m_hh_for_responser_to_processor;
};

struct gd_timer_mgr {
    gd_app_context_t m_app;
    mem_allocrator_t m_alloc;
    error_monitor_t m_em;
    tl_t m_tl;

    int m_debug;

    size_t m_timer_count_in_page;
    size_t m_timer_page_count;
    size_t m_timer_page_capacity;
    struct gd_timer_processor ** m_timer_buf;
    struct cpe_hash_table m_responser_to_processor;
    struct cpe_range_mgr m_ids;
};

#endif

