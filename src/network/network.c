#include <unistd.h>
#include <stdio.h>
#include "network.h"

ssize_t udp_send(int fd, const void *buffer, size_t n, const struct sockaddr *dest);

ssize_t tcp_send(int fd, const void *buffer, size_t n, const struct sockaddr *dest);

size_t
send_file(int file_fd, const struct sockaddr *dest, const in_port_t* self_port, const protocol_t *protocol, size_t buf_size) {
    int type;
    ssize_t (*send)(int fd, const void *buffer, size_t n, const struct sockaddr *dest);

    switch (*protocol) {
        case UDP:
            send = &udp_send;
            type = SOCK_DGRAM;
            break;
        case TCP:
            send = &tcp_send;
            type = SOCK_STREAM;
            break;
    }

    int socket_fd = socket(dest->sa_family, type, 0);
    if (socket_fd < 0) {
        perror("could not open socket");
    }

    //set port if changed
    if (self_port != 0) {
        struct sockaddr_in self;
        self.sin_family = AF_INET;
        self.sin_addr.s_addr = INADDR_ANY;
        self.sin_port = *self_port;

        int bind_ret = bind(socket_fd, (const struct sockaddr *) &self, sizeof(struct sockaddr));
        if (bind_ret == -1)
            return 0;
    }

    if (*protocol == TCP) {
        if (connect(socket_fd, dest, sizeof(struct sockaddr)) == -1) {
            perror("tcp connection error");
            return 0;
        }
    }

    uint8_t *buffer[buf_size];

    size_t sum_sent = 0U;
    ssize_t bytes_read;
    ssize_t bytes_sent;
    do {
        bytes_read = read(file_fd, buffer, buf_size);
        if (bytes_read < 0) {
            perror("read error");
            return sum_sent;
        }
        bytes_sent = send(socket_fd, buffer, bytes_read, dest);
        if ((ssize_t) bytes_sent != bytes_read) {
            perror("send error");
            return sum_sent;
        }
        sum_sent += bytes_sent;
    } while (bytes_read > 0);

    close(socket_fd);
    return sum_sent;
}

ssize_t udp_send(int fd, const void *buffer, size_t n, const struct sockaddr *dest) {
    return sendto(fd, buffer, n, 0, dest, sizeof(struct sockaddr));
}

ssize_t tcp_send(int fd, const void *buffer, size_t n, const struct sockaddr *dest) {
    return send(fd, buffer, n, 0);
}
