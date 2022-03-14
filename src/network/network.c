#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include "network.h"

typedef ssize_t (*send_fun)(int fd, const void *buffer, size_t n, const sockaddr_t *dest);

ssize_t udp_send(int fd, const void *buffer, size_t n, const sockaddr_t *dest);

ssize_t tcp_send(int fd, const void *buffer, size_t n, const sockaddr_t *dest);

int prepare_socket(const sockaddr_t *dest, const in_port_t *self_port, const protocol_t *protocol) {
    int type;

    switch (*protocol) {
        case UDP:
            type = SOCK_DGRAM;
            break;
        case TCP:
            type = SOCK_STREAM;
            break;
    }

    int socket_fd = socket(dest->in.sin_family, type, 0);
    if (socket_fd < 0) {
        perror("could not open socket");
    }

    //set port if changed
    if (*self_port != 0) {
        sockaddr_t self;
        memset(&self, 0, sizeof(sockaddr_t));
        self.in.sin_family = dest->in.sin_family;

        switch (dest->in.sin_family) {
            case AF_INET:
                self.in.sin_port = *self_port;
                self.in.sin_addr.s_addr = INADDR_ANY;
                break;
            case AF_INET6:
                self.in6.sin6_port = *self_port;
                self.in6.sin6_addr = in6addr_any;
                break;
        }

        int bind_ret = bind(socket_fd, &self.addr, sizeof(sockaddr_t));
        if (bind_ret == -1) {
            perror("couldn't set port");
            return 0;
        }
    }

    if (*protocol == TCP) {
        if (connect(socket_fd, &dest->addr, sizeof(sockaddr_t)) == -1) {
            perror("tcp connection error");
            return 0;
        }
    }

    return socket_fd;
}

size_t send_stdin(int socket, const sockaddr_t *dest, send_fun send, size_t buf_size) {
    uint8_t *buffer[buf_size];

    size_t sum_sent = 0U;
    size_t bytes_read;
    size_t bytes_sent;
    do {
        bytes_read = fread(buffer, 1, buf_size, stdin);
        bytes_sent = send(socket, buffer, bytes_read, dest);
        if ((ssize_t) bytes_sent != bytes_read) {
            perror("send error");
            return sum_sent;
        }
        sum_sent += bytes_sent;
    } while (bytes_read > 0);

    close(socket);
    return sum_sent;
}

ssize_t udp_send(int fd, const void *buffer, size_t n, const sockaddr_t *dest) {
    return sendto(fd, buffer, n, 0, &dest->addr, sizeof(sockaddr_t));
}

ssize_t tcp_send(int fd, const void *buffer, size_t n, const sockaddr_t *dest) {
    return send(fd, buffer, n, 0);
}

size_t send_in(int socket, const sockaddr_t *dest, const protocol_t *protocol, size_t buf_size) {
    send_fun send;
    switch (*protocol) {
        case UDP:
            send = &udp_send;
            break;
        case TCP:
            send = &tcp_send;
            break;
    }
    return send_stdin(socket, dest, send, buf_size);
}
