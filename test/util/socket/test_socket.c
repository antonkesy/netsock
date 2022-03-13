#include "test_socket.h"
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <stdio.h>

typedef ssize_t (*recv_fun)(int socket_fd, void *buffer, size_t n);

typedef int(*prep_sock)();

ssize_t recv_udp(int socket_fd, void *buffer, size_t n) {
    socklen_t len = sizeof(struct sockaddr);
    return recvfrom(socket_fd, buffer, n, 0, NULL, &len);
}

ssize_t recv_tcp(int socket_fd, void *buffer, size_t n) {
    return recv(socket_fd, buffer, n, 0);
}

int prep_tcp(int socket_fd) {
    struct sockaddr out;
    assert(listen(socket_fd, 1) != -1);
    socklen_t len = sizeof(struct sockaddr_in);
    int tcp_fd = accept(socket_fd, &out, &len);
    assert(tcp_fd != -1);
    return tcp_fd;
}

void
test_file_socket(sockaddr_t *self, const char *expect_file_path, int type, prep_sock prep, recv_fun recv) {
    int expect_fd = open(expect_file_path, O_RDONLY);
    assert(expect_fd >= 0);

    int socket_fd = socket(self->in.sin_family, type, 0);
    assert(socket_fd >= 0);

    int bind_ret = bind(socket_fd, &self->addr, sizeof(struct sockaddr));
    assert(bind_ret == 0);

    if (prep != NULL) {
        socket_fd = prep(socket_fd);
    }

    uint size = sizeof(uint);
    int buf_size;
    int get_opt = getsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, (void *) &buf_size, &size);
    assert(get_opt != -1);

    char recv_buf[buf_size];
    char file_buf[buf_size];


    ssize_t bytes_read;
    ssize_t bytes_recv;

    do {
        bytes_recv = recv(socket_fd, recv_buf, buf_size);
        assert(bytes_recv >= 0);
        bytes_read = read(expect_fd, file_buf, bytes_recv);
        assert(bytes_read == bytes_recv);
        assert(memcmp(recv_buf, file_buf, bytes_read) == 0);
    } while (bytes_read > 0);
    //check EOF
    assert(read(expect_fd, file_buf, 1) == 0);

    close(expect_fd);
    close(socket_fd);
}

void test_udp_server(struct test_file_server_args *args) {
    test_file_socket(args->self, args->expected_file_path, SOCK_DGRAM, NULL, recv_udp);
}

void test_tcp_server(struct test_file_server_args *args) {
    test_file_socket(args->self, args->expected_file_path, SOCK_STREAM, prep_tcp, recv_tcp);
}

void test_self_port_server_tcp(struct self_port_args *args) {
    int socket_fd = socket(args->self->in.sin_family, SOCK_STREAM, 0);
    assert(socket_fd != -1);

    int bind_ret = bind(socket_fd, (const struct sockaddr *) args->self, sizeof(struct sockaddr));
    assert(bind_ret == 0);

    struct sockaddr_in out;
    assert(listen(socket_fd, 1) != -1);
    socklen_t len = sizeof(struct sockaddr_in);
    int tcp_fd = accept(socket_fd, (struct sockaddr *) &out, &len);
    assert(tcp_fd != -1);

    assert(out.sin_port == *args->expected_port);
}
