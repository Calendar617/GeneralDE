#ifndef GD_TL_INTERNAL_OPS_H
#define GD_TL_INTERNAL_OPS_H
#include "tl_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_tl_event_node * gd_tl_event_node_alloc(gd_tl_t tl, size_t capacity);
void gd_tl_event_node_free(struct gd_tl_event_node * node);

int gd_tl_event_node_insert(struct gd_tl_event_node * node);

#ifdef __cplusplus
}
#endif

#endif


