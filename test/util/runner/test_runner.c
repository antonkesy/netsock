#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <fcntl.h>
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

    int file_fd = -1;
    if (server_args->is_stdin) {
        file_fd = open(server_args->file_path, O_RDONLY);
        if (file_fd < 0) {
            perror("open");
        }
    }
    switch (fork()) {
        case 0:
            if (server_args->is_stdin) {
                dup2(file_fd, STDIN_FILENO);
                close(file_fd);
            }
            execv("./netcopy", (char *const *) argu);
            PASSERT(false, "exec failed")
        default:
            //first arg should be file
            func(server_args);
            break;
    }

    va_end(va_args);

}
