#include <assert.h>
#include <string.h>
#include "buffer_private.h"

void mem_buffer_init(struct mem_buffer * buffer, struct mem_allocrator * allocrator) {
    buffer->m_default_allocrator = allocrator;
    buffer->m_size = 0;
    buffer->m_auto_inc_size = 128;
    TAILQ_INIT(&buffer->m_trunks);
}

void mem_buffer_free(struct mem_buffer * buffer) {
    while(!TAILQ_EMPTY(&buffer->m_trunks)) {
        mem_trunk_free(buffer, TAILQ_FIRST(&buffer->m_trunks));
    }

    assert(buffer->m_size == 0);
}

size_t mem_buffer_size(struct mem_buffer * buffer) {
    return buffer->m_size;
}

struct mem_buffer_trunk *
mem_buffer_append_trunk(struct mem_buffer * buffer, size_t capacity) {
    struct mem_buffer_trunk * trunk = mem_trunk_alloc(buffer->m_default_allocrator, capacity);

    if (trunk) {
        TAILQ_INSERT_TAIL(&buffer->m_trunks, trunk, m_next);
    }

    return trunk;
}

struct mem_buffer_trunk *
mem_buffer_append_trunk_after(
    struct mem_buffer * buffer,
    struct mem_buffer_trunk * preTrunk,
    size_t capacity)
{
    struct mem_buffer_trunk * trunk = mem_trunk_alloc(buffer->m_default_allocrator, capacity);

    if (trunk) {
        TAILQ_INSERT_AFTER(&buffer->m_trunks, preTrunk, trunk, m_next);
    }

    return trunk;
}

size_t mem_buffer_read(void * buf, size_t size, struct mem_buffer * buffer) {
    size_t readedSize = 0;

    struct mem_buffer_trunk * trunk = TAILQ_FIRST(&buffer->m_trunks);

    while(readedSize < size && trunk != TAILQ_END(&trunk->m_trunks)) {
        size_t readSize = size - readedSize;
        if (readSize > trunk->m_size) {
            readSize = trunk->m_size;
        }

        memcpy(buf + readedSize, mem_trunk_data(trunk), readSize);
        readedSize += readSize;

        trunk = TAILQ_NEXT(trunk, m_next);
    }

    return readedSize;
}

int mem_buffer_append(struct mem_buffer * buffer, const void * buf, size_t size) {
    size_t writedSize = 0;
    size_t newTrunkSize = 0;

    struct mem_buffer_trunk * trunk = NULL;

    trunk = TAILQ_LAST(&buffer->m_trunks, mem_buffer_trunk_list);

    if (trunk != TAILQ_END(&buffer->m_trunks)) {
        writedSize += mem_trunk_append(buffer, trunk, buf, size);
    }

    newTrunkSize = size - writedSize;
    if (newTrunkSize < buffer->m_auto_inc_size) {
        newTrunkSize = buffer->m_auto_inc_size;
    }

    trunk = mem_buffer_append_trunk(buffer, newTrunkSize);
    if (trunk == NULL) {
            return -1;
    }

    writedSize += mem_trunk_append(buffer, trunk, buf + writedSize, size - writedSize);

    return 0;
}

void * mem_buffer_make_continuous(struct mem_buffer * buffer) {
    struct mem_buffer_trunk * trunk = TAILQ_FIRST(&buffer->m_trunks);

    if (trunk == TAILQ_END(&buffer->m_trunks)) {
        return NULL;
    }

    if (TAILQ_NEXT(trunk, m_next) == TAILQ_END(&buffer->m_trunks)) {
        return mem_trunk_data(trunk);
    }

    trunk = mem_trunk_alloc(buffer->m_default_allocrator, buffer->m_size);
    if (trunk == NULL) {
        return NULL;
    }

    mem_buffer_read(mem_trunk_data(trunk), buffer->m_size, buffer);

    mem_buffer_free(buffer);

    TAILQ_INSERT_HEAD(&buffer->m_trunks, trunk, m_next);

    buffer->m_size = trunk->m_size;

    return mem_trunk_data(trunk);
}
