#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define PTHLEN 256
#define BUFLEN 256


#define ofr(fp, path)\
    {\
        if((fp=fopen(path,"r"))==NULL)\
        {   fprintf(stderr,"Blad otwarcia pliku %s lub plik nie istnieje.",path);\
            return;\
        }\
    }

#define ofw(fp, path)\
    {\
        if((fp=fopen(path,"w"))==NULL)\
        {   fprintf(stderr,"Blad otwarcia pliku %s",path);\
            return;\
        }\
    }
