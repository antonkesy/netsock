#ifndef NETCOPY_TEST_SOCKET_H
#define NETCOPY_TEST_SOCKET_H

#include <stddef.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "../runner/test_runner.h"
#include "../../../src/network/network.h"

void test_udp_server(server_args_t *args);

void test_tcp_server(server_args_t *args);

void test_self_port_server_tcp(server_args_t *args);

#endif //NETCOPY_TEST_SOCKET_H
