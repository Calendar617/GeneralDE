#include <stdio.h>
#include "cpe/utils/error.h"

void cpe_error_do_notify(error_monitor_t monitor, const char * fmt, ...) {
    struct error_monitor_node * node = &monitor->m_node;
    va_list args;

    va_start(args, fmt);

    while(node) {
            node->on_error(&monitor->m_curent_location, node->m_context, fmt, args);
            node = node->m_next;
    }

    va_end(args);
}

void cpe_error_do_notify_var(error_monitor_t monitor, const char * fmt, va_list args) {
    struct error_monitor_node * node = &monitor->m_node;

    while(node) {
            node->on_error(&monitor->m_curent_location, node->m_context, fmt, args);
            node = node->m_next;
    }
}

void cpe_error_monitor_add_node(error_monitor_t monitor, struct error_monitor_node * node) {
    struct error_monitor_node * lastNode = &monitor->m_node;
    while(lastNode->m_next) {
        lastNode = lastNode->m_next;
    }

    lastNode->m_next = node;
    node->m_next = NULL;
}

void cpe_error_monitor_remove_node(error_monitor_t monitor, struct error_monitor_node * removeNode) {
    struct error_monitor_node * node = &monitor->m_node;
    while(node->m_next) {
        if (node->m_next == removeNode) {
            node->m_next = removeNode->m_next;
        }
        else {
            node = node->m_next;
        }
    }
}

void cpe_error_log_to_file(struct error_info * info, void * context, const char * fmt, va_list args) {
    if (context == NULL) {
        return;
    }
    vfprintf((FILE *)context, fmt, args);
}

void cpe_error_log_to_consol(struct error_info * info, void * context, const char * fmt, va_list args) {
    vprintf(fmt, args);
    printf("\n");
}
