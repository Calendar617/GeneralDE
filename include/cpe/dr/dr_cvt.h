#ifndef CPE_DR_CVT_H
#define CPE_DR_CVT_H
#include "dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

dr_cvt_t dr_cvt_create(const char * name);
void dr_cvt_free(dr_cvt_t cvt);

const char * dr_cvt_name(dr_cvt_t cvt);

dr_cvt_result_t dr_cvt_encode(
    dr_cvt_t cvt, 
    LPDRMETA meta,
    void * output, size_t * output_capacity,
    const void * input, size_t * input_capacity,
    error_monitor_t em, int debug);

dr_cvt_result_t dr_cvt_decode(
    dr_cvt_t cvt, 
    LPDRMETA meta,
    void * output, size_t * output_capacity,
    const void * input, size_t * input_capacity,
    error_monitor_t em, int debug);

int dr_cvt_type_create(
    const char * name,
    dr_cvt_fun_t encode,
    dr_cvt_fun_t decode,
    void * ctx);
void dr_cvt_type_free(const char * name);

#ifdef __cplusplus
}
#endif

#endif
