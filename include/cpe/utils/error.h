#ifndef CPE_UTILS_ERRORMONITOR_H
#define CPE_UTILS_ERRORMONITOR_H
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CPE_ERROR_MSG_MAX_LEN 128

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

typedef struct error_monitor {
    void (*on_error)(struct error_info * info, void * context, const char * msg);
    void * m_context;
    struct error_info m_curent_location;
} * error_monitor_t;

#define _CPE_DO_ERROR_NOTIFY(monitor, level, en, format, args...)   \
    if (monitor) {                                                  \
        char __buf[CPE_ERROR_MSG_MAX_LEN];                          \
        snprintf(__buf, CPE_ERROR_MSG_MAX_LEN, format, ##args);     \
        monitor->m_curent_location.m_errno = en;                    \
        monitor->m_curent_location.m_level = level;                 \
        monitor->on_error(                                          \
            &monitor->m_curent_location,                            \
            monitor->m_context, __buf);                             \
    }

#define CPE_DEF_ERROR_MONITOR(name, fun, context) \
    struct error_monitor name = { fun, context, { NULL, -1, 0, CPE_EL_ERROR } }

#define CPE_ERROR_LINE(monitor, line)           \
    monitor->m_curent_location.m_line = line;

#define CPE_ERROR_FILE(monitor, file)           \
    monitor->m_curent_location.m_file = file;

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
