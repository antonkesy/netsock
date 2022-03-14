#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include "network.h"

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