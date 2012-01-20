#include "cpe/pal/pal_string.h"
#include "cpe/utils/stream_error.h"

static void stream_do_flush_to_error_i(
    struct write_stream_error * es,
    const char * buf,
    size_t size)
{
    while(size > 0) {
        size_t copySize = size;
        if (es->m_size + copySize >= 128) {
            copySize = 127 - es->m_size;
        }

        memcpy(es->m_buf + es->m_size, buf, copySize);
        es->m_buf[es->m_size + copySize] = 0;

        if (buf[copySize] == 0 || copySize != size) {
            cpe_error_do_notify(es->m_em, "%s", es->m_buf);
            es->m_size = 0;
        }

        size -= copySize;
        buf += copySize;
    }
}

int stream_do_write_to_error(struct write_stream * stream, const void * input, size_t size) {
    struct write_stream_error * es = (struct write_stream_error *)stream;
    size_t beginPos = 0;
    size_t pos = 0;
    const char * buf = (const char *)input;

    while(pos < size) {
        if(buf[pos] == 0) {
            stream_do_flush_to_error_i(es, buf + beginPos, pos - beginPos);
            beginPos = pos + 1;
        }

        ++pos;
    }

    stream_do_flush_to_error_i(es, buf + beginPos, pos - beginPos);

    return size;
}

int stream_do_flush_to_error(struct write_stream * stream) {
    struct write_stream_error * es = (struct write_stream_error *)stream;
    int r = es->m_size;
    if (es->m_size > 0) {
        es->m_buf[es->m_size] = 0;
        cpe_error_do_notify(es->m_em, "%s", es->m_buf);
        es->m_size = 0;
    }

    return r;
}

void write_stream_error_init(struct write_stream_error * stream, error_monitor_t em, error_level_t level) {
    struct write_stream_error * es = (struct write_stream_error *)stream;

    es->m_em = em;
    es->m_size = 0;
    es->m_em->m_curent_location.m_level = level;
    es->m_em->m_curent_location.m_errno = -1;
}
