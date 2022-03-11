#ifndef NETCOPY_TEST_RUNNER_H
#define NETCOPY_TEST_RUNNER_H

#include <stddef.h>
#include <arpa/inet.h>

typedef void (*server_func)(struct sockaddr_in *self, const char *expect_file_path);


void run_ncp(server_func func, struct sockaddr_in *server, const char *file_path, const char *ip, const char *port,
             ...);

#endif //NETCOPY_TEST_RUNNER_H
