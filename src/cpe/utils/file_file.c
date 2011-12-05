#include <string.h>
#include <stdio.h>
#include "cpe/utils/file.h"
#include "cpe/utils/stream_buffer.h"

FILE * file_open(const char *path, const char *mode, error_monitor_t em);

int file_write_from_buf(const char * file, const void * buf, size_t size, error_monitor_t em) {
    FILE * fp;
    size_t writeSize;
    int rv;

    fp = file_open(file, "w", em);
    if (fp == NULL) return -1;

    while((writeSize = fwrite(buf, 1, size, fp)) > 0) {
        size -= writeSize;
    }

    rv = ferror(fp) == 0 ? 0 : -1;
    fclose(fp);

    return rv;
}

int file_write_from_str(const char * file, const char * str, error_monitor_t em) {
    return file_write_from_buf(file, str, strlen(str), em);
}

int file_write_from_stream(const char * file, read_stream_t stream, error_monitor_t em) {
    FILE * fp;
    size_t writeSize;
    size_t writeOkSize;
    size_t size;
    int rv;
    char buf[128];

    fp = file_open(file, "w", em);
    if (fp == NULL) return -1;

    while((size = stream_read(stream, buf, 128)) > 0) {
        writeOkSize = 0;
        while(size > writeOkSize
              && (writeSize = fwrite(buf + writeOkSize, 1, size - writeOkSize, fp)) > 0)
        {
            writeOkSize += writeSize;
        }

        if (writeOkSize < size) break;
    }

    rv = ferror(fp) == 0 ? 0 : -1;
    fclose(fp);

    return rv;
}

int file_append_from_buf(const char * file, const void * buf, size_t size, error_monitor_t em) {
    FILE * fp;
    size_t writeSize;
    int rv;

    fp = file_open(file, "a", em);
    if (fp == NULL) return -1;

    while((writeSize = fwrite(buf, 1, size, fp)) > 0) {
        size -= writeSize;
    }

    rv = ferror(fp) == 0 ? 0 : -1;
    fclose(fp);

    return rv;
}

int file_append_from_str(const char * file, const char * str, error_monitor_t em) {
    return file_append_from_buf(file, str, strlen(str), em);
}

int file_append_from_stream(const char * file, read_stream_t stream, error_monitor_t em) {
    FILE * fp;
    size_t writeSize;
    size_t writeOkSize;
    size_t size;
    int rv;
    char buf[128];

    fp = file_open(file, "a", em);
    if (fp == NULL) return -1;

    while((size = stream_read(stream, buf, 128)) > 0) {
        writeOkSize = 0;
        while(size > writeOkSize
              && (writeSize = fwrite(buf + writeOkSize, 1, size - writeOkSize, fp)) > 0)
        {
            writeOkSize += writeSize;
        }

        if (writeOkSize < size) break;
    }

    rv = ferror(fp) == 0 ? 0 : -1;
    fclose(fp);

    return rv;
}

int file_load_to_buffer(mem_buffer_t buffer, const char * file, error_monitor_t em) {
    struct write_stream_buffer stream = CPE_WRITE_STREAM_BUFFER_INITIALIZER(buffer);
    return file_load_to_stream((write_stream_t)&stream, file, em);
}

int file_load_to_stream(write_stream_t stream, const char * file, error_monitor_t em) {
    FILE * fp;
    size_t writeSize;
    size_t writeOkSize;
    size_t size;
    int rv;
    char buf[128];

    fp = file_open(file, "r", em);
    if (fp == NULL) return -1;

    while((size = fread(buf, 1, 128, fp)) > 0) {
        writeOkSize = 0;
        while(size > writeOkSize
              && (writeSize = stream_write(stream, buf + writeOkSize, size - writeOkSize)) > 0)
        {
            writeOkSize += writeSize;
        }

        if (writeOkSize < size) break;
    }

    rv = feof(fp) == 1 ? 0 : -1;

    fclose(fp);

    return rv;
}
