#include <fcntl.h>
#include <stdio.h>
#include "arguments/arguments.h"
#include "network/network.h"
#include "out/out.h"

int main(int argc, char *argv[]) {
    args_t flags;
    if (!parse_args(argc - 1, &argv[1], &flags)) {
        return 0;
    }

    int fd = open(flags.file_path, O_RDONLY);
    if (fd < 0) {
        perror("could not open file");
        return 0;
    }
    PRINTVI(flags.isVerbose, "opened file %s\n", flags.file_path)

    size_t bytes_sent = send_file(fd, &flags.dest, &flags.self_port, &flags.protocol, 1024);
    PRINTVI(flags.isVerbose, "bytes sent: %lu\n", bytes_sent)

    return 0;
}
