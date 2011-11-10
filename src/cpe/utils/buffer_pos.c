#include "buffer_private.h"

void mem_buffer_head(struct mem_buffer_pos * pos, struct mem_buffer * buffer) {
    pos->m_buffer = 0;
    pos->m_trunk = TAILQ_FIRST(&buffer->m_trunks);
    pos->m_pos_in_trunk = 0;
}

void mem_buffer_seek(struct mem_buffer_pos * pos, size_t n) {
    if (pos->m_trunk == NULL) {
        return;
    }

    //while(pos->m_trunk && 
}

void mem_buffer_at(struct mem_buffer_pos * pos, struct mem_buffer * buffer, size_t n) {
    if (pos->m_trunk == NULL) {
        return;
    }
}

