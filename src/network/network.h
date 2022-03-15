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

#endif //ISOCKCOM_NETWORK_H
