#include <assert.h>
#include "tl_internal_ops.h"

struct gd_tl_event_node *
gd_tl_event_node_alloc(gd_tl_t tl, size_t capacity) {
    struct gd_tl_event_node * node = 
        (struct gd_tl_event_node *)mem_alloc(
            tl->m_manage->m_alloc,
            sizeof(struct gd_tl_event_node) + capacity);
    if (node == NULL) return NULL;

    node->m_state = gd_tl_event_node_state_in_building_queue;
    node->m_event.m_tl = tl;
    node->m_event.m_capacity = capacity;

    if (tl->m_event_construct) {
        tl->m_event_construct(&node->m_event, tl->m_event_op_context);
    }

    TAILQ_INSERT_TAIL(&tl->m_events, node, m_next_in_tl);
    TAILQ_INSERT_HEAD(&tl->m_manage->m_event_building_queue, node, m_next);

    return node;
}

void gd_tl_event_node_free(struct gd_tl_event_node * node) {
    if (node == NULL) return;

    if (node->m_event.m_tl->m_event_destory) {
        node->m_event.m_tl->m_event_destory(
            &node->m_event,
            node->m_event.m_tl->m_event_op_context);
    }

    TAILQ_REMOVE(&node->m_event.m_tl->m_events, node, m_next_in_tl);

    if (node->m_state == gd_tl_event_node_state_in_building_queue) {
        TAILQ_REMOVE(&node->m_event.m_tl->m_manage->m_event_building_queue, node, m_next);
    }
    else if (node->m_state == gd_tl_event_node_state_in_event_queue) {
        TAILQ_REMOVE(&node->m_event.m_tl->m_manage->m_event_queue, node, m_next);
    }

    mem_free(node->m_event.m_tl->m_manage->m_alloc, node);
}

int gd_tl_event_node_insert(struct gd_tl_event_node * node) {
    struct gd_tl_event_node * insertPos;
    gd_tl_manage_t tm;

    assert(node);
    assert(node->m_event.m_tl);
    assert(node->m_event.m_tl->m_manage);

    tm = node->m_event.m_tl->m_manage;

    insertPos = TAILQ_FIRST(&tm->m_event_queue);
    while(insertPos != TAILQ_END(&tm->m_event_queue)
          && insertPos->m_execute_time <= node->m_execute_time)
    {
        insertPos = TAILQ_NEXT(insertPos, m_next);
    }

    if (insertPos == TAILQ_END(&tm->m_event_queue)) {
        TAILQ_INSERT_TAIL(&tm->m_event_queue, node, m_next);
    }
    else {
        TAILQ_INSERT_BEFORE(insertPos, node, m_next);
    }

    node->m_state = gd_tl_event_node_state_in_event_queue;

    return 0;
}

int gd_tl_event_queue_clear(struct gd_tl_event_node_queue * queue) {
    int count = 0;

    assert(queue);

    while(!TAILQ_EMPTY(queue)) {
        gd_tl_event_node_free(TAILQ_FIRST(queue));
        ++count;
    }

    return count;
}

