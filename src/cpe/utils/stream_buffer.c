#include "cpe/utils/stream_buffer.h"

int stream_do_write_to_buf(struct write_stream * input_stream, const void * buf, size_t size) {
    struct write_stream_buffer * stream = (struct write_stream_buffer *)input_stream;

    return mem_buffer_append(stream->m_buffer, buf, size);
}

void write_stream_buffer_init(struct write_stream_buffer * stream, struct mem_buffer * buffer) {
    stream->m_stream.write = stream_do_write_to_buf;
    stream->m_stream.flush = stream_do_flush_dummy;
    stream->m_buffer = buffer;
}

