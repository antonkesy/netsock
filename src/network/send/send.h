#ifndef NETSOCK_SEND_H
#define NETSOCK_SEND_H

#include <stddef.h>

#include "../network.h"

size_t send_in(int socket, const sockaddr_t *dest, const protocol_t *protocol,
               size_t buf_size);

#endif  // NETSOCK_SEND_H
