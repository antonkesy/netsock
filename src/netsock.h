#ifndef NETSOCK_H
#define NETSOCK_H

#define DEFAULT_BUF_SIZE 1024

#include <stdbool.h>

enum Protocol {
    TCP = 0, UDP = 1
};

typedef enum Protocol protocol_t;

int get_buffer_size(int socket_fd, bool is_listener);

#endif //NETSOCK_H
