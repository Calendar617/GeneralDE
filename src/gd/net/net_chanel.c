#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "gd/net/net_chanel.h"
#include "net_internal_types.h"

gd_net_chanel_t
gd_net_chanel_mem_create(gd_net_mgr_t nmgr, size_t capacity) {
    gd_net_chanel_t chanel = 
        mem_alloc(
            nmgr->m_alloc,
            sizeof(struct gd_net_chanel) + capacity);
    if (chanel == NULL) return NULL;

    chanel->m_mgr = nmgr;
    chanel->m_buf = (chanel + 1);
    chanel->m_capacity = capacity;
    chanel->m_write_ptr = chanel->m_buf;
    chanel->m_read_ptr = chanel->m_buf;
    chanel->close = NULL;

    TAILQ_INSERT_TAIL(&nmgr->m_chanels, chanel, m_next);

    return chanel;
}

void gd_net_chanel_free(gd_net_chanel_t chanel) {
    assert(chanel);
    TAILQ_REMOVE(&chanel->m_mgr->m_chanels, chanel, m_next);
    if (chanel->close) chanel->close(chanel);
    mem_free(chanel->m_mgr->m_alloc, chanel);
}

size_t gd_net_chanel_capacity(gd_net_chanel_t chanel) {
    return chanel->m_capacity;
}

int gd_net_chanel_wite(gd_net_chanel_t chanel, void * buf, size_t size) {
    size_t size_to_end;
    size_t size_from_begin;

    size_to_end = ((char *)chanel->m_buf + chanel->m_capacity) - (char*)chanel->m_write_ptr;
    if (size_to_end >= size) {
        memcpy(chanel->m_write_ptr, buf, size);
        chanel->m_write_ptr =((char*)chanel->m_write_ptr) +  size;
        return 0;
    }

    size_from_begin = ((char *)chanel->m_read_ptr) - ((char*)chanel->m_buf);
    if ((size_from_begin + size_to_end) >= size) {
        memcpy(chanel->m_write_ptr, buf, size_to_end);
        memcpy(chanel->m_buf, ((char*)buf) + size_to_end, size - size_to_end);
        chanel->m_write_ptr = ((char*)buf) + (size - size_to_end);
        return 0;
    }

    return -1;
}
