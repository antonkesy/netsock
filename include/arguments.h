#ifndef NETSOCK_ARGUMENTS_H
#define NETSOCK_ARGUMENTS_H

#include <arpa/inet.h>
#include <stdbool.h>

#include "network.h"

#define PREFIX_CHAR '-'
#define UDP_FLAG_STR "u"
#define TCP_FLAG_STR "t"
#define VERBOSE_FLAG_STR "v"
#define IPV4_FLAG_STR "4"
#define IPV6_FLAG_STR "6"
#define LISTEN_FLAG_STR "l"

typedef struct {
  sockaddr_t sockaddr;
  protocol_t protocol;
  bool is_verbose;
  bool is_listening;
} args_t;

bool parse_args(unsigned int argc, char *argv[], args_t *out_flags);

#endif  // NETSOCK_ARGUMENTS_H
