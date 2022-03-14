#ifndef NETCOPY_TEST_OUT_H
#define NETCOPY_TEST_OUT_H

#include <stdio.h>
#include <stdbool.h>

//print assert
#define PASSERT(statement, str)                 \
do{                                             \
    bool pass = (bool)(statement);              \
    if(!pass)                                   \
    {                                           \
        fprintf(stderr,(const char*)str);       \
        fprintf(stderr,"\n");assert(pass);      \
    }                                           \
}while(0);

//print error assert
#define PEASSERT(statement, str)                \
do{                                             \
    bool pass = (bool)(statement);              \
    if(!pass)                                   \
    {                                           \
        perror((const char*)str);               \
    }                                           \
}while(0);

#endif //NETCOPY_TEST_OUT_H
