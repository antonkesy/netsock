#include "arguments/arguments.h"

int main(int argc, char *argv[]) {
    args_t flags;
    if (!parse_args(argc - 1, &argv[1], &flags))
        return 0;

    return 0;
}
