#ifndef ISOCKCOM_RECV_H
#define ISOCKCOM_RECV_H

#include <stddef.h>
#include "../network.h"

typedef ssize_t (*recv_fun)(int fd, void *buffer, size_t n);

ssize_t udp_recv(int fd, void *buffer, size_t n);

ssize_t tcp_recv(int fd, void *buffer, size_t n);

size_t recv_stdin(int socket, recv_fun recv, size_t buf_size);

#endif //ISOCKCOM_RECV_H
