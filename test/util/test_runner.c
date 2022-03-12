#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include "test_runner.h"


void run_ncp(server_func func, struct sockaddr_in *server, const char *args,
             ...) {
    //convert va_args to array
    va_list va_args;
    va_start(va_args, args);

    size_t count_args = 1U;
    while (va_arg(va_args, const char *) != (void *) 0) {
        ++count_args;
    }
    va_start(va_args, args);

    const char *argu[count_args + 2];
    argu[0] = "./ncp";
    argu[1] = args;
    for (int i = 2U; i < (count_args + 2); ++i) {
        argu[i] = va_arg(va_args, const char*);
    }
    argu[count_args + 1] = NULL;

    switch (fork()) {
        case 0:
            execv("./ncp", (char *const *) argu);
            assert(1);
        default:
            //first arg should be file
            func(server, argu[1]);
            break;
    }

    va_end(va_args);

}
