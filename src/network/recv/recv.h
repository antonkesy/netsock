#ifndef ISOCKCOM_RECV_H
#define ISOCKCOM_RECV_H

#include <stddef.h>
#include "../network.h"

size_t recv_in(int socket, const protocol_t *protocol, size_t buf_size);

#endif //ISOCKCOM_RECV_H
