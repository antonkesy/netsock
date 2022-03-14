#ifndef ISOCKCOM_OUT_H
#define ISOCKCOM_OUT_H

//print error
#define PRINTE(str)                                     \
        fprintf(stderr,(const char*)str);               \
        fprintf(stderr,"\n");                           \

//print error info
#define PRINTEI(str, info_str)                          \
        fprintf(stderr,(const char*)str);               \
        fprintf(stderr,"%s",(const char*)info_str);     \
        fprintf(stderr,"\n");                           \

//print verbose
#define PRINTV(to_print, str)                           \
do                                                      \
        {                                               \
        if(!(bool)(to_print)) break;                    \
        printf((const char*)(str));                     \
}while(0);

//print verbose
#define PRINTVI(to_print, str, info)                    \
do                                                      \
        {                                               \
        if(!(bool)(to_print)) break;                    \
        printf((const char*)(str),info);                \
}while(0);

#endif //ISOCKCOM_OUT_H
