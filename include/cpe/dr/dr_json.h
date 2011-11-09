#ifndef CPE_DR_JSON_H
#define CPE_DR_JSON_H
#include "cpe/mem/buffer.h"
#include "dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int dr_json_read(
    struct mem_buffer * result, 
    const char * input,
    LPDRMETA meta);

#ifdef __cplusplus
}
#endif

#endif
