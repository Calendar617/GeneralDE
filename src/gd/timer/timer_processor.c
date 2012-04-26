#include <assert.h>
#include "cpe/pal/pal_strings.h"
#include "cpe/pal/pal_stdio.h"
#include "cpe/tl/tl_action.h"
#include "gd/timer/timer_manage.h"
#include "timer_internal_ops.h"

int gd_timer_processor_free_id(gd_timer_mgr_t mgr, gd_timer_id_t processorId) {
    return cpe_range_put_one(&mgr->m_ids, processorId);
}

struct gd_timer_processor *
gd_timer_processor_get(gd_timer_mgr_t mgr, gd_timer_id_t processorId) {
    size_t pagePos;
    struct gd_timer_processor * processorPage;

    pagePos  = processorId / mgr->m_timer_count_in_page;
    assert(pagePos < mgr->m_timer_page_count);

    processorPage  = mgr->m_timer_buf[pagePos];
    return &processorPage[processorId % mgr->m_timer_count_in_page];
}

int gd_timer_processor_alloc(gd_timer_mgr_t mgr, gd_timer_id_t * id) {
    ptr_int_t newStart;
    struct gd_timer_processor * newPage;
    size_t i;

    if (!cpe_range_mgr_is_empty(&mgr->m_ids)) {
        *id = (gd_timer_id_t)cpe_range_get_one(&mgr->m_ids);
        return 0;
    }

    if (mgr->m_timer_page_count + 1 >= mgr->m_timer_page_capacity) {
        size_t newProcessorPageCapacity;
        struct gd_timer_processor ** newProcessorBuf;

        newProcessorPageCapacity = mgr->m_timer_page_count + 128;
        newProcessorBuf = (struct gd_timer_processor **)mem_alloc(mgr->m_alloc, sizeof(struct gd_timer_processor*) * newProcessorPageCapacity);
        if (newProcessorBuf == NULL) return -1;

        bzero(newProcessorBuf, sizeof(struct gd_timer_processor *) * newProcessorPageCapacity);
        memcpy(newProcessorBuf, mgr->m_timer_buf, sizeof(struct gd_timer_processor*) * mgr->m_timer_page_count);

        if (mgr->m_timer_buf) mem_free(mgr->m_alloc, mgr->m_timer_buf);

        mgr->m_timer_buf = newProcessorBuf;
        mgr->m_timer_page_capacity = newProcessorPageCapacity;

        if (mgr->m_debug) {
            CPE_INFO(mgr->m_em, "%s: gd_timer_processor_alloc: resize processor buf to "  FMT_SIZE_T, gd_timer_mgr_name(mgr), newProcessorPageCapacity);
        }
    }

    newStart = mgr->m_timer_page_count * mgr->m_timer_count_in_page;
    newPage = (struct gd_timer_processor *)mem_alloc(mgr->m_alloc, sizeof(struct gd_timer_processor) * mgr->m_timer_count_in_page);
    if (newPage == NULL) {
        return -1;
    }

    bzero(newPage, sizeof(struct gd_timer_processor) * mgr->m_timer_count_in_page);
    for(i = 0; i < mgr->m_timer_count_in_page; ++i) {
        newPage[i].m_id = newStart + i;
    }

    if (cpe_range_put_range(&mgr->m_ids, newStart, newStart + mgr->m_timer_count_in_page) != 0) {
        mem_free(mgr->m_alloc, newPage);
        return -1;
    }

    mgr->m_timer_buf[mgr->m_timer_page_count] = newPage;
    ++mgr->m_timer_page_count;

    if (mgr->m_debug) {
        CPE_INFO(
            mgr->m_em,
            "alloc a new processor page[%d,%d), page count is "  FMT_SIZE_T,
            (gd_timer_id_t)newStart, (gd_timer_id_t)(newStart + mgr->m_timer_page_count),
            mgr->m_timer_page_count);
    }

    *id = (gd_timer_id_t)cpe_range_get_one(&mgr->m_ids);
    return 0;
}

void gd_timer_mgr_free_processor_buf(gd_timer_mgr_t mgr) {
    size_t i;

    for(i = 0; i < mgr->m_timer_page_count; ++i) {
        mem_free(mgr->m_alloc, mgr->m_timer_buf[i]);
    }

    if (mgr->m_timer_buf)
        mem_free(mgr->m_alloc, mgr->m_timer_buf);

    mgr->m_timer_page_count = 0;
    mgr->m_timer_page_capacity = 0;
}

void gd_timer_processor_free_basic(gd_timer_mgr_t mgr, struct gd_timer_processor * data) {
    if (data->m_tl_event) {
        tl_event_free(data->m_tl_event);
        data->m_tl_event = NULL;
    }
    else {
        if (data->m_state == timer_processor_state_InResponserHash) {
            cpe_hash_table_remove_by_ins(&mgr->m_responser_to_processor, data);
            data->m_state = timer_processor_state_NotInResponserHash;
        }

        if (data->m_process_arg_free) data->m_process_arg_free(data->m_process_arg);

        data->m_process_ctx = NULL;
        data->m_process_arg = NULL;
        data->m_process_arg_free = NULL;
        data->m_process_fun = NULL;
    }
}

uint32_t gd_timer_processor_hash_fun(const struct gd_timer_processor * o) {
    return (((ptr_int_t)o->m_process_ctx) & 0xFFFFFFFF);
}

int gd_timer_processor_cmp_fun(const struct gd_timer_processor * l, const struct gd_timer_processor * r) {
    return l->m_process_ctx == r->m_process_ctx;
}
