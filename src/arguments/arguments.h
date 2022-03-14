#ifndef NETCOPY_ARGUMENTS_H
#define NETCOPY_ARGUMENTS_H

#include <stdbool.h>
#include <arpa/inet.h>
#include "../netcopy.h"
#include "../network/network.h"

#define PREFIX_CHAR '-'
#define UDP_FLAG_STR "u"
#define TCP_FLAG_STR "t"
#define VERBOSE_FLAG_STR "v"
#define IPV4_FLAG_STR "4"
#define IPV6_FLAG_STR "6"
#define LISTEN_FLAG_STR "l"

#define MIN_ARGC 2 //DEST_IP DEST_PORT

typedef struct {
    sockaddr_t dest;
    protocol_t protocol;
    bool isVerbose;
    bool is_listening;
} args_t;

bool parse_args(unsigned int argc, char *argv[], args_t *out_flags);

#endif //NETCOPY_ARGUMENTS_H
