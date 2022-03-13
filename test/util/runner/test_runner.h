#ifndef NETCOPY_TEST_RUNNER_H
#define NETCOPY_TEST_RUNNER_H

#include <stddef.h>
#include <arpa/inet.h>

typedef struct {
    struct sockaddr_in *self;
    void *extra;
} server_args_t;

typedef void (*server_fun)(server_args_t *args);


void run_ncp(server_fun func, server_args_t *server_args, const char *args, ...);

#endif //NETCOPY_TEST_RUNNER_H
