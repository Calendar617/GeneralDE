#ifndef CPE_UTILS_STREAM_H
#define CPE_UTILS_STREAM_H
#include "cpe/pal/pal_stdarg.h"
#include "cpe/pal/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct write_stream {
    int (*write)(struct write_stream * stream, const void * buf, size_t size);
    int (*flush)(struct write_stream * stream);
} * write_stream_t;

typedef struct read_stream {
    int (*read)(struct read_stream * stream, void * buf, size_t len);
} * read_stream_t;

int stream_write(struct write_stream * stream, const void * buf, size_t size);
#define stream_flush(stream) (stream)->flush(stream)

int stream_printf(struct write_stream * stream, const char * fmt, ...);
int stream_vprintf(struct write_stream * stream, const char * fmt, va_list ap);

int stream_do_flush_dummy(struct write_stream * stream);

int stream_putc_count(struct write_stream * stream, char c, size_t n);
#define stream_putc(stream, c) do{ char __b = c; (stream)->write((stream), &__b, 1); } while(0)
#define stream_read(stream, buf, size) (stream)->read((stream), buf, size)

#define CPE_WRITE_STREAM_INITIALIZER(_write, _flush) { (_write), (_flush) }
#define CPE_READ_STREAM_INITIALIZER(_read) { (_read) }

#ifdef __cplusplus
}
#endif

#endif
