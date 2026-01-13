#ifndef NETSOCK_LOGGER_H
#define NETSOCK_LOGGER_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

bool is_verbose;

void netsock_error(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}

void netsock_log(const char* fmt, ...) {
  if (!is_verbose) return;
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

void set_verbose() { is_verbose = true; }

#endif  // NETSOCK_LOGGER_H
