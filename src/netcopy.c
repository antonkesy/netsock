#include <fcntl.h>
#include <stdio.h>
#include "arguments/arguments.h"
#include "network/network.h"

int main(int argc, char *argv[]) {
    args_t flags;
    if (!parse_args(argc - 1, &argv[1], &flags)) {
        printf("could not parse args\n");
        return 0;
    }

    int fd = open(flags.file_path, O_RDONLY);
    if (fd < 0) {
        perror("could not open file");
        return 0;
    }

    size_t bytes_sent = send_file(fd, &flags.dest,&flags.self_port, &flags.protocol, 1024);
    printf("bytes sent: %lu\n", bytes_sent);

    return 0;
}
