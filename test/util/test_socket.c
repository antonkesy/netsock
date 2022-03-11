#include "test_socket.h"
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void test_udp_server(struct sockaddr_in *self, const char *expect_file_path) {
    int expect_fd = open(expect_file_path, O_RDONLY);
    assert(expect_fd >= 0);

    int socket_fd = socket(self->sin_family, SOCK_DGRAM, 0);
    assert(socket_fd != -1);

    char *buffer[BUF_SIZE];
    char *file_buf[BUF_SIZE];

    int bind_ret = bind(socket_fd, (const struct sockaddr *) self, sizeof(struct sockaddr));
    assert(bind_ret == 0);

    size_t bytes_read;
    size_t bytes_recv;

    socklen_t len = sizeof(struct sockaddr);

    do {
        bytes_recv = recvfrom(socket_fd, buffer, BUF_SIZE, 0, NULL, &len);
        bytes_read = read(expect_fd, file_buf, BUF_SIZE);
        assert(bytes_recv >= 0);
        assert(bytes_read == bytes_recv);
        assert(memcmp(buffer, file_buf, bytes_recv) == 0);
    } while (bytes_read > 0);

    close(expect_fd);
    close(socket_fd);
}
