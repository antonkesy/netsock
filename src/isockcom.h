#ifndef ISOCKCOM_H
#define ISOCKCOM_H

#define DEFAULT_BUF_SIZE 1024

#include <stdbool.h>

enum Protocol {
    UDP, TCP
};

typedef enum Protocol protocol_t;

int get_buffer_size(int socket_fd, bool is_listener);

#endif //ISOCKCOM_H
