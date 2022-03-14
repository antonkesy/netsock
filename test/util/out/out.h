#ifndef NETCOPY_OUT_H
#define NETCOPY_OUT_H

#include <stdio.h>
#include <stdbool.h>

//print assert
#define PASSERT(statement, print)               \
do{                                             \
    bool pass = (bool)(statement);              \
    if(!pass)                                   \
    {                                           \
        fprintf(stderr,(const char*)print);     \
        fprintf(stderr,"\n");assert(pass);      \
    }                                           \
}while(0);

//print error assert
#define PEASSERT(statement, print)              \
do{                                             \
    bool pass = (bool)(statement);              \
    if(!pass)                                   \
    {                                           \
        perror((const char*)print);             \
    }                                           \
}while(0);

#endif //NETCOPY_OUT_H
