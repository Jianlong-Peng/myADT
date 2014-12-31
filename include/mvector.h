/*=============================================================================
#     FileName: mvector.h
#         Desc: implemented using double-ended queue like structure.
#               being easy to expand at both begin and end of vector.
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-03 14:16:25
#   LastChange: 2014-08-04 12:05:33
#      History: 
=============================================================================*/
#ifndef  MY_VECTOR_H_INC
#define  MY_VECTOR_H_INC

#include "mconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Vector;
typedef struct Vector* VectorPtr;

VectorPtr vector_new(void *(*copy_func)(const void*), void (*free_func)(void*));

VectorPtr vector_new_n(int n, void *(*copy_func)(const void*), void (*free_func)(void*));

VectorPtr vector_copy(const VectorPtr v);

void vector_clear(VectorPtr v);

void vector_free(VectorPtr *v);

int vector_size(const VectorPtr v);
BOOL vector_empty(const VectorPtr v);
BOOL vector_full(const VectorPtr v);

// do nothing if n is less than or equal to the capacity of vector
void vector_resize(VectorPtr v, int n);

void vector_push_front(VectorPtr v, void *src);
void vector_push_back(VectorPtr v, void *src);

void vector_pop_front(VectorPtr v);
void vector_pop_back(VectorPtr v);

void *vector_front(VectorPtr v);
void *vector_back(VectorPtr v);

// i can be negative integer.
void *vector_get(VectorPtr v, int i);

void vector_set(VectorPtr v, void *src, int i);
#define VECTOR_SET(type, v, data, i) \
do { \
    type src = data; \
    vector_set(v, &src, i); \
} while(0)

// i and j can be negative integer.
void vector_swap(VectorPtr v, int i, int j);

// not fully implemented!!
void vector_sort(VectorPtr v, int (*_cmp)(const void*, const void*));

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_VECTOR_H_INC  ----- */

