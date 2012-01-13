#ifndef CPE_DR_JSON_H
#define CPE_DR_JSON_H
#include "cpe/utils/buffer.h"
#include "cpe/utils/stream.h"
#include "cpe/utils/error.h"
#include "dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DR_JSON_PRINT_BEAUTIFY 0x00
#define DR_JSON_PRINT_MINIMIZE 0x01 
#define DR_JSON_PRINT_VALIDATE_UTF8 0x10

int dr_json_read(
    struct mem_buffer * result, 
    const char * input,
    LPDRMETA meta,
    error_monitor_t em);

int dr_json_print(
    write_stream_t output,
    const void * input,
    LPDRMETA meta,
    int flag,
    error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
