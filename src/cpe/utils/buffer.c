#include <assert.h>
#include <string.h>
#include "buffer_private.h"

void mem_buffer_init(struct mem_buffer * buffer, struct mem_allocrator * allocrator) {
    buffer->m_default_allocrator = allocrator;
    buffer->m_size = 0;
    buffer->m_auto_inc_size = 128;
    TAILQ_INIT(&buffer->m_trunks);
}

void mem_buffer_clear(struct mem_buffer * buffer) {
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

ssize_t mem_buffer_read(void * buf, size_t size, struct mem_buffer * buffer) {
    size_t readedSize = 0;
    struct mem_buffer_trunk * trunk;

    if (!buf || size <= 0 || !buffer) return -1;

    trunk = TAILQ_FIRST(&buffer->m_trunks);

    while(readedSize < size && trunk != TAILQ_END(&trunk->m_trunks)) {
        size_t readSize = size - readedSize;
        if (readSize > trunk->m_size) {
            readSize = trunk->m_size;
        }

        memcpy(buf + readedSize, mem_trunk_data(trunk), readSize);
        readedSize += readSize;

        trunk = TAILQ_NEXT(trunk, m_next);
    }

    return (ssize_t)readedSize;
}

ssize_t mem_buffer_append(struct mem_buffer * buffer, const void * buf, size_t size) {
    size_t writedSize = 0;
    size_t newTrunkSize = 0;
    struct mem_buffer_trunk * trunk = NULL;

    if (!buffer || !buf || size < 0) return -1;

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

    return size;
}

void * mem_buffer_make_continuous(struct mem_buffer * buffer) {
    struct mem_buffer_trunk * trunk;

    if (!buffer) return NULL;

    trunk = TAILQ_FIRST(&buffer->m_trunks);

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

    mem_buffer_clear(buffer);

    TAILQ_INSERT_HEAD(&buffer->m_trunks, trunk, m_next);

    buffer->m_size = trunk->m_size;

    return mem_trunk_data(trunk);
}

void * mem_buffer_alloc(struct mem_buffer * buffer, size_t size) {
    void * result = NULL;
    struct mem_buffer_trunk * trunk = NULL;

    if (!buffer || size <= 0) return NULL;

    trunk = TAILQ_LAST(&buffer->m_trunks, mem_buffer_trunk_list);
    if (trunk == NULL || trunk->m_size + size > trunk->m_capacity) {
        trunk = mem_buffer_append_trunk(buffer, size);
    }

    if (trunk == NULL) {
        return NULL;
    }

    result = mem_trunk_data(trunk) + trunk->m_size;

    trunk->m_size += size;
    buffer->m_size += size;

    return result;
}

char * mem_buffer_strdup(struct mem_buffer * buffer, const char * s) {
    size_t n;
    char * p;

    if (!s) return NULL;

    n = strlen(s);
    p = (char*)mem_buffer_alloc(buffer, n + 1);
    if (p == NULL) {
        return NULL;
    }

    memcpy(p, s, n + 1);
    return p;
}

char * mem_buffer_strndup(struct mem_buffer * buffer, const char * s, size_t n) {
    char * p;

    if (!s || n <= 0) return NULL;

    p = (char*)mem_buffer_alloc(buffer, n + 1);
    if (p == NULL) {
        return NULL;
    }

    memcpy(p, s, n);
    p[n] = 0;

    return p;
}

size_t mem_buffer_trunk_count(mem_buffer_t buffer) {
    size_t count;
    struct mem_buffer_trunk * trunk;

    count = 0;
    trunk = TAILQ_FIRST(&buffer->m_trunks);
    while (trunk != TAILQ_END(&buffer->m_trunks)) {
        ++count;
        trunk = TAILQ_NEXT(trunk, m_next);
    }

    return count;
}

struct mem_buffer_trunk *
mem_buffer_trunk_at(mem_buffer_t buffer, size_t pos) {
    struct mem_buffer_trunk * trunk;

    trunk = TAILQ_FIRST(&buffer->m_trunks);
    while (pos > 0 && trunk != TAILQ_END(&buffer->m_trunks)) {
        --pos;
        trunk = TAILQ_NEXT(trunk, m_next);
    }

    return trunk;
}
