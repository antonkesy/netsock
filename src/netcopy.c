#include <stdio.h>
#include "arguments/arguments.h"
#include "network/network.h"
#include "out/out.h"

int main(int argc, char *argv[]) {
    args_t flags;
    if (!parse_args(argc - 1, &argv[1], &flags)) {
        return 0;
    }

    int socket = prepare_socket(&flags.dest, &flags.self_port, &flags.protocol);

    size_t bytes_sent = send_in(socket, &flags.dest, &flags.protocol, 1024);

    PRINTVI(flags.isVerbose, "bytes sent: %lu\n", bytes_sent)

    return 0;
}
