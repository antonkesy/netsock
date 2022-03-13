#include "test_socket.h"
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

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
test_file_socket(struct sockaddr_in *self, const char *expect_file_path, int type, prep_sock prep, recv_fun recv) {
    int expect_fd = open(expect_file_path, O_RDONLY);
    assert(expect_fd >= 0);

    int socket_fd = socket(self->sin_family, type, 0);
    assert(socket_fd != -1);

    int bind_ret = bind(socket_fd, (const struct sockaddr *) self, sizeof(struct sockaddr));
    assert(bind_ret == 0);

    if (prep != NULL) {
        socket_fd = prep();
    }

    char recv_buf[BUF_SIZE];
    char file_buf[BUF_SIZE];


    ssize_t bytes_read;
    ssize_t bytes_recv;


    do {
        //TODO not correct when not same read/recv size
        //TODO add file pos pointers
        //TODO work with stream
        bytes_recv = recv(socket_fd, recv_buf, BUF_SIZE);
        bytes_read = read(expect_fd, file_buf, BUF_SIZE);
        assert(bytes_recv >= 0);
        assert(bytes_read == bytes_recv);
        assert(memcmp(recv_buf, file_buf, bytes_read) == 0);
    } while (bytes_read > 0);

    close(expect_fd);
    close(socket_fd);
}

void test_udp_server(struct sockaddr_in *self, const char *expect_file_path) {
    test_file_socket(self, expect_file_path, SOCK_DGRAM, NULL, recv_udp);
}

void test_tcp_server(struct sockaddr_in *self, const char *expect_file_path) {
    test_file_socket(self, expect_file_path, SOCK_STREAM, prep_tcp, recv_tcp);
}
