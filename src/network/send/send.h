#ifndef ISOCKCOM_SEND_H
#define ISOCKCOM_SEND_H

#include <stddef.h>
#include "../network.h"

size_t send_in(int socket, const sockaddr_t *dest, const protocol_t *protocol, size_t buf_size);

#endif //ISOCKCOM_SEND_H
