#include <assert.h>
#ifdef _MSC_VER
#include <WinSock.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <fcntl.h>
#include <errno.h>
#include "cpe/pal/pal_unistd.h"
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_unistd.h"
#include "net_internal_ops.h"

int gd_net_socket_set_none_block(int fd, error_monitor_t em) {
#if _MSC_VER
    u_long flag;

    flag = 1;
    if (ioctlsocket(fd, FIONBIO, &flag) != 0) {
        CPE_ERROR(em, "gd_net_tcp_set_none_block: ioctlsocket fail! errno=%d", WSAGetLastError());
        return -1;
    }
    else {
        return 0;
    }
#else
    int flags;
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        CPE_ERROR(em, "gd_net_tcp_set_none_block: get fd flag fail! errno=%d (%s)", errno, strerror(errno));
        return -1;
    }

    if (flags & O_NONBLOCK) {
        CPE_INFO(em, "gd_net_tcp_set_none_block: is already non block");
        return 0;
    }
    else {
        flags |= O_NONBLOCK;
        if (fcntl(fd, F_SETFL, flags) == -1) {
            CPE_ERROR(em, "gd_net_tcp_set_none_block: set fd flag fail! errno=%d (%s)", errno, strerror(errno));
            return -1;
        }
        else {
            return 0;
        }
    }
#endif
}

void gd_net_socket_close(int * fd, error_monitor_t em) {
    if (*fd == -1) return;

#ifdef _MSC_VER
    if (closesocket(*fd) != 0) {
        CPE_ERROR(em, "gd_net_socket_destory: close fail, errno=%d (%s)", errno, strerror(errno));
    }
#else
    if (close(*fd) != 0) {
        CPE_ERROR(em, "gd_net_socket_destory: close fail, errno=%d (%s)", errno, strerror(errno));
    }
#endif
    *fd = -1;
}
