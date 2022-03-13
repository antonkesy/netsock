#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "util/socket/test_socket.h"
#include "util/runner/test_runner.h"
#include "util/file/test_file.h"

#define TEST_FILE_PATH "./test.me"

#define IP4_LOOPBACK_STR "127.0.0.1"
#define IP6_LOOPBACK_STR "::1"

sockaddr_t get_local_socket(int af, uint16_t port) {
    sockaddr_t local_socket;
    memset(&local_socket, 0, sizeof(sockaddr_t));
    local_socket.in.sin_family = af;
    local_socket.in.sin_port = htons(port);

    switch (af) {
        case AF_INET:
            assert(inet_pton(af, IP4_LOOPBACK_STR, &(local_socket.in.sin_addr)) != -1);
            break;
        case AF_INET6:
            assert(inet_pton(af, IP6_LOOPBACK_STR, &(local_socket.in6.sin6_addr)) != -1);
            break;
        default:
            exit(1);
    }

    return local_socket;
}

void test_udp_4() {
    sockaddr_t recv = get_local_socket(AF_INET, 55000);

    struct test_file_server_args args;
    args.self = &recv;
    args.expected_file_path = TEST_FILE_PATH;
    run_ncp((server_fun) &test_udp_server, (server_args_t *) &args, TEST_FILE_PATH, IP4_LOOPBACK_STR, "55000", "-u",
            NULL);
}

void test_udp_6() {
    sockaddr_t recv = get_local_socket(AF_INET6, 55300);

    struct test_file_server_args args;
    args.self = &recv;
    args.expected_file_path = TEST_FILE_PATH;
    run_ncp((server_fun) &test_udp_server, (server_args_t *) &args, TEST_FILE_PATH, IP6_LOOPBACK_STR, "55300", "-u",
            "-6", NULL);
}

void test_tcp_4() {
    sockaddr_t recv = get_local_socket(AF_INET, 55001);

    struct test_file_server_args args;
    args.self = &recv;
    args.expected_file_path = TEST_FILE_PATH;

    run_ncp((server_fun) &test_tcp_server, (server_args_t *) &args, TEST_FILE_PATH, IP4_LOOPBACK_STR, "55001", NULL);
}

void test_tcp_6() {
    sockaddr_t recv = get_local_socket(AF_INET6, 55201);

    struct test_file_server_args args;
    args.self = &recv;
    args.expected_file_path = TEST_FILE_PATH;

    run_ncp((server_fun) &test_tcp_server, (server_args_t *) &args, TEST_FILE_PATH, IP6_LOOPBACK_STR, "55201", "-6",
            NULL);
}

void test_set_port_4() {
    sockaddr_t recv = get_local_socket(AF_INET, 55002);

    in_port_t expected_port = htons(34234);

    struct self_port_args args;
    args.self = &recv;
    args.expected_port = &expected_port;

    run_ncp((server_fun) &test_self_port_server_tcp, (server_args_t *) &args, TEST_FILE_PATH, IP4_LOOPBACK_STR, "55002",
            "-p", "34234", NULL);
}

void test_set_port_6() {
    sockaddr_t recv = get_local_socket(AF_INET6, 54343);

    in_port_t expected_port = htons(44444);

    struct self_port_args args;
    args.self = &recv;
    args.expected_port = &expected_port;

    run_ncp((server_fun) &test_self_port_server_tcp, (server_args_t *) &args, TEST_FILE_PATH, IP6_LOOPBACK_STR, "54343",
            "-p", "44444", "-6", NULL);
}

int main() {
    create_test_file(TEST_FILE_PATH);

    test_udp_4();
    test_udp_6();
    test_tcp_4();
    test_tcp_6();
    test_set_port_4();
    test_set_port_6();
    return 0;
}