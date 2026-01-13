#ifndef NETSOCK_RECV_H
#define NETSOCK_RECV_H

#include <stddef.h>

#include "protocol.h"

size_t recv_in(int socket, const protocol_t* protocol, size_t buf_size);

#endif  // NETSOCK_RECV_H
