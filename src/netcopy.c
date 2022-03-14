#include <stdio.h>
#include <stdlib.h>
#include "netcopy.h"
#include "arguments/arguments.h"
#include "network/network.h"
#include "out/out.h"

#ifdef STANDALONE

int main(int argc, char *argv[]) {
    return netcopy(argc, argv);
}

#endif

int netcopy(int argc, char *argv[]) {
    args_t flags;
    if (!parse_args(argc - 1, &argv[1], &flags)) {
        return EXIT_FAILURE;
    }

    int socket = prepare_socket(&flags.dest, &flags.protocol, flags.is_listening);

    if (socket < 0) {
        return EXIT_FAILURE;
    }

    size_t bytes_communicated;
    if (flags.is_listening) {
        bytes_communicated = recv_in(socket, &flags.dest, &flags.protocol, 1024);
    } else {
        bytes_communicated = send_in(socket, &flags.dest, &flags.protocol, 1024);
    }

    PRINTVI(flags.isVerbose, "bytes sent: %lu\n", bytes_communicated)

    return 0;
}
