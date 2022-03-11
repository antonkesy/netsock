#include <unistd.h>
#include <assert.h>
#include "test_runner.h"


void run_ncp(server_func func, struct sockaddr_in *server, const char *file_path, const char *ip, const char *port,
             ...) {
    switch (fork()) {
        case 0:
            execl("./ncp", "./ncp", file_path, ip, port, "-u", NULL);
            assert(1);
        default:
            func(server, file_path);
            break;
    }

}
