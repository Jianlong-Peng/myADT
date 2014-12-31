/*=============================================================================
#     FileName: mtools.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 15:41:54
#   LastChange: 2014-11-06 10:04:21
#      History: 
=============================================================================*/
#include <string.h>
#include "mtools.h"
#include "memory.h"


#define ADT_COPY_ELEMENT(type, data1) \
{ \
    type *data2; \
    NEW(data2); \
    memcpy(data2, (type*)data1, sizeof(type)); \
    return (void*)data2; \
}

void *adt_copy_char(const void *data)
ADT_COPY_ELEMENT(char, data)

void *adt_copy_int(const void *data)
ADT_COPY_ELEMENT(int, data)

void *adt_copy_float(const void *data)
ADT_COPY_ELEMENT(float, data)

void *adt_copy_double(const void *data)
ADT_COPY_ELEMENT(double, data)

void *adt_copy_long(const void *data)
ADT_COPY_ELEMENT(long, data)


void adt_free(void *data)
{
    FREE(data);
}

void *adt_copy_nothing(const void *src)
{
	return (void*)src;
}

void  adt_free_nothing(void *src) {}

