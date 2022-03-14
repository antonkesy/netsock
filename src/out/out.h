#ifndef NETCOPY_OUT_H
#define NETCOPY_OUT_H

//print error
#define PRINTE(str)                                     \
        fprintf(stderr,(const char*)str);               \
        fprintf(stderr,"\n");                           \

//print error info
#define PRINTEI(str, info_str)                          \
        fprintf(stderr,(const char*)str);               \
        fprintf(stderr,"%s",(const char*)info_str);     \
        fprintf(stderr,"\n");                           \

#endif //NETCOPY_OUT_H
