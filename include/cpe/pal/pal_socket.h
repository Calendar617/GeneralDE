#ifndef CPE_PAL_SOCKET_H
#define CPE_PAL_SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER

#include <WinSock.h>
#include <errno.h>

#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif

#define cpe_sock_close closesocket
#define cpe_sock_errno() WSAGetLastError()
#define cpe_sock_errstr(n) ""

#else

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#define cpe_sock_close close
#define cpe_sock_errno() errno
#define cpe_sock_errstr(n) strerror(n)

#endif

int cpe_sock_set_none_block(int fd, int is_non_block);

#ifdef __cplusplus
}
#endif

#endif
