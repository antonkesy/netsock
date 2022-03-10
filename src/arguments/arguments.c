#include "arguments.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define IS_FLAG(flag) (strcmp(&argv[i][1], (const char*)flag)==0)

void reset_args(args_t *args);

bool parse_port(const char *in, in_port_t *out_port);

bool parse_args(unsigned int argc, char *argv[], int af, args_t *out_flags) {
    if (argc < MIN_ARGC) {
        printf("too few arguments\n");
        return false;
    }

    reset_args(out_flags);

    bool wasProtocolSet = false;
    bool wasVersionSet = false;
    for (unsigned int i = 3U; i < argc; ++i) {
        if (argv[i][0] == PREFIX_CHAR) {
            if (IS_FLAG(UDP_FLAG_STR)) {
                if (wasProtocolSet) {
                    printf("double definition of protocol\n");
                    return false;
                }
                out_flags->protocol = UDP;
                wasProtocolSet = true;
                continue;
            }
            if (IS_FLAG(TCP_FLAG_STR)) {
                if (wasProtocolSet) {
                    printf("double definition of protocol\n");
                    return false;
                }
                out_flags->protocol = TCP;
                wasProtocolSet = true;
                continue;
            }
            if (IS_FLAG(IPV4_FLAG_STR)) {
                if (wasVersionSet) {
                    printf("double definition of version\n");
                    return false;
                }
                out_flags->ipv = _4;
                wasVersionSet = true;
                continue;
            }
            if (IS_FLAG(IPV6_FLAG_STR)) {
                if (wasVersionSet) {
                    printf("double definition of version\n");
                    return false;
                }
                out_flags->ipv = _6;
                wasVersionSet = true;
                continue;
            }
            if (IS_FLAG(LOCAL_IP_FLAG_STR)) {
                out_flags->isLocalIP = true;
                if (i + 1 == argc) {
                    printf("missing follow-up argument for local ip\n");
                    return false;
                }
                if (inet_pton(af, argv[i + 1], &(out_flags->src.sin_addr)) != 1) {
                    printf("not valid source ip address\n");
                    return false;
                }
                ++i;
                continue;
            }
            if (IS_FLAG(LOCAL_PORT_FLAG_STR)) {
                out_flags->isLocalPort = true;
                if (i + 1 == argc) {
                    printf("missing follow-up argument for local port\n");
                    return false;
                }
                if (!parse_port(argv[i + 1], &out_flags->src.sin_port)) {
                    printf("not valid source port\n");
                    return false;
                }
                ++i;
                continue;
            }
            if (IS_FLAG(VERBOSE_FLAG_STR)) {
                out_flags->isVerbose = true;
                continue;
            }
            printf("unknown option\n");
            return false;
        } else {
            printf("missing options prefix\n");
            return false;
        }
    }

    out_flags->file_path = argv[0];

    out_flags->dest.sin_family = af;
    if (inet_pton(af, argv[1], &(out_flags->dest.sin_addr)) != 1) {
        printf("not valid destination ip address\n");
        return false;
    }

    if (!parse_port(argv[2], &out_flags->dest.sin_port)) {
        printf("not valid destination port\n");
        return false;
    }

    return true;
}

bool parse_port(const char *in, in_port_t *out_port) {
    if (out_port == NULL)
        return false;

    char *end;
    long port = strtol(in, &end, 10);
    size_t len = strlen(in);
    size_t end_offset = (end - in);
    if (end_offset != len || port == 0 || port > 65535) {
        return false;
    }

    *out_port = htons(port);
    return true;
}

void reset_args(args_t *args) {
    args->file_path = NULL;
    args->protocol = TCP;
    args->isVerbose = false;
    args->isLocalIP = false;
    args->isLocalPort = false;
}
