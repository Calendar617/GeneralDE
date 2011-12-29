#ifndef CPE_UTILS_RANGEMGR_H
#define CPE_UTILS_RANGEMGR_H
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

typedef struct cpe_range_mgr {
    mem_allocrator_t m_alloc;
    size_t m_range_capacity;
    size_t m_range_count;
    struct cpe_range * m_ranges;
} * cpe_range_mgr_t;

int cpe_range_mgr_init(cpe_range_mgr_t ra, mem_allocrator_t alloc);
void cpe_range_mgr_fini(cpe_range_mgr_t ra);
void cpe_range_mgr_clear(cpe_range_mgr_t ra);
void cpe_range_mgr_dump(write_stream_t stream, cpe_range_mgr_t ra);

int32_t cpe_range_get_one(cpe_range_mgr_t ra);
int cpe_range_put_one(cpe_range_mgr_t ra, int32_t value);

struct cpe_range cpe_range_get_range(cpe_range_mgr_t ra, size_t require_count);
int cpe_range_put_range(cpe_range_mgr_t ra, int32_t start, int32_t end);

#ifdef __cplusplus
}
#endif

#endif
