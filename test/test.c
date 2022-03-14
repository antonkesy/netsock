#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "util/socket/test_socket.h"
#include "util/runner/test_runner.h"
#include "util/file/test_file.h"
#include "util/out/out.h"

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
            PASSERT(inet_pton(af, IP4_LOOPBACK_STR, &(local_socket.in.sin_addr)) != -1, "couldn't parse IPv4 addr")
            break;
        case AF_INET6:
            PASSERT(inet_pton(af, IP6_LOOPBACK_STR, &(local_socket.in6.sin6_addr)) != -1, "couldn't parse IPv6 addr")
            break;
        default:
            exit(1);
    }

    return local_socket;
}

void test_udp_4() {
    sockaddr_t recv = get_local_socket(AF_INET, 55000);

    server_args_t args;
    args.self = &recv;
    args.file_path = TEST_FILE_PATH;
    args.is_stdin = true;
    run_ncp((server_fun) &test_udp_server, &args, IP4_LOOPBACK_STR, "55000", "-u",
            NULL);
}

void test_udp_6() {
    sockaddr_t recv = get_local_socket(AF_INET6, 55300);

    server_args_t args;
    args.self = &recv;
    args.file_path = TEST_FILE_PATH;
    args.is_stdin = true;
    run_ncp((server_fun) &test_udp_server, &args, IP6_LOOPBACK_STR, "55300", "-u",
            "-6", NULL);
}

void test_tcp_4() {
    sockaddr_t recv = get_local_socket(AF_INET, 55001);

    server_args_t args;
    args.self = &recv;
    args.file_path = TEST_FILE_PATH;
    args.is_stdin = true;

    run_ncp((server_fun) &test_tcp_server, &args, IP4_LOOPBACK_STR, "55001", NULL);
}

void test_tcp_6() {
    sockaddr_t recv = get_local_socket(AF_INET6, 55201);

    server_args_t args;
    args.self = &recv;
    args.file_path = TEST_FILE_PATH;
    args.is_stdin = true;

    run_ncp((server_fun) &test_tcp_server, &args, IP6_LOOPBACK_STR, "55201", "-6",
            NULL);
}

int main() {
    create_test_file(TEST_FILE_PATH);

    test_udp_4();
    test_udp_6();
    test_tcp_4();
    test_tcp_6();

    return 0;
}