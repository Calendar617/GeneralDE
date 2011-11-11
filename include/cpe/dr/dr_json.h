#ifndef CPE_DR_JSON_H
#define CPE_DR_JSON_H
#include "cpe/utils/buffer.h"
#include "cpe/utils/error.h"
#include "dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int dr_json_read(
    struct mem_buffer * result, 
    const char * input,
    LPDRMETA meta,
    error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
