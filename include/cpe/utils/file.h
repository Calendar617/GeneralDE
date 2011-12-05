#ifndef CPE_UTILS_FILE_H
#define CPE_UTILS_FILE_H
#include "error.h"
#include "memory.h"
#include "stream.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/*file operations*/
int file_write_from_buf(const char * file, const void * buf, size_t size, error_monitor_t em);
int file_write_from_str(const char * file, const char * str, error_monitor_t em);
int file_write_from_stream(const char * file, read_stream_t stream, error_monitor_t em);

int file_append_from_buf(const char * file, const void * buf, size_t size, error_monitor_t em);
int file_append_from_str(const char * file, const char * str, error_monitor_t em);
int file_append_from_stream(const char * file, read_stream_t stream, error_monitor_t em);

int file_load_to_buffer(mem_buffer_t buffer, const char * file, error_monitor_t em);
int file_load_to_stream(write_stream_t stream, const char * file, error_monitor_t em);

int file_rm(const char * file, error_monitor_t em);
int file_exist(const char * file);

/*dir operations*/
int dir_exist(const char * path);
int dir_is_empty(const char * path);
int dir_rm(const char * path, error_monitor_t em);
int dir_mk(const char * path, mode_t mode, error_monitor_t em);
int dir_mk_recursion(const char * path, mode_t mode, error_monitor_t em, mem_allocrator_t talloc);
int dir_rm_recursion(const char * path, error_monitor_t em, mem_allocrator_t talloc);

extern int DIR_DEFAULT_MODE;
extern int FILE_DEFAULT_MODE;

#ifdef __cplusplus
}
#endif

#endif
