#ifndef CPE_UTILS_STREAM_MEM_H
#define CPE_UTILS_STREAM_MEM_H
#include "stream.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

struct write_stream_mem {
    struct write_stream m_stream;
    void * m_buffer;
    size_t m_capacity;
    size_t m_pos;
};

int stream_do_write_to_mem(struct write_stream * stream, const void * buf, size_t size);

void write_stream_mem_init(struct write_stream_mem * stream, void * buf, size_t size);

#define CPE_STREAM_MEM_INITIALIZER(__buf, __len)                        \
    { CPE_STREAM_INITIALIZER(stream_do_write_to_mem, stream_do_flush_dummy), __buf, __len, 0 }

#ifdef __cplusplus
}
#endif

#endif
