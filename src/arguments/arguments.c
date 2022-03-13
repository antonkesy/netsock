#include "arguments.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define IS_FLAG(flag) (strcmp(&argv[i][1], (const char*)(flag))==0)

void reset_args(args_t *args);

bool parse_port(const char *in, in_port_t *out_port);

bool parse_ip_destination(const char *argv[2], sockaddr_t *out_dest);

bool parse_args(unsigned int argc, char *argv[], args_t *out_flags) {
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
                out_flags->dest.in.sin_family = AF_INET;
                wasVersionSet = true;
                continue;
            }
            if (IS_FLAG(IPV6_FLAG_STR)) {
                if (wasVersionSet) {
                    printf("double definition of version\n");
                    return false;
                }
                out_flags->dest.in.sin_family = AF_INET6;
                wasVersionSet = true;
                continue;
            }
            if (IS_FLAG(LOCAL_PORT_FLAG_STR)) {
                out_flags->isLocalPort = true;
                if (i + 1 == argc) {
                    printf("missing follow-up argument for local port\n");
                    return false;
                }
                if (!parse_port(argv[i + 1], &out_flags->self_port)) {
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
            printf("unknown option %s\n", argv[i]);
            return false;
        } else {
            printf("missing options prefix by %s\n", argv[i]);
            return false;
        }
    }

    //set default values
    if (!wasProtocolSet) {
        out_flags->protocol = TCP;
    }
    if (!wasVersionSet) {
        out_flags->dest.in.sin_family = AF_INET;
    }

    out_flags->file_path = argv[0];

    if (out_flags->protocol == UDP || out_flags->protocol == TCP) {
        if (!parse_ip_destination((const char **) argv + 1, &out_flags->dest))
            return false;
    }

    return true;
}

bool parse_ip_destination(const char *argv[2], sockaddr_t *out_dest) {
    if (out_dest == NULL) return false;

    void *addr_dest = NULL;
    void *port_dest = NULL;
    switch (out_dest->in.sin_family) {
        case AF_INET:
            addr_dest = &out_dest->in.sin_addr;
            port_dest = &out_dest->in.sin_port;
            break;
        case AF_INET6:
            addr_dest = &out_dest->in6.sin6_addr;
            port_dest = &out_dest->in6.sin6_port;
            break;
    }

    if (inet_pton(out_dest->in.sin_family, argv[0], addr_dest) != 1) {
        printf("not valid destination ip address %s\n", argv[0]);
        return false;
    }

    if (!parse_port(argv[1], port_dest)) {
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
    args->self_port = 0;
    args->isLocalPort = false;
}