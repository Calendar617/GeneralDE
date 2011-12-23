#ifndef CPE_UTILS_LOG_H_
#define CPE_UTILS_LOG_H_
#include "log4c.h"

#define LOG_ERROR(category, msg, args...) \
    { \
    const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
    log4c_category_log_locinfo(mycat, &locinfo, LOG4C_PRIORITY_ERROR, msg, ##args); \
    }

#define LOG_WARN(category, msg, args...) \
    { \
    const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
    log4c_category_log_locinfo(mycat, &locinfo, LOG4C_PRIORITY_WARN, msg, ##args); \
    } 
 
#define LOG_INFO(category, msg, args...) \
    { \
    const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL); \
    log4c_category_log_locinfo(mycat, &locinfo, LOG4C_PRIORITY_INFO, msg, ##args); \
    }

#define LOG_DEBUG(category, msg, args...) \
    { \
    const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL); \
    log4c_category_log_locinfo(mycat, &locinfo, LOG4C_PRIORITY_DEBUG, msg, ##args); \
    }
 
#define LOG_TRACE(category, msg, args...) \
    { \
    const log4c_location_info_t locinfo = LOG4C_LOCATION_INFO_INITIALIZER(NULL);\
    log4c_category_log_locinfo(mycat, &locinfo, LOG4C_PRIORITY_TRACE, msg, ##args); \
    }

#endif
