#include <stdio.h>
#include <unistd.h>
#include "send.h"
#include "../../out/out.h"

size_t send_stdin(int socket, const sockaddr_t *dest, send_fun send, size_t buf_size) {
    uint8_t buffer[buf_size];

    size_t sum_sent = 0U;
    size_t bytes_read;
    size_t bytes_sent;
    do {
        bytes_read = fread(buffer, 1, buf_size, stdin);
        bytes_sent = send(socket, buffer, bytes_read, dest);
        PRINTVI("sent %li bytes\n", bytes_read)
        if ((ssize_t) bytes_sent != bytes_read) {
            perror("send error");
            return sum_sent;
        }
        sum_sent += bytes_sent;
    } while (bytes_read > 0);

    close(socket);
    return sum_sent;
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

ssize_t udp_send(int fd, const void *buffer, size_t n, const sockaddr_t *dest) {
    return sendto(fd, buffer, n, 0, &dest->addr, sizeof(sockaddr_t));
}

ssize_t tcp_send(int fd, const void *buffer, size_t n, const sockaddr_t *dest) {
    return send(fd, buffer, n, 0);
}
