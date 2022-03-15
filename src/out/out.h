#ifndef ISOCKCOM_OUT_H
#define ISOCKCOM_OUT_H

#include <stdbool.h>

extern bool is_verbose;

//print error
#define PRINTE(str)                                     \
        fprintf(stderr,(const char*)(str));             \
        fprintf(stderr,"\n");                           \

//print error info
#define PRINTEI(str, info_str)                          \
        fprintf(stderr,(const char*)(str));             \
        fprintf(stderr,"%s",(const char*)(info_str));   \
        fprintf(stderr,"\n");                           \

//print verbose
#define PRINTV(str)                                     \
do                                                      \
        {                                               \
        if(!(bool)(is_verbose)) break;                  \
        printf((const char*)(str));                     \
}while(0);

//print verbose
#define PRINTVI(str, info)                              \
do                                                      \
        {                                               \
        if(!(bool)(is_verbose)) break;                  \
        printf((const char*)(str),info);                \
}while(0);

#endif //ISOCKCOM_OUT_H
