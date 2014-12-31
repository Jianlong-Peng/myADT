/*=============================================================================
#     FileName: memory.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 13:04:15
#   LastChange: 2014-08-04 12:04:36
#      History:  
=============================================================================*/
#ifndef  MY_MEMORY_H_INC
#define  MY_MEMORY_H_INC

#ifdef __cplusplus
extern "C" {
#endif

void *memory_alloc(long nbytes, const char *file, int line);
#define ALLOC(nbytes) memory_alloc(nbytes, __FILE__, __LINE__)
#define NEW(p) ((p) = ALLOC((long)sizeof *(p)))

void *memory_calloc(long count, long nbytes, const char *file, int line);
#define CALLOC(count, nbytes) memory_calloc((count), (nbytes), __FILE__, __LINE__)
#define NEWC(p, count) ((p) = CALLOC((count), (long)sizeof *(p)))

void *memory_resize(void *ptr, long nbytes, const char *file, int line);
#define RESIZE(p, nbytes) ((p) = memory_resize(p, (nbytes), __FILE__, __LINE__))

void memory_free(void *ptr, const char *file, int line);
#define FREE(p) ((void)memory_free((p), __FILE__, __LINE__), (p)=0)

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_MEMORY_H_INC  ----- */

