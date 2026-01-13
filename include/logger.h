#ifndef NETSOCK_LOGGER_H
#define NETSOCK_LOGGER_H

#include <stdarg.h>
#include <stdbool.h>

extern bool is_verbose;

void netsock_error(const char* fmt, ...);
void netsock_log(const char* fmt, ...);
void set_verbose();

#endif  // NETSOCK_LOGGER_H
