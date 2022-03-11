#include "util/test_socket.h"
#include "util/test_runner.h"
#include "util/test_file.h"

#define TEST_FILE_PATH "./test.me"

void test_udp() {
    struct sockaddr_in recv;
    recv.sin_family = AF_INET;
    inet_pton(recv.sin_family, "127.0.0.1", &(recv.sin_addr));
    recv.sin_port = htons(55000);

    run_ncp(&test_udp_server, &recv, TEST_FILE_PATH, "127.0.0.1", "55000");
}

int main() {
    create_test_file(TEST_FILE_PATH);

    test_udp();
    return 0;
}