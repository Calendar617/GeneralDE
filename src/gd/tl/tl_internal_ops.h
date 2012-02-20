#ifndef GD_TL_INTERNAL_OPS_H
#define GD_TL_INTERNAL_OPS_H
#include "tl_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*node operations*/
struct gd_tl_event_node * gd_tl_event_node_alloc(gd_tl_t tl, size_t capacity);
void gd_tl_event_node_free(struct gd_tl_event_node * node);
int gd_tl_event_node_insert(struct gd_tl_event_node * node);
int gd_tl_event_queue_clear(struct gd_tl_event_node_queue * queue);

#define gd_tl_event_to_node(e)                  \
    ((struct gd_tl_event_node *)                \
     ((char *)(e)                               \
      - (sizeof(struct gd_tl_event_node)        \
         - sizeof(struct gd_tl_event))))

#define gd_tl_event_node_remove_from_building_queue(i)  \
    TAILQ_REMOVE(                                       \
        &(i)->m_event.m_tl->m_manage->m_event_building_queue, \
        (i), m_next)

#ifdef __cplusplus
}
#endif

#endif


