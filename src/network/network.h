#ifndef NETSOCK_NETWORK_H
#define NETSOCK_NETWORK_H

#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>

#include "../netsock.h"

typedef union {
    struct sockaddr addr;
    struct sockaddr_in in;
    struct sockaddr_in6 in6;
    struct sockaddr_storage storage;
} sockaddr_t;

int prepare_socket(sockaddr_t *addr, const protocol_t *protocol, bool is_listener);

#endif //NETSOCK_NETWORK_H
