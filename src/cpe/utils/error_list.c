#include <string.h>
#include "cpe/pal/queue.h"
#include "cpe/utils/error_list.h"

#define CPE_ERROR_LIST_MSG_MAX_LEN 128

struct error_list_node {
    struct error_info m_info;
    char m_msg[CPE_ERROR_LIST_MSG_MAX_LEN];
    TAILQ_ENTRY(error_list_node) m_next;
};

struct error_list {
    TAILQ_HEAD(error_list_node_list, error_list_node) m_nodes;
    int m_count;
    mem_allocrator_t m_alloc;
};

error_list_t cpe_error_list_create(mem_allocrator_t alloc) {
    error_list_t el = (error_list_t)mem_alloc(alloc, sizeof(struct error_list));
    if (el == NULL) {
        return NULL;
    }

    TAILQ_INIT(&el->m_nodes);
    el->m_count = 0;
    el->m_alloc = alloc;

    return el;
}

void cpe_error_list_free(error_list_t el) {
    if (el == NULL) {
        return;
    }

    while(!TAILQ_EMPTY(&el->m_nodes)) {
        struct error_list_node * node = TAILQ_FIRST(&el->m_nodes);
        TAILQ_REMOVE(&el->m_nodes, node, m_next);
        mem_free(el->m_alloc, node);
    }

    mem_free(el->m_alloc, el);
}

void cpe_error_list_visit(error_list_t el, void(*p)(void * ctx, struct error_info * info, const char * msg), void * ctx) {
    struct error_list_node * n;

    if (el == NULL) {
        return;
    }

    TAILQ_FOREACH(n, &el->m_nodes, m_next) {
        p(ctx, &n->m_info, n->m_msg);
    }
}

int cpe_error_list_have_errno(error_list_t el, int e) {
    struct error_list_node * n;

    TAILQ_FOREACH(n, &el->m_nodes, m_next) {
        if (n->m_info.m_errno == e) {
            return 1;
        }
    }

    return 0;
}

int cpe_error_list_have_msg(error_list_t el, const char * partMsg) {
    struct error_list_node * n;

    TAILQ_FOREACH(n, &el->m_nodes, m_next) {
        if (strstr(n->m_msg, partMsg)) {
            return 1;
        }
    }

    return 0;
}

void cpe_error_list_collect(struct error_info * info, void * context, const char * fmt, va_list args) {
    error_list_t el = (error_list_t)context;
    struct error_list_node * node = mem_alloc(el->m_alloc, sizeof(struct error_list_node));
    if (node == NULL) {
        return;
    }

    memcpy(&node->m_info, info, sizeof(node->m_info));
    vsnprintf(node->m_msg, CPE_ERROR_LIST_MSG_MAX_LEN, fmt, args);

    ++el->m_count;
    TAILQ_INSERT_TAIL(&el->m_nodes, node, m_next);
}

int cpe_error_list_error_count(error_list_t el) {
    return el->m_count;
}
