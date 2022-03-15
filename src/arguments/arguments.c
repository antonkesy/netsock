#include "arguments.h"
#include "../out/out.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define IS_FLAG(opt_str, flag) (strcmp(opt_str, (const char*)(flag))==0)

void reset_args(args_t *args);

bool parse_port(const char *in, in_port_t *out_port);

bool parse_ip_destination(const char *argv[2], sockaddr_t *out_dest, bool wasVersionSet);

bool parse_args(unsigned int argc, char *argv[], args_t *out_flags) {
    if (argc < MIN_ARGC) {
        PRINTE("too few arguments")
        return false;
    }

    reset_args(out_flags);

    bool was_protocol_set = false;
    bool was_version_set = false;

    for (unsigned int i = 2U; i < argc; ++i) {
        if (argv[i][0] == PREFIX_CHAR) {
            const char *option = &argv[i][1];

            if (IS_FLAG(option, UDP_FLAG_STR)) {
                if (was_protocol_set) {
                    PRINTE("double definition of protocol")
                    return false;
                }
                out_flags->protocol = UDP;
                was_protocol_set = true;
                continue;
            }

            if (IS_FLAG(option, TCP_FLAG_STR)) {
                if (was_protocol_set) {
                    PRINTE("double definition of protocol")
                    return false;
                }
                out_flags->protocol = TCP;
                was_protocol_set = true;
                continue;
            }

            if (IS_FLAG(option, IPV4_FLAG_STR)) {
                if (was_version_set) {
                    PRINTE("double definition of version")
                    return false;
                }
                out_flags->sockaddr.in.sin_family = AF_INET;
                was_version_set = true;
                continue;
            }

            if (IS_FLAG(option, IPV6_FLAG_STR)) {
                if (was_version_set) {
                    PRINTE("double definition of version")
                    return false;
                }
                out_flags->sockaddr.in.sin_family = AF_INET6;
                was_version_set = true;
                continue;
            }

            if (IS_FLAG(option, VERBOSE_FLAG_STR)) {
                out_flags->is_verbose = true;
                continue;
            }

            if (IS_FLAG(option, LISTEN_FLAG_STR)) {
                out_flags->is_listening = true;
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
    if (!was_protocol_set) {
        out_flags->protocol = TCP;
    }
    if (!was_version_set) {
        out_flags->sockaddr.in.sin_family = AF_INET;
    }

    if (out_flags->protocol == UDP || out_flags->protocol == TCP) {
        if (!parse_ip_destination((const char **) argv, &out_flags->sockaddr, was_version_set))
            return false;
    }

    return true;
}

bool parse_ip_destination(const char *argv[2], sockaddr_t *out_dest, bool wasVersionSet) {
    if (out_dest == NULL) return false;

    void *port_dest = NULL;

    if (wasVersionSet == true) {
        void *addr_dest = NULL;
        switch (out_dest->in.sin_family) {
            case AF_INET:
                addr_dest = &out_dest->in.sin_addr;
                port_dest = &out_dest->in.sin_port;
                break;
            case AF_INET6:
                addr_dest = &out_dest->in6.sin6_addr;
                port_dest = &out_dest->in6.sin6_port;
                break;
            default:
                return false;
        }

        if (strcmp(argv[0], "localhost") == 0) {
            switch (out_dest->in.sin_family) {
                case AF_INET:
                    out_dest->in.sin_addr.s_addr = INADDR_LOOPBACK;
                    break;
                case AF_INET6:
                    out_dest->in6.sin6_addr = in6addr_loopback;
                    break;
                default:
                    return false;
            }
        } else {
            if (inet_pton(out_dest->in.sin_family, argv[0], addr_dest) != 1) {
                PRINTEI("not valid destination ip address ", argv[0])
                return false;
            }
        }

    } else {
        //Test parsing ipv4/v6
        port_dest = &out_dest->in.sin_port;
        int inet_pton_result = inet_pton(AF_INET, argv[0], &out_dest->in.sin_addr);
        if (inet_pton_result == -1) {
            port_dest = &out_dest->in6.sin6_port;
            inet_pton_result = inet_pton(AF_INET6, argv[0], &out_dest->in6.sin6_addr);
            if (inet_pton_result == -1) {
                PRINTEI("not valid destination ipv4 or ipv6 address ", argv[0])
                return false;
            }
        }
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
    args->is_listening = false;
}