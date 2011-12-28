#ifndef CPE_UTILS_RANGEALLOCRATOR_H
#define CPE_UTILS_RANGEALLOCRATOR_H
#include "cpe/pal/types.h"
#include "memory.h"
#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

struct cpe_range {
    int32_t m_start;
    int32_t m_end;
};

typedef struct cpe_range_allocrator {
    mem_allocrator_t m_alloc;
    size_t m_range_capacity;
    size_t m_range_count;
    struct cpe_range * m_ranges;
} * cpe_range_allocrator_t;

int cpe_range_allocrator_init(cpe_range_allocrator_t ra, mem_allocrator_t alloc);
void cpe_range_allocrator_fini(cpe_range_allocrator_t ra);
void cpe_range_allocrator_dump(write_stream_t stream, cpe_range_allocrator_t ra);

int32_t cpe_range_alloc_one(cpe_range_allocrator_t ra);
int cpe_range_free_one(cpe_range_allocrator_t ra, int32_t value);

struct cpe_range cpe_range_alloc_range(cpe_range_allocrator_t ra, size_t require_count);
int cpe_range_free_range(cpe_range_allocrator_t ra, int32_t start, int32_t end);

#ifdef __cplusplus
}
#endif

#endif
