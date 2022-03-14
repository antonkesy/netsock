#include "arguments.h"
#include "../out/out.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define IS_FLAG(opt_str, flag) (strcmp(opt_str, (const char*)(flag))==0)

void reset_args(args_t *args);

bool parse_port(const char *in, in_port_t *out_port);

bool parse_ip_destination(const char *argv[2], sockaddr_t *out_dest);

bool parse_args(unsigned int argc, char *argv[], args_t *out_flags) {
    if (argc < MIN_ARGC) {
        PRINTE("too few arguments")
        return false;
    }

    reset_args(out_flags);

    bool wasProtocolSet = false;
    bool wasVersionSet = false;

    for (unsigned int i = 2U; i < argc; ++i) {
        if (argv[i][0] == PREFIX_CHAR) {
            const char *option = &argv[i][1];
            if (IS_FLAG(option, UDP_FLAG_STR)) {
                if (wasProtocolSet) {
                    PRINTE("double definition of protocol")
                    return false;
                }
                out_flags->protocol = UDP;
                wasProtocolSet = true;
                continue;
            }
            if (IS_FLAG(option, TCP_FLAG_STR)) {
                if (wasProtocolSet) {
                    PRINTE("double definition of protocol")
                    return false;
                }
                out_flags->protocol = TCP;
                wasProtocolSet = true;
                continue;
            }
            if (IS_FLAG(option, IPV4_FLAG_STR)) {
                if (wasVersionSet) {
                    PRINTE("double definition of version")
                    return false;
                }
                out_flags->dest.in.sin_family = AF_INET;
                wasVersionSet = true;
                continue;
            }
            if (IS_FLAG(option, IPV6_FLAG_STR)) {
                if (wasVersionSet) {
                    PRINTE("double definition of version")
                    return false;
                }
                out_flags->dest.in.sin_family = AF_INET6;
                wasVersionSet = true;
                continue;
            }
            if (IS_FLAG(option, LOCAL_PORT_FLAG_STR)) {
                out_flags->isLocalPort = true;
                if (i + 1 == argc) {
                    PRINTE("missing follow-up argument for local port")
                    return false;
                }
                if (!parse_port(argv[i + 1], &out_flags->self_port)) {
                    PRINTE("not valid source port")
                    return false;
                }
                ++i;
                continue;
            }
            if (IS_FLAG(option, VERBOSE_FLAG_STR)) {
                out_flags->isVerbose = true;
                continue;
            }
            PRINTEI("unknown option", argv[i])
            return false;
        } else {
            PRINTEI("missing options prefix by ", argv[i])
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

    if (out_flags->protocol == UDP || out_flags->protocol == TCP) {
        if (!parse_ip_destination((const char **) argv, &out_flags->dest))
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
        PRINTEI("not valid destination ip address ", argv[0])
        return false;
    }

    if (!parse_port(argv[1], port_dest)) {
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
        PRINTEI("port out of range ", out_port)
        return false;
    }

    *out_port = htons(port);
    return true;
}

void reset_args(args_t *args) {
    args->protocol = TCP;
    args->isVerbose = false;
    args->self_port = 0;
    args->isLocalPort = false;
}