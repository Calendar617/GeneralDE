#ifndef CPE_UTILS_ERRORMONITOR_H
#define CPE_UTILS_ERRORMONITOR_H
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CPE_EL_INFO,
    CPE_EL_WARNING,
    CPE_EL_ERROR
} error_level_t;

struct error_info {
    const char * m_file;
    int m_line;
    int m_errno;
    error_level_t m_level;
};

struct error_monitor_node {
    void (*on_error)(struct error_info * info, void * context, const char * fmt, va_list args);
    void * m_context;
    struct error_monitor_node * m_next;
};

typedef struct error_monitor {
    struct error_monitor_node m_node;
    struct error_info m_curent_location;
} * error_monitor_t;

/*utils functions*/
void cpe_error_log_to_file(struct error_info * info, void * context, const char * fmt, va_list args);
void cpe_error_log_to_consol(struct error_info * info, void * context, const char * fmt, va_list args);
void cpe_error_save_last_errno(struct error_info * info, void * context, const char * fmt, va_list args);

/*operations*/
void cpe_error_do_notify(error_monitor_t monitor, const char * fmt, ...);
void cpe_error_do_notify_var(error_monitor_t monitor, const char * fmt, va_list args);
void cpe_error_monitor_add_node(error_monitor_t monitor, struct error_monitor_node * node);
void cpe_error_monitor_remove_node(error_monitor_t monitor, struct error_monitor_node * node);

#define _CPE_DO_ERROR_NOTIFY(monitor, level, en, format, args...)   \
    if (monitor) {                                                  \
        monitor->m_curent_location.m_errno = en;                    \
        monitor->m_curent_location.m_level = level;                 \
        cpe_error_do_notify(monitor, format, ##args);               \
    }

#define CPE_DEF_ERROR_MONITOR(name, fun, context) \
    struct error_monitor name = { { fun, context, NULL }, { NULL, -1, 0, CPE_EL_ERROR } }

#define CPE_DEF_ERROR_MONITOR_ADD(name, monitor, fun, context)          \
    struct error_monitor_node name = { fun, context, NULL };            \
    cpe_error_monitor_add_node(monitor, &name);

#define CPE_DEF_ERROR_MONITOR_REMOVE(name, monitor)                     \
    cpe_error_monitor_remove_node(monitor, &name);

#define CPE_ERROR_SET_LINE(monitor, line) monitor->m_curent_location.m_line = line
#define CPE_ERROR_SET_FILE(monitor, file) monitor->m_curent_location.m_file = file
#define CPE_ERROR_SET_ERRNO(monitor, e) monitor->m_curent_location.m_errno = e
#define CPE_ERROR_SET_LEVEL(monitor, l) monitor->m_curent_location.m_level = l

#define CPE_INFO_NOTIFY(monitor, format, args...)               \
    _CPE_DO_ERROR_NOTIFY(monitor, CPE_EL_INFO, 0, format, ##args)

#define CPE_WARNING(monitor, format, args...)                       \
    _CPE_DO_ERROR_NOTIFY(monitor, CPE_EL_WARNING, -1, format, ##args)

#define CPE_ERROR(monitor, format, args...)                         \
    _CPE_DO_ERROR_NOTIFY(monitor, CPE_EL_ERROR, -1, format, ##args)

#define CPE_WARNING_EX(monitor, en, format, args...)                \
    _CPE_DO_ERROR_NOTIFY(monitor, CPE_EL_WARNING, en, format, ##args)

#define CPE_ERROR_EX(monitor, en, format, args...)                  \
    _CPE_DO_ERROR_NOTIFY(monitor, CPE_EL_ERROR, en, format, ##args)

#ifdef __cplusplus
}
#endif

#endif
