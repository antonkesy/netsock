#ifndef NETCOPY_H
#define NETCOPY_H

enum Protocol {
    UDP, TCP
};

typedef enum Protocol protocol_t;

int netcopy(int argc, char *argv[]);

#endif //NETCOPY_H
