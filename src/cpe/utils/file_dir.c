#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "cpe/utils/file.h"
#include "cpe/utils/buffer.h"

DIR * dir_open(const char * path, error_monitor_t em);
void dir_close(DIR * dirp, error_monitor_t em);

int dir_mk_recursion(const char * path, mode_t mode, error_monitor_t em, mem_allocrator_t talloc) {
    size_t path_len = strlen(path) + 1;
    char * path_buf = mem_alloc(talloc, path_len);
    char * nextSepPos;
    int rv = 0;

    memcpy(path_buf, path, path_len);

    for(nextSepPos = strchr(path_buf, '/');
        nextSepPos && rv == 0;
        nextSepPos = strchr(nextSepPos + 1, '/'))
    {
        *nextSepPos = 0;

        rv = dir_mk(path_buf, mode, em);
        if (rv && errno == EEXIST) rv = 0;

        *nextSepPos = '/';
    }

    if (rv == 0) {
        rv = dir_mk(path_buf, mode, em);
        if (rv && errno == EEXIST) rv = 0;
    }

    mem_free(talloc, path_buf);
    return rv;
}

int dir_rm_recursion(const char * path, error_monitor_t em, mem_allocrator_t talloc) {
    DIR * dirp;
    struct dirent dbuf;
    struct dirent * dp;
    int rv = 0;
    int haveError;

    char * subPath = NULL;
    size_t pathSize;

    dirp = dir_open(path, em);
    if (dirp == NULL) {
        return errno == ENOENT ? 0 : -1;
    }

    pathSize = strlen(path);
    haveError = 0;

    /*remove sub entities*/
    while((rv = readdir_r(dirp, &dbuf, &dp)) == 0 && dp) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;

        if (subPath == NULL) {
            subPath = mem_alloc(talloc, pathSize + 1/*path sep*/ + MAXNAMLEN + 1);
            if (subPath == NULL) {
                CPE_ERROR_EX(em, ENOMEM, "alloc for path buf fail!");
                rv = -1;
                break;
            }

            memcpy(subPath, path, pathSize);
            subPath[pathSize] = '/';
        }
        strncpy(subPath + pathSize + 1, dp->d_name, MAXNAMLEN);

        if (S_ISDIR(DTTOIF(dp->d_type))) {
            rv = dir_rm_recursion(subPath, em, talloc);
        }
        else {
            rv = file_rm(subPath, em);
        }

        if (rv != 0) break;
    }

    /*remove curent node*/
    if (rv == 0) {
        rv = dir_rm(path, em);
    }

    /*clear resource*/
    if (subPath) mem_free(talloc, subPath);
    dir_close(dirp, em);
    return rv;
}

int dir_is_empty(const char * path) {
    DIR * dirp;
    struct dirent dbuf;
    struct dirent * dp;
    int rv;

    dirp = dir_open(path, NULL);
    if (dirp == NULL) { return 1; }

    rv = readdir_r(dirp, &dbuf, &dp);

    dir_close(dirp, NULL);

    return rv == 0 ? 0 : 1;
}
