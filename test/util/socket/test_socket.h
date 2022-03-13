#ifndef NETCOPY_TEST_SOCKET_H
#define NETCOPY_TEST_SOCKET_H

#include <stddef.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "../runner/test_runner.h"
#include "../../../src/network/network.h"


struct test_file_server_args {
    sockaddr_t *self;
    const char *expected_file_path;
};

void test_udp_server(struct test_file_server_args *args);

void test_tcp_server(struct test_file_server_args *args);

struct self_port_args {
    sockaddr_t *self;
    in_port_t *expected_port;
};

void test_self_port_server_tcp(struct self_port_args *args);

#endif //NETCOPY_TEST_SOCKET_H
