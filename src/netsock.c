#include "netsock.h"

#include <stdio.h>
#include <stdlib.h>

#include "arguments/arguments.h"
#include "network/network.h"
#include "network/recv/recv.h"
#include "network/send/send.h"
#include "out/out.h"

bool is_verbose;

int main(int argc, char *argv[]) {
  args_t args;
  if (!parse_args(argc - 1, &argv[1], &args)) {
    return EXIT_FAILURE;
  }

  is_verbose = args.is_verbose;

  int socket =
      prepare_socket(&args.sockaddr, &args.protocol, args.is_listening);

  if (socket < 0) {
    return EXIT_FAILURE;
  }

  int buffer_size = get_buffer_size(socket, args.is_listening);
  size_t bytes_communicated;
  if (args.is_listening) {
    bytes_communicated = recv_in(socket, &args.protocol, buffer_size);
  } else {
    bytes_communicated =
        send_in(socket, &args.sockaddr, &args.protocol, buffer_size);
  }

  PRINTVI("total bytes sent: %lu\n", bytes_communicated)

  return 0;
}

int get_buffer_size(int socket_fd, bool is_listener) {
  uint size = sizeof(uint);
  int buf_size;
  int get_opt =
      getsockopt(socket_fd, SOL_SOCKET, is_listener ? SO_RCVBUF : SO_SNDBUF,
                 (void *)&buf_size, &size);
  if (get_opt == -1) {
    PRINTE("couldn't get system socket size");
    buf_size = DEFAULT_BUF_SIZE;
  }

  PRINTVI("set socket buffer to: %d\n", buf_size)

  return buf_size;
}