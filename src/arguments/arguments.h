#ifndef NETCOPY_ARGUMENTS_H
#define NETCOPY_ARGUMENTS_H

#include <stdbool.h>
#include <arpa/inet.h>

// ncp FILE_PATH DEST_IP DEST_PORT [OPTIONS]

#define PREFIX_CHAR '-'
#define UDP_FLAG_STR "u"
#define TCP_FLAG_STR "t"
#define LOCAL_PORT_FLAG_STR "p"
#define LOCAL_IP_FLAG_STR "i"
#define VERBOSE_FLAG_STR "v"
#define IPV4_FLAG_STR "4"
#define IPV6_FLAG_STR "6"

#define AF_IP4 AF_INET
#define AF_IP6 AF_INET6

#define MIN_ARGC 3 //FILE_PATH DEST_IP DEST_PORT

enum IPV {
    _4, _6
};

enum Protocol {
    UDP, TCP
};

typedef struct {
    const char *file_path;
    struct sockaddr_in dest;
    struct sockaddr_in src;
    enum IPV ipv;
    enum Protocol protocol;
    bool isLocalPort;
    bool isLocalIP;
    bool isVerbose;
} args_t;

bool parse_args(unsigned int argc, char *argv[], int af, args_t *out_flags);

#endif //NETCOPY_ARGUMENTS_H
