#ifndef ISOCKCOM_SEND_H
#define ISOCKCOM_SEND_H

#include <stddef.h>
#include "../network.h"

typedef ssize_t (*send_fun)(int fd, const void *buffer, size_t n, const sockaddr_t *dest);

ssize_t udp_send(int fd, const void *buffer, size_t n, const sockaddr_t *dest);

ssize_t tcp_send(int fd, const void *buffer, size_t n, const sockaddr_t *dest);

#endif //ISOCKCOM_SEND_H
