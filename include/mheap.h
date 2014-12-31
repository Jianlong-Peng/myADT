/*=============================================================================
#     FileName: mheap.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 18:49:00
#   LastChange: 2014-11-10 15:07:08
#      History:
=============================================================================*/
#ifndef  MY_HEAP_H_INC
#define  MY_HEAP_H_INC

#include <stdio.h>
#include "mconfig.h"
#include "marray.h"

/*
 * if cmp_func(v1, v2) < 0, then v1 has priority over v2
 */
 
#ifdef __cplusplus
extern "C" {
#endif



 // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 // min(max) heap
struct Heap;
typedef struct Heap* HeapPtr;

HeapPtr heap_new(void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*));

HeapPtr heap_new_n(int n, void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*));

void heap_clear(HeapPtr p);

void heap_free(HeapPtr *p);

int heap_size(const HeapPtr p);
BOOL heap_empty(const HeapPtr p);

void heap_insert(HeapPtr p, void *src);

void *heap_top(HeapPtr p);

void heap_pop(HeapPtr p);

void heap_display(HeapPtr p, FILE *outf, void (*print_data)(FILE*, const void*));

void heap_to_dot(HeapPtr p, FILE *outf, void (*display_data)(FILE*, const void*));

BOOL heap_valid(const HeapPtr hp, void (*func)(const void*));

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// min-max heap
struct MinMaxHeap;
typedef struct MinMaxHeap* MinMaxHeapPtr;

MinMaxHeapPtr mheap_new(void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*));

MinMaxHeapPtr mheap_new_n(int n, void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*));

void mheap_clear(MinMaxHeapPtr p);

void mheap_free(MinMaxHeapPtr *p);

int mheap_size(const MinMaxHeapPtr p);
BOOL mheap_empty(const MinMaxHeapPtr p);

void mheap_insert(MinMaxHeapPtr p, void *src);

void *mheap_min(MinMaxHeapPtr p);
void *mheap_max(MinMaxHeapPtr p);

void mheap_pop_min(MinMaxHeapPtr p);
void mheap_pop_max(MinMaxHeapPtr p);

void mheap_to_dot(MinMaxHeapPtr p, FILE *outf, void (*display_data)(FILE*, const void*));

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// double-ended heap
struct Deap;
typedef struct Deap* DeapPtr;

DeapPtr deap_new(void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*));

DeapPtr deap_new_n(int n, void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*));

void deap_clear(DeapPtr p);

void deap_free(DeapPtr *p);

int deap_size(const DeapPtr p);
BOOL deap_empty(const DeapPtr p);

void deap_insert(DeapPtr p, void *src);

void *deap_min(DeapPtr p);
void *deap_max(DeapPtr p);

void deap_pop_min(DeapPtr p);
void deap_pop_max(DeapPtr p);

void deap_to_dot(DeapPtr p, FILE *outf, void (*display_data)(FILE*, const void*));

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_HEAP_H_INC  ----- */

