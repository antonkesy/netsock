#include "network.h"

#include <memory.h>
#include <stdio.h>
#include <unistd.h>

#include "../out/out.h"

int prepare_sender(sockaddr_t *addr, const protocol_t *protocol,
                   int socket_fd) {
  if (*protocol == TCP) {
    PRINTV("connect\n")
    if (connect(socket_fd, &addr->addr, sizeof(sockaddr_t)) == -1) {
      perror("tcp connection error");
      return -1;
    }
  }
  return socket_fd;
}

int prepare_listener(sockaddr_t *addr, const protocol_t *protocol,
                     int socket_fd) {
  switch (addr->in.sin_family) {
    case AF_INET:
      addr->in.sin_addr.s_addr = INADDR_ANY;
      break;
    case AF_INET6:
      addr->in6.sin6_addr = in6addr_any;
      break;
  }

  PRINTV("bind socket\n")
  int bind_ret = bind(socket_fd, &addr->addr, sizeof(sockaddr_t));
  if (bind_ret == -1) {
    perror("couldn't bind socket");
    return -1;
  }

  if (*protocol == TCP) {
    PRINTV("listen\n")
    int list_ret = listen(socket_fd, 1);
    if (list_ret == -1) {
      perror("listen failed");
      return -1;
    }
    PRINTV("accept\n")
    socklen_t len = sizeof(struct sockaddr_in);
    socket_fd = accept(socket_fd, NULL, &len);
    if (socket_fd < 0) {
      perror("accepting failed");
    }
  }
  return socket_fd;
}

int prepare_socket(sockaddr_t *addr, const protocol_t *protocol,
                   bool is_listener) {
  int type;
  switch (*protocol) {
    case UDP:
      PRINTV("create UDP socket\n")
      type = SOCK_DGRAM;
      break;
    case TCP:
      PRINTV("create TCP socket\n")
      type = SOCK_STREAM;
      break;
    default:
      return -1;
  }

  int socket_fd = socket(addr->in.sin_family, type, 0);
  if (socket_fd < 0) {
    perror("could not open socket");
  }

  if (is_listener == true) {
    socket_fd = prepare_listener(addr, protocol, socket_fd);
  } else {
    socket_fd = prepare_sender(addr, protocol, socket_fd);
  }

  return socket_fd;
}