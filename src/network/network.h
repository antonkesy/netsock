#ifndef NETCOPY_NETWORK_H
#define NETCOPY_NETWORK_H

#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>

#include "../netcopy.h"

typedef union {
    struct sockaddr addr;
    struct sockaddr_in in;
    struct sockaddr_in6 in6;
    struct sockaddr_storage storage;
} sockaddr_t;

size_t send_stdin(const sockaddr_t *dest, const in_port_t *self_port, const protocol_t *protocol, size_t buf_size);

#endif //NETCOPY_NETWORK_H
