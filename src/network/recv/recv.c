#include <stdio.h>
#include <unistd.h>
#include "recv.h"

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
