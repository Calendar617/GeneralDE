#include "cpe/pal/pal_socket.h"

int cpe_sock_set_block(int fd, int is_non_block) {
#if _MSC_VER
    u_long flag;

    flag = is_non_block ? 1 : 0;
    return ioctlsocket(fd, FIONBIO, &flag);
#else
    int flags;
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        return -1;
    }

    if (is_non_block) {
        if (flags & O_NONBLOCK) {
            return 0;
        }
        else {
            flags |= O_NONBLOCK;
        }
    }
    else {
        if (!(flags & O_NONBLOCK)) {
            return 0;
        }
        else {
            flags &= ~O_NONBLOCK;
        }
    }

    return fcntl(fd, F_SETFL, flags);
#endif
}


