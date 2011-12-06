#include <string.h>
#include "cpe/utils/file.h"
#include "cpe/utils/stream_buffer.h"

ssize_t file_write_from_buf(const char * file, const void * buf, size_t size, error_monitor_t em) {
    ssize_t totalSize;
    FILE * fp;
    
    fp = file_stream_open(file, "w", em);
    if (fp == NULL) return -1;

    totalSize = file_stream_write_from_buf(fp, buf, size, em);

    file_stream_close(fp, em);

    return totalSize;
}

ssize_t file_write_from_str(const char * file, const char * str, error_monitor_t em) {
    return file_write_from_buf(file, str, strlen(str), em);
}

ssize_t file_write_from_stream(const char * file, read_stream_t stream, error_monitor_t em) {
    ssize_t totalSize;
    FILE * fp;

    fp = file_stream_open(file, "w", em);
    if (fp == NULL) return -1;

    totalSize = file_stream_write_from_stream(fp, stream, em);

    file_stream_close(fp, em);

    return totalSize;
}

ssize_t file_append_from_buf(const char * file, const void * buf, size_t size, error_monitor_t em) {
    ssize_t totalSize;
    FILE * fp;

    fp = file_stream_open(file, "a", em);
    if (fp == NULL) return -1;

    totalSize = file_stream_write_from_buf(fp, buf, size, em);

    file_stream_close(fp, em);

    return totalSize;
}

ssize_t file_append_from_str(const char * file, const char * str, error_monitor_t em) {
    return file_append_from_buf(file, str, strlen(str), em);
}

ssize_t file_append_from_stream(const char * file, read_stream_t stream, error_monitor_t em) {
    FILE * fp;
    ssize_t totalSize;

    fp = file_stream_open(file, "a", em);
    if (fp == NULL) return -1;

    totalSize = file_stream_write_from_stream(fp, stream, em);

    file_stream_close(fp, em);

    return totalSize;
}

ssize_t file_load_to_buffer(mem_buffer_t buffer, const char * file, error_monitor_t em) {
    struct write_stream_buffer stream = CPE_WRITE_STREAM_BUFFER_INITIALIZER(buffer);
    return file_load_to_stream((write_stream_t)&stream, file, em);
}

ssize_t file_load_to_stream(write_stream_t stream, const char * file, error_monitor_t em) {
    FILE * fp;
    ssize_t totalSize;

    fp = file_stream_open(file, "r", em);
    if (fp == NULL) return -1;

    totalSize = file_stream_load_to_stream(stream, fp, em);

    if (!feof(fp)) {
        totalSize = -1;
    }

    file_stream_close(fp, em);
    return totalSize;
}

ssize_t file_stream_write_from_buf(FILE * fp, const void * buf, size_t size, error_monitor_t em) {
    ssize_t totalSize;
    size_t writeSize;

    totalSize = 0;
    while((writeSize = fwrite(buf, 1, size, fp)) > 0) {
        size -= writeSize;
        totalSize += writeSize;
    }

    if (ferror(fp)) {
        totalSize = -1;
    }

    return totalSize;
}

ssize_t file_stream_write_from_str(FILE * fp, const char * str, error_monitor_t em) {
    return file_stream_write_from_buf(fp, str, strlen(str), em);
}

ssize_t file_stream_write_from_stream(FILE * fp, read_stream_t stream, error_monitor_t em) {
    ssize_t totalSize;
    size_t writeSize;
    size_t writeOkSize;
    size_t size;
    char buf[128];

    totalSize = 0;
    while((size = stream_read(stream, buf, 128)) > 0) {
        writeOkSize = 0;
        while(size > writeOkSize
              && (writeSize = fwrite(buf + writeOkSize, 1, size - writeOkSize, fp)) > 0)
        {
            writeOkSize += writeSize;
        }

        totalSize += writeOkSize;

        if (writeOkSize < size) break;
    }

    if (ferror(fp)) {
        totalSize = -1;
    }

    return totalSize;
}

ssize_t file_stream_load_to_buffer(mem_buffer_t buffer, FILE * fp, error_monitor_t em) {
    struct write_stream_buffer stream = CPE_WRITE_STREAM_BUFFER_INITIALIZER(buffer);
    return file_stream_load_to_stream((write_stream_t)&stream, fp, em);
}

ssize_t file_stream_load_to_stream(write_stream_t stream, FILE * fp, error_monitor_t em) {
    size_t writeSize;
    size_t writeOkSize;
    size_t size;
    ssize_t totalSize;
    char buf[128];

    totalSize = 0;
    while((size = fread(buf, 1, 128, fp)) > 0) {
        writeOkSize = 0;
        while(size > writeOkSize
              && (writeSize = stream_write(stream, buf + writeOkSize, size - writeOkSize)) > 0)
        {
            writeOkSize += writeSize;
        }

        totalSize += writeOkSize;
        if (writeOkSize < size) break;
    }

    if (ferror(fp)) {
        totalSize = -1;
    }

    return totalSize;
}
