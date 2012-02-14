#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_unistd.h"
#include "net_internal_ops.h"

int gd_net_socket_set_none_block(int fd, error_monitor_t em) {
    int flags;
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        CPE_ERROR(em, "gd_net_tcp_set_none_block: get fd flag fail! errno=%s", strerror(errno));
        return -1;
    }

    if (flags & O_NONBLOCK) {
        CPE_INFO(em, "gd_net_tcp_set_none_block: is already non block");
        return 0;
    }
    else {
        flags |= O_NONBLOCK;
        if (fcntl(fd, F_SETFL, flags) == -1) {
            CPE_ERROR(em, "gd_net_tcp_set_none_block: set fd flag fail! errno=%s", strerror(errno));
            return -1;
        }
        else {
            CPE_INFO(em, "gd_net_tcp_set_none_block: set fd to non block success!");
            return 0;
        }
    }
}

void gd_net_socket_close(int * fd, error_monitor_t em) {
    if (*fd == -1) return;

    if (close(*fd) != 0) {
        CPE_ERROR(em, "gd_net_socket_destory: close fail, errno=%s", strerror(errno));
    }
    *fd = -1;
}
