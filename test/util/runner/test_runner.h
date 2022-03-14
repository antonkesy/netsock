#ifndef NETCOPY_TEST_RUNNER_H
#define NETCOPY_TEST_RUNNER_H

#include <stddef.h>
#include <arpa/inet.h>
#include "../../../src/network/network.h"

typedef struct {
    sockaddr_t *self;
    bool is_stdin;
    const char *file_path;
    void *extra;
} server_args_t;

typedef void (*server_fun)(server_args_t *args);

void run_ncp(server_fun func, server_args_t *server_args, const char *args, ...);

#endif //NETCOPY_TEST_RUNNER_H
