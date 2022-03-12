#ifndef NETCOPY_TEST_SOCKET_H
#define NETCOPY_TEST_SOCKET_H

#include <stddef.h>
#include <stdbool.h>
#include <arpa/inet.h>

void test_udp_server(struct sockaddr_in *self, const char *expect_file_path);

void test_tcp_server(struct sockaddr_in *self, const char *expect_file_path);

#endif //NETCOPY_TEST_SOCKET_H
