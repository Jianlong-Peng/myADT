/*=============================================================================
#     FileName: marray.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 12:24:06
#   LastChange: 2014-11-10 15:06:40
#      History:
=============================================================================*/
#ifndef  MY_ARRAY_H_INC
#define  MY_ARRAY_H_INC

#include "mconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Array;
typedef struct Array* ArrayPtr;


ArrayPtr array_new(void *(*copy_func)(const void*), void (*free_func)(void *));

ArrayPtr array_new_n(int n, void *(*copy_func)(const void*), void (*free_func)(void *));

void array_clear(ArrayPtr p);

void array_free(ArrayPtr *p);

int array_size(const ArrayPtr p);
int array_capacity(const ArrayPtr p);
BOOL array_empty(const ArrayPtr p);

// i can be negative integer
void *array_get(ArrayPtr p, int i);
void *array_front(ArrayPtr p);
void *array_back(ArrayPtr p);

// array[i] = d
// i can be negative integer
void array_set(ArrayPtr p, void *d, int i);
#define ARRAY_SET(type, p, data, i) \
do { \
    type __src = data; \
    array_set(p, &__src, i); \
} while(0)

void array_resize(ArrayPtr p, int n);

void array_push_back(ArrayPtr p, void *d);

void array_pop_back(ArrayPtr p);

// i and j can be negative integer
void array_swap(ArrayPtr p, int i, int j);

void array_sort(ArrayPtr p, int (*_cmp)(const void*, const void*));

// display array as complete binary tree to .dot format
void array_to_complete_tree(ArrayPtr p, FILE *outf, void (*display_data)(FILE*, const void*));

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_ARRAY_H_INC  ----- */

