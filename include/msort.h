/*=============================================================================
#     FileName: msort.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Created: 2014-05-07 10:45:51
#   LastChange: 2014-08-04 12:05:06
#      History:
=============================================================================*/
#ifndef  MY_SORT_H
#define  MY_SORT_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * base : array to be sorted
 * n    : number of elements in base
 * width: sizeof(type)
 * cmp  : make a comparision between v1 and v2
 *        'cmp(v1,v2) < 0' means 'v1' has priority over 'v2'
 *        if (v1<v2 and cmp<0) or (v1>v2 and cmp>0), then sort ascendingly.
 *        Otherwise, sort descendingly.
 */
void insertion_sort(void *base, size_t n, size_t width, 
        int (*cmp)(const void *, const void *));

void quick_sort(void *base, size_t n, size_t width, 
        int (*cmp)(const void *, const void *));

void heap_sort(void *base, size_t n, size_t width, 
        int (*cmp)(const void *, const void *));

void merge_sort(void *base, size_t n, size_t width,
        int (*cmp)(const void*, const void*));

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_SORT_H  ----- */

