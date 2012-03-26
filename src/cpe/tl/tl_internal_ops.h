#ifndef CPE_TL_INTERNAL_OPS_H
#define CPE_TL_INTERNAL_OPS_H
#include "tl_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*node operations*/
struct tl_event_node * tl_event_node_alloc(tl_t tl, size_t capacity);
void tl_event_node_free(struct tl_event_node * node);
int tl_event_node_insert(struct tl_event_node * node);
int tl_event_queue_clear(struct tl_event_node_queue * queue);

#define tl_event_to_node(e)                  \
    ((struct tl_event_node *)                \
     ((char *)(e)                               \
      - (sizeof(struct tl_event_node)        \
         - sizeof(struct tl_event))))

#define tl_event_node_remove_from_building_queue(i)  \
    TAILQ_REMOVE(                                       \
        &(i)->m_event.m_tl->m_manage->m_event_building_queue, \
        (i), m_next)

#ifdef __cplusplus
}
#endif

#endif


