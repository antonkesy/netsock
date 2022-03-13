#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "util/socket/test_socket.h"
#include "util/runner/test_runner.h"
#include "util/file/test_file.h"

#define TEST_FILE_PATH "./test.me"

sockaddr_t get_local_socket(int af, uint16_t port) {
    sockaddr_t local_socket;
    memset(&local_socket, 0, sizeof(sockaddr_t));
    local_socket.in.sin_family = af;

    char *ip;
    switch (af) {
        case AF_INET:
            ip = "127.0.0.1";
            break;
        case AF_INET6:
            ip = "::1";
            break;
        default:
            exit(1);
    }

    assert(inet_pton(local_socket.in.sin_family, ip, &(local_socket.in.sin_addr)) != -1);
    local_socket.in.sin_port = htons(port);

    return local_socket;
}

void test_udp_4() {
    sockaddr_t recv = get_local_socket(AF_INET, 55000);

    struct test_file_server_args args;
    args.self = &recv;
    args.expected_file_path = TEST_FILE_PATH;
    run_ncp((server_fun) &test_udp_server, (server_args_t *) &args, TEST_FILE_PATH, "127.0.0.1", "55000", "-u", NULL);
}

void test_udp_6() {
    sockaddr_t recv;
    recv.in6.sin6_family = AF_INET6;
    recv.in6.sin6_port = htons(55300);
    recv.in6.sin6_addr = in6addr_any;

    struct test_file_server_args args;
    args.self = &recv;
    args.expected_file_path = TEST_FILE_PATH;
    run_ncp((server_fun) &test_udp_server, (server_args_t *) &args, TEST_FILE_PATH, "::1", "55300", "-u", "-6", NULL);
}

void test_tcp() {
    sockaddr_t recv = get_local_socket(AF_INET, 55001);

    struct test_file_server_args args;
    args.self = &recv;
    args.expected_file_path = TEST_FILE_PATH;

    run_ncp((server_fun) &test_tcp_server, (server_args_t *) &args, TEST_FILE_PATH, "127.0.0.1", "55001", NULL);
}

void test_set_port() {
    sockaddr_t recv = get_local_socket(AF_INET, 55002);

    in_port_t expected_port = htons(34234);

    struct self_port_args args;
    args.self = &recv;
    args.expected_port = &expected_port;

    run_ncp((server_fun) &test_self_port_server_tcp, (server_args_t *) &args, TEST_FILE_PATH, "127.0.0.1", "55002",
            "-p", "34234", NULL);
}

int main() {
    create_test_file(TEST_FILE_PATH);

    test_udp_4();
    //test_udp_6();
    test_tcp();
    test_set_port();
    return 0;
}