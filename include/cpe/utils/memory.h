#ifndef CPE_MEM_ALLOCRATOR_H
#define CPE_MEM_ALLOCRATOR_H
#include "cpe/pal/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mem_allocrator;

typedef void * (*mem_allocrator_alloc_t)(size_t size, struct mem_allocrator * allocrator);
typedef void (*mem_allocrator_free_t)(void * p, struct mem_allocrator * allocrator);

typedef struct mem_allocrator {
    mem_allocrator_alloc_t m_alloc;
    mem_allocrator_free_t m_free;
} * mem_allocrator_t;

void * mem_alloc(struct mem_allocrator * alloc, size_t size);
void mem_free(struct mem_allocrator * alloc, void * p);

struct mem_allocrator * mem_allocrator_null(void);

#ifdef __cplusplus
}
#endif

#endif
