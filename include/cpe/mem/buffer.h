#ifndef CPE_MEM_BUFFER_H
#define CPE_MEM_BUFFER_H
#include "cpe/pal/queue.h"
#include "memory.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mem_buffer;
struct mem_buffer_trunk;

struct mem_buffer {
    struct mem_allocrator * m_default_allocrator;
    size_t m_size;
    size_t m_auto_inc_size;
    TAILQ_HEAD(mem_buffer_trunk_list, mem_buffer_trunk) m_trunks;
};

struct mem_buffer_pos {
    struct mem_buffer * m_buffer;
    struct mem_buffer_trunk * m_trunk;
    size_t m_pos_in_trunk;
};

/* trunk operations */
struct mem_buffer_trunk *
mem_buffer_append_trunk(struct mem_buffer * buffer, size_t capacity);
struct mem_buffer_trunk *
mem_buffer_append_trunk_after(struct mem_buffer * buffer, struct mem_buffer_trunk * preTrunk, size_t capacity);
void mem_trunk_free(struct mem_buffer * buffer, struct mem_buffer_trunk * trunk);

void * mem_trunk_data(struct mem_buffer_trunk * trunk);
size_t mem_trunk_capacity(struct mem_buffer_trunk * trunk);
size_t mem_trunk_size(struct mem_buffer_trunk * trunk);
size_t mem_trunk_append(struct mem_buffer * buffer, struct mem_buffer_trunk * trunk, const void * buf, size_t size);
void mem_trunk_set_size(struct mem_buffer * buffer, struct mem_buffer_trunk * trunk, size_t size);

/* buffer basic operations */
size_t mem_buffer_size(struct mem_buffer * buffer);

void mem_buffer_init(struct mem_buffer * buffer, struct mem_allocrator * allocrator);
void mem_buffer_free(struct mem_buffer * buffer);

/* buffer position operations */
void mem_buffer_head(struct mem_buffer_pos * pos, struct mem_buffer * buffer);
void mem_buffer_seek(struct mem_buffer_pos * pos, size_t n);
void mem_buffer_at(struct mem_buffer_pos * pos, struct mem_buffer * buffer, size_t n);

/* buffer read write operations */
int mem_buffer_append(struct mem_buffer * buffer, const void * buf, size_t size);
size_t mem_buffer_read(void * buf, size_t size, struct mem_buffer * buffer);
void * mem_buffer_make_continuous(struct mem_buffer * buffer);

#ifdef __cplusplus
}
#endif

#endif
