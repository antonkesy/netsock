#include "util/socket/test_socket.h"
#include "util/runner/test_runner.h"
#include "util/file/test_file.h"

#define TEST_FILE_PATH "./test.me"

struct sockaddr_in get_local_socket(int af, uint16_t port) {
    struct sockaddr_in recv;
    recv.sin_family = af;
    inet_pton(recv.sin_family, "127.0.0.1", &(recv.sin_addr));
    recv.sin_port = htons(port);

    return recv;
}

void test_udp() {
    struct sockaddr_in recv = get_local_socket(AF_INET, 55000);
    struct test_file_server_args args;
    args.self = &recv;
    args.expected_file_path = TEST_FILE_PATH;
    run_ncp((server_fun) &test_udp_server, (server_args_t *) &args, TEST_FILE_PATH, "127.0.0.1", "55000", "-u", NULL);
}

void test_tcp() {
    struct sockaddr_in recv = get_local_socket(AF_INET, 55001);

    struct test_file_server_args args;
    args.self = &recv;
    args.expected_file_path = TEST_FILE_PATH;

    run_ncp((server_fun) &test_tcp_server, (server_args_t *) &args, TEST_FILE_PATH, "127.0.0.1", "55001", NULL);
}

void test_set_port() {
    struct sockaddr_in recv = get_local_socket(AF_INET, 55002);

    in_port_t expected_port = htons(34234);

    struct self_port_args args;
    args.self = &recv;
    args.expected_port = &expected_port;

    run_ncp((server_fun) &test_self_port_server_tcp, (server_args_t *) &args, TEST_FILE_PATH, "127.0.0.1", "55002",
            "-p", "34234", NULL);
}

int main() {
    create_test_file(TEST_FILE_PATH);

    test_udp();
    test_tcp();
    test_set_port();
    return 0;
}