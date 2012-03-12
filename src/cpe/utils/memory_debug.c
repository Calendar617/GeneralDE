#include <execinfo.h>
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/memory_debug.h"
#include "cpe/utils/hash.h"

struct debug_mem_alloc_info {
    void * m_addr;
    void * m_stack[5];
    
    TAILQ_ENTRY(debug_mem_alloc_info) m_next;

    struct cpe_hash_entry m_hh;
};

struct debug_mem_allocrator {
    struct mem_allocrator m_alloc;
    mem_allocrator_t m_parent;
    error_monitor_t m_em;

    struct cpe_hash_table m_alloc_infos;
};

static uint32_t debug_mem_alloc_info_hash(const struct debug_mem_alloc_info * context) {
    return (uint32_t)context->m_addr;
}

int debug_mem_alloc_info_cmp(const struct debug_mem_alloc_info * l, const struct debug_mem_alloc_info * r) {
    return l->m_addr == r->m_addr;
}

static void * do_debug_allocrator_alloc(size_t size, struct mem_allocrator * allocrator) {
    struct debug_mem_allocrator * dalloc = (struct debug_mem_allocrator *)allocrator;

    void * r = mem_alloc(dalloc->m_parent, size);

    return r;
}

static void do_debug_allocrator_free(void * p, struct mem_allocrator * allocrator) {
    struct debug_mem_allocrator * dalloc = (struct debug_mem_allocrator *)allocrator;

    mem_free(dalloc->m_parent, p);
}

mem_allocrator_t mem_allocrator_debug_create(mem_allocrator_t parent, error_monitor_t em) {
    struct debug_mem_allocrator * alloc =
        (struct debug_mem_allocrator *)mem_alloc(parent, sizeof(struct debug_mem_allocrator));
    if (alloc == NULL) return NULL;

    alloc->m_alloc.m_alloc = do_debug_allocrator_alloc;
    alloc->m_alloc.m_free = do_debug_allocrator_free;
    alloc->m_parent = parent;
    alloc->m_em = em;

    return (mem_allocrator_t)alloc;
}

void mem_allocrator_debug_free(mem_allocrator_t dalloc) {
}

void mem_allocrator_debug_dump(write_stream_t stream, mem_allocrator_t dalloc) {
}

int mem_allocrator_debug_alloc_count(mem_allocrator_t dalloc) {
    return 0;
}

int mem_allocrator_debug_free_count(mem_allocrator_t dalloc) {
    return 0;
}

int mem_allocrator_debug_alloc_size(mem_allocrator_t dalloc) {
    return 0;
}

int mem_allocrator_debug_free_size(mem_allocrator_t dalloc) {
    return 0;
}
