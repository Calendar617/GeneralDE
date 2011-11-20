#include <assert.h>
#include "buffer_private.h"

void mem_buffer_begin(struct mem_buffer_pos * pos, struct mem_buffer * buffer) {
    assert(pos);
    assert(buffer);

    pos->m_buffer = buffer;
    pos->m_pos_in_trunk = 0;

    pos->m_trunk = TAILQ_FIRST(&buffer->m_trunks);
    while(pos->m_trunk != TAILQ_END(&buffer->m_trunks) && pos->m_trunk->m_size <= 0) {
        pos->m_trunk = TAILQ_NEXT(pos->m_trunk, m_next);
    }
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

    if (n > 0) {
        while(pos->m_trunk && n > 0) {
            int left = pos->m_trunk->m_size - pos->m_pos_in_trunk - 1;
            if (left >= n) {
                pos->m_pos_in_trunk += n;
                done += n;
                n = 0;
            }
            else {
                done += (left + 1);
                n -= (left + 1);

                do {
                    pos->m_trunk = TAILQ_NEXT(pos->m_trunk, m_next);
                } while(pos->m_trunk && pos->m_trunk->m_size == 0);

                pos->m_pos_in_trunk = 0;
            }
        }
    }
    else if (n < 0) {
        n = -n;
        while(n > 0) {
            int d;

            if (pos->m_trunk == NULL || pos->m_pos_in_trunk == 0) {;
                struct mem_buffer_trunk * pre = 
                    pos->m_trunk == NULL
                    ? TAILQ_LAST(&pos->m_buffer->m_trunks, mem_buffer_trunk_list)
                    : TAILQ_PREV(pos->m_trunk, mem_buffer_trunk_list, m_next);
                
                while(pre && pre != TAILQ_FIRST(&pos->m_buffer->m_trunks) && pre->m_size <= 0) {
                    pre = TAILQ_PREV(pre, mem_buffer_trunk_list, m_next);
                }

                if (pre && pre->m_size > 0) {
                    pos->m_trunk = pre;
                    pos->m_pos_in_trunk = pre->m_size - 1;
                    done -= 1;
                    n -= 1;
                }
                else {
                    break;
                }
            }

            assert(pos->m_trunk);
            assert(pos->m_trunk->m_size > 0);

            d = pos->m_pos_in_trunk;
            if (d == 0) continue;
            if (d >= n) d = n;

            assert(pos->m_pos_in_trunk >= d);
            pos->m_pos_in_trunk -= d;
            done -= d;
            n -= d;
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
