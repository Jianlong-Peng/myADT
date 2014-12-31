/*=============================================================================
#     FileName: memory.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 13:12:15
#   LastChange: 2014-08-06 21:35:39
#      History: 
=============================================================================*/
#include "memory.h"
#include "mtools.h"
#include <stdlib.h>
#include <stdio.h>

#define CHECK_POSITIVE(n, func_name) \
    do { \
        if((n) <= 0) { \
            printf("Error: try to allocate negative-sized memory block!\n"); \
            printf("  raised in %s at %s:%d\n", func_name, file, line); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)


#define CHECK_MEM_FAILED(ptr, func_name) \
    do { \
        if((ptr) == NULL) { \
            printf("Error: failed to allocate memory!\n"); \
            printf("  raised in %s at %s:%d\n", func_name, file, line); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)


void *memory_alloc(long nbytes, const char *file, int line)
{
    void *ptr;
    CHECK_POSITIVE(nbytes, "memory_alloc");
    ptr = malloc(nbytes);
    CHECK_MEM_FAILED(ptr, "memory_alloc");
    return ptr;
}

void *memory_calloc(long count, long nbytes, const char *file, int line)
{
    void *ptr;
    CHECK_POSITIVE(count, "memory_calloc");
    CHECK_POSITIVE(nbytes, "memory_calloc");
    ptr = calloc(count, nbytes);
    CHECK_MEM_FAILED(ptr, "memory_calloc");
    return ptr;
}

void *memory_resize(void *ptr, long nbytes, const char *file, int line)
{
    CHECK_NULL_POINTER(ptr, "memory_resize");
    CHECK_POSITIVE(nbytes, "memory_resize");
    ptr = realloc(ptr, nbytes);
    CHECK_MEM_FAILED(ptr, "memory_resize");
    return ptr;
}

void memory_free(void *ptr, const char *file, int line)
{
    if(ptr)
        free(ptr);
}

