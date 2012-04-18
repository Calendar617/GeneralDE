#ifndef GD_TIMER_INTERNAL_OPS_H
#define GD_TIMER_INTERNAL_OPS_H
#include "timer_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t gd_timer_processor_hash_fun(const struct gd_timer_processor * o);
int gd_timer_processor_cmp_fun(const struct gd_timer_processor * l, const struct gd_timer_processor * r);
void gd_timer_mgr_free_processor_buf(gd_timer_mgr_t mgr);

struct gd_timer_processor * gd_timer_processor_get(gd_timer_mgr_t mgr, gd_timer_id_t processorId);
int gd_timer_processor_alloc(gd_timer_mgr_t mgr, gd_timer_id_t * id);
void gd_timer_processor_free_basic(gd_timer_mgr_t mgr, struct gd_timer_processor * data);
int gd_timer_processor_free_id(gd_timer_mgr_t mgr, gd_timer_id_t processorId);

#ifdef __cplusplus
}
#endif

#endif

