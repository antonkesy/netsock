#include <stdio.h>
#include <stdlib.h>
#include "isockcom.h"
#include "arguments/arguments.h"
#include "network/network.h"
#include "out/out.h"

int main(int argc, char *argv[]) {
    args_t args;
    if (!parse_args(argc - 1, &argv[1], &args)) {
        return EXIT_FAILURE;
    }

    int socket = prepare_socket(&args.sockaddr, &args.protocol, args.is_listening);

    if (socket < 0) {
        return EXIT_FAILURE;
    }

    size_t bytes_communicated;
    if (args.is_listening) {
        bytes_communicated = recv_in(socket, &args.sockaddr, &args.protocol, 1024);
    } else {
        bytes_communicated = send_in(socket, &args.sockaddr, &args.protocol, 1024);
    }

    PRINTVI(args.isVerbose, "bytes sent: %lu\n", bytes_communicated)

    return 0;
}
