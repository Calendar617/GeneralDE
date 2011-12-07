#include <string.h>
#include "cpe/pal/queue.h"
#include "cpe/utils/file.h"
#include "cpe/utils/buffer.h"
#include "file_internal.h"

void dir_search_i(
    dir_visitor_t visitor, void * ctx,
    int maxLevel,
    error_monitor_t em,
    mem_buffer_t buffer)
{
    DIR * dirp;
    struct dirent dbuf;
    struct dirent * dp;
    char * path;
    int rv;

    if (maxLevel == 0)  return;

    path = mem_buffer_make_continuous(buffer, 0);

    dirp = dir_open(path, 0, em);
    if (dirp == NULL) return;

    while((rv = readdir_r(dirp, &dbuf, &dp)) == 0 && dp) {
        int nameLen;

        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;

        nameLen = strlen(dp->d_name);
        path = mem_buffer_make_continuous(buffer, nameLen);
        //assert(mem_buffer_trunk_count(buffer) == 1);

        
        if (S_ISDIR(DTTOIF(dp->d_type))) {
            //rv = dir_rm_recursion(subPath, em, talloc);
        }
        else if (S_ISREG(DTTOIF(dp->d_type))) {
            //rv = file_rm(subPath, em);
        }
    }

    /*clear resources*/
    dir_close(dirp, em);
}

void dir_search(
    dir_visitor_t visitor, void * ctx,
    const char * path, int maxLevel,
    error_monitor_t em, mem_allocrator_t talloc)
{
}

