#ifndef ISOCKCOM_NETWORK_H
#define ISOCKCOM_NETWORK_H

#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>

#include "../isockcom.h"

typedef union {
    struct sockaddr addr;
    struct sockaddr_in in;
    struct sockaddr_in6 in6;
    struct sockaddr_storage storage;
} sockaddr_t;

int prepare_socket(sockaddr_t *addr, const protocol_t *protocol, bool is_listener);

size_t send_in(int socket, const sockaddr_t *dest, const protocol_t *protocol, size_t buf_size);

size_t recv_in(int socket, const protocol_t *protocol, size_t buf_size);

#endif //ISOCKCOM_NETWORK_H
