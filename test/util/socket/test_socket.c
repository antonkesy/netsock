#include "test_socket.h"
#include "../out/out.h"
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

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
    PEASSERT(listen(socket_fd, 1) != -1, "listen failed")
    socklen_t len = sizeof(struct sockaddr_in);
    int tcp_fd = accept(socket_fd, &out, &len);
    PEASSERT(tcp_fd != -1, "tcp accept failed")
    return tcp_fd;
}

void
test_file_socket(sockaddr_t *self, const char *expect_file_path, int type, prep_sock prep, recv_fun recv) {
    int expect_fd = open(expect_file_path, O_RDONLY);
    PEASSERT(expect_fd >= 0, "expected file open failed")

    int socket_fd = socket(self->in.sin_family, type, 0);
    PEASSERT(socket_fd >= 0, "server socket failed")

    int bind_ret = bind(socket_fd, &self->addr, sizeof(sockaddr_t));
    PEASSERT(bind_ret == 0, "server bind failed")

    if (prep != NULL) {
        socket_fd = prep(socket_fd);
    }

    uint size = sizeof(uint);
    int buf_size;
    int get_opt = getsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, (void *) &buf_size, &size);
    PEASSERT(get_opt != -1, "get socket options failed");

    char recv_buf[buf_size];
    char file_buf[buf_size];


    ssize_t bytes_read;
    ssize_t bytes_recv;

    do {
        bytes_recv = recv(socket_fd, recv_buf, buf_size);
        PEASSERT(bytes_recv >= 0, "recv failed")
        bytes_read = read(expect_fd, file_buf, bytes_recv);
        PEASSERT(bytes_read == bytes_recv, "read != recv")
        PASSERT(memcmp(recv_buf, file_buf, bytes_read) == 0, "buffer not the same")
    } while (bytes_read > 0);
    //check EOF
    PASSERT(read(expect_fd, file_buf, 1) == 0, "not end of file")

    close(expect_fd);
    close(socket_fd);
}

void test_udp_server(server_args_t *args) {
    test_file_socket(args->self, args->file_path, SOCK_DGRAM, NULL, recv_udp);
}

void test_tcp_server(server_args_t *args) {
    test_file_socket(args->self, args->file_path, SOCK_STREAM, prep_tcp, recv_tcp);
}

void test_self_port_server_tcp(server_args_t *args) {
    int socket_fd = socket(args->self->in.sin_family, SOCK_STREAM, 0);
    PEASSERT(socket_fd != -1, "server socket failed")

    int bind_ret = bind(socket_fd, &args->self->addr, sizeof(sockaddr_t));
    PEASSERT(bind_ret == 0, "server bind failed")

    struct sockaddr_in out;
    PEASSERT(listen(socket_fd, 1) != -1, "server listen failed")
    socklen_t len = sizeof(struct sockaddr_in);
    int tcp_fd = accept(socket_fd, (struct sockaddr *) &out, &len);
    PEASSERT(tcp_fd != -1, "server accept failed")

    assert(out.sin_port == *(in_port_t *) args->extra);

    close(socket_fd);
}
