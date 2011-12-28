#ifndef CPE_UTILS_RANGEBITARRAY_H
#define CPE_UTILS_RANGEBITARRAY_H
#include "range_allocrator.h"
#include "bitarry.h"

#ifdef __cplusplus
extern "C" {
#endif

int cpe_range_free_from_bitarray(
    cpe_range_allocrator_t ra,
    cpe_ba_t ba,
    int32_t baStartPos,
    size_t ba_capacity);

#ifdef __cplusplus
}
#endif

#endif
