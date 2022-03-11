#ifndef NETCOPY_NETWORK_H
#define NETCOPY_NETWORK_H

#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>

#include "../netcopy.h"

size_t send_file(int file_fd, const struct sockaddr *dest, const protocol_t *protocol, size_t buf_size);

#endif //NETCOPY_NETWORK_H
