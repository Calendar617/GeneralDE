#include <assert.h>
#include "buffer_private.h"

void mem_buffer_begin(struct mem_buffer_pos * pos, struct mem_buffer * buffer) {
    assert(pos);
    assert(buffer);

    pos->m_buffer = buffer;
    pos->m_trunk = TAILQ_FIRST(&buffer->m_trunks);
    pos->m_pos_in_trunk = 0;
}

void mem_buffer_end(struct mem_buffer_pos * pos, struct mem_buffer * buffer) {
    assert(pos);
    assert(buffer);

    pos->m_buffer = buffer;
    pos->m_trunk = TAILQ_END(&buffer->m_trunks);
    pos->m_pos_in_trunk = 0;
}

ssize_t mem_pos_seek(struct mem_buffer_pos * pos, ssize_t n) {
    ssize_t done = 0;

    assert(pos);
    assert(pos->m_buffer);

    if (pos->m_trunk == NULL) {
        return done;
    }

    if (n > 0) {
        while(pos->m_trunk && n > 0) {
            int left = pos->m_trunk->m_size - pos->m_pos_in_trunk - 1;
            if (left >= n) {
                pos->m_pos_in_trunk += n;
                done += n;
                n = 0;
            }
            else {
                pos->m_trunk = TAILQ_NEXT(pos->m_trunk, m_next);
                pos->m_pos_in_trunk = 0;
                done += left;
                n -= left;
            }
        }
    }
    else if (n < 0) {
        while(pos->m_trunk && n < 0) {
            int left = pos->m_pos_in_trunk;
            if (left >= n) {
                pos->m_pos_in_trunk -= n;
                done -= n;
                n = 0;
            }
            else {
                pos->m_trunk = TAILQ_PREV(pos->m_trunk, mem_buffer_trunk_list, m_next);
                pos->m_pos_in_trunk = pos->m_trunk->m_size;
                done -= left;
                n += left;
            }
        }
        
    }

    return done;
}

void mem_pos_at(struct mem_buffer_pos * pos, struct mem_buffer * buffer, size_t n) {
    assert(pos);
    assert(buffer);

    mem_buffer_begin(pos, buffer);
    mem_pos_seek(pos, n);
}

int mem_pos_eq(mem_buffer_pos_t l, mem_buffer_pos_t r) {
    assert(l);
    assert(r);

    return l->m_buffer == r->m_buffer
        && l->m_trunk == r->m_trunk
        && l->m_pos_in_trunk == r->m_pos_in_trunk
        ? 1
        : 0;
}

ssize_t mem_pos_diff(mem_buffer_pos_t l, mem_buffer_pos_t r) {
    ssize_t from_l = 0;

    assert(l);
    assert(r);
    assert(l->m_buffer == r->m_buffer);

    while(l->m_trunk && l->m_trunk != r->m_trunk) {
        from_l += (l->m_trunk->m_size - l->m_pos_in_trunk);

        l->m_trunk = TAILQ_NEXT(l->m_trunk, m_next);
        l->m_pos_in_trunk = 0;
    }

    if (l->m_trunk) {
        assert(l->m_trunk == r->m_trunk);
        return from_l + r->m_pos_in_trunk;
    }
    else {
        if (!r->m_trunk) {
            return 0;
        }
        else {
            ssize_t head_to_r = 0;
            struct mem_buffer_trunk * trunk = TAILQ_FIRST(&r->m_buffer->m_trunks);

            while(trunk && trunk != r->m_trunk) {
                head_to_r += trunk->m_size;
                trunk = TAILQ_NEXT(trunk, m_next);
            }

            assert(trunk == r->m_trunk);
            if (trunk == r->m_trunk) {
                head_to_r += r->m_pos_in_trunk;
            }

            return - (r->m_buffer->m_size - from_l - head_to_r);
        }
    }
}

int mem_pos_valide(mem_buffer_pos_t l) {
    return l
        && l->m_buffer
        && l->m_trunk
        && l->m_pos_in_trunk < l->m_trunk->m_size
        ? 1
        : 0;
}

char mem_pos_data(mem_buffer_pos_t l) {
    assert(mem_pos_valide(l));
    return ((char *)mem_trunk_data(l->m_trunk))[l->m_pos_in_trunk];
}
