#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include "test_runner.h"
#include "../out/out.h"


void run_ncp(server_fun func, server_args_t *server_args, const char *args, ...) {
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
            PASSERT(false, "exec failed")
        default:
            //first arg should be file
            func(server_args);
            break;
    }

    va_end(va_args);

}
