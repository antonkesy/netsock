#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include "network.h"

typedef ssize_t (*send_fun)(int fd, const void *buffer, size_t n, const sockaddr_t *dest);

ssize_t udp_send(int fd, const void *buffer, size_t n, const sockaddr_t *dest);

ssize_t tcp_send(int fd, const void *buffer, size_t n, const sockaddr_t *dest);

typedef ssize_t (*recv_fun)(int fd, void *buffer, size_t n);

ssize_t udp_recv(int fd, void *buffer, size_t n);

ssize_t tcp_recv(int fd, void *buffer, size_t n);

size_t recv_stdin(int socket, recv_fun recv, size_t buf_size);

int prepare_socket(sockaddr_t *dest, const protocol_t *protocol, bool is_listener) {

    int type;
    switch (*protocol) {
        case UDP:
            type = SOCK_DGRAM;
            break;
        case TCP:
            type = SOCK_STREAM;
            break;
        default:
            return -1;
    }

    int socket_fd = socket(dest->in.sin_family, type, 0);
    if (socket_fd < 0) {
        perror("could not open socket");
    }


    if (is_listener == true) {
        switch (dest->in.sin_family) {
            case AF_INET:
                dest->in.sin_addr.s_addr = INADDR_ANY;
                break;
            case AF_INET6:
                dest->in6.sin6_addr = in6addr_any;
                break;
        }
        int bind_ret = bind(socket_fd, &dest->addr, sizeof(sockaddr_t));
        if (bind_ret == -1) {
            perror("couldn't bind socket");
            return -1;
        }
    }

    if (*protocol == TCP) {
        if (is_listener) {
            int list_ret = listen(socket_fd, 1);
            if (list_ret == -1) {
                perror("listen failed");
                return -1;
            }
            socklen_t len = sizeof(struct sockaddr_in);
            int tcp_acc_sock = accept(socket_fd, NULL, &len);
            if (tcp_acc_sock < 0) {
                perror("accepting failed");
            }
            return tcp_acc_sock;
        } else {
            if (connect(socket_fd, &dest->addr, sizeof(sockaddr_t)) == -1) {
                perror("tcp connection error");
                return -1;
            }
        }
    }

    return socket_fd;
}

size_t send_stdin(int socket, const sockaddr_t *dest, send_fun send, size_t buf_size) {
    uint8_t buffer[buf_size];

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

size_t recv_in(int socket, const sockaddr_t *dest, const protocol_t *protocol, size_t buf_size) {
    recv_fun recv;
    switch (*protocol) {
        case UDP:
            recv = &udp_recv;
            break;
        case TCP:
            recv = &tcp_recv;
            break;
    }
    return recv_stdin(socket, recv, buf_size);
}

size_t recv_stdin(int socket, recv_fun recv, size_t buf_size) {
    uint8_t buffer[buf_size];

    size_t sum_recv = 0U;
    size_t bytes_recv;
    do {
        bytes_recv = recv(socket, buffer, buf_size);
        buffer[bytes_recv] = '\0';
        fprintf(stdout, "%s", (char *) buffer);
        sum_recv += bytes_recv;
    } while (bytes_recv > 0);

    close(socket);
    return sum_recv;
}

ssize_t udp_recv(int fd, void *buffer, size_t n) {
    socklen_t len = sizeof(sockaddr_t);
    return recvfrom(fd, buffer, n, 0, NULL, &len);
}

ssize_t tcp_recv(int fd, void *buffer, size_t n) {
    return recv(fd, buffer, n, 0);
}
