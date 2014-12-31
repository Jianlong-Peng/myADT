/*=============================================================================
#     FileName: msort.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Created: 2014-05-07 10:51:05
#   LastChange: 2014-08-04 12:07:32
#      History:
=============================================================================*/
#include <stdlib.h>
#include <string.h>
#include "msort.h"

/*
 * it's bad to use 'memcpy' to copy elements whose type is 'struct' ??????
 */

/*
static void sort_swap(char *base, size_t width, size_t i, size_t j)
{
    char tmp;
    char *a = base+i*width;
    char *b = base+j*width;

    while(width--) {
        tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    }
}
#define SORT_SWAP(base, width, i, j) sort_swap(base, width, i, j)
*/
#define SORT_SWAP(base, width, i, j) \
do { \
    register char *_swap_a_ = (base)+(i)*(width); \
    register char *_swap_b_ = (base)+(j)*(width); \
    register size_t _swap_i_ = width; \
    while(_swap_i_--) { \
        char _swap_tmp_ = *_swap_a_; \
        *_swap_a_++ = *_swap_b_; \
        *_swap_b_++ = _swap_tmp_; \
    } \
} while(0)

/*
#define SORT_SWAP(base, width, i, j) \
{ \
    char *_swap_tmp_ = malloc(width); \
    memcpy(_swap_tmp_, (base)+(i)*(width), (width)); \
    memcpy((base)+(i)*(width), (base)+(j)*(width), (width)); \
    memcpy((base)+(j)*(width), _swap_tmp_, (width)); \
    free(_swap_tmp_); \
}
*/
void insertion_sort(void *base, size_t n, size_t width, 
        int (*cmp)(const void *, const void *))
{
    size_t i,j;
    char *temp = (char*)malloc(width);
    char *base1 = (char*)base;

    for(i=1; i<n; ++i) {
        memcpy(temp, base1+i*width, width);
        for(j=i-1; j>0 && cmp(temp, base1+j*width)<0; --j)
            memcpy(base1+(j+1)*width, base1+j*width, width);
        if(j==0 && cmp(temp, base1)<0) {
            memcpy(base1+width, base1, width);
            memcpy(base1, temp, width);
        }
        else
            memcpy(base1+(j+1)*width, temp, width);
    }
    free(temp);
}


// in average, O(nlog2(n))
void quick_sort(void *base, size_t n, size_t width, 
        int (*cmp)(const void *, const void *))
{
    size_t pivot = 0;
    size_t i=0, j=n;
    char *base1 = (char*)base;
    size_t lo,mid,hi;

    if(n <= 5)
        insertion_sort(base, n, width, cmp);
    else {
        mid = 0, hi = n-1, lo = (mid+hi)/2;
        if(cmp(base1+mid*width, base1+lo*width) < 0)
            SORT_SWAP(base1, width, mid, lo);
        if(cmp(base1+hi*width, base1+mid*width) < 0) {
            SORT_SWAP(base1, width, hi, mid);
            if(cmp(base1+mid*width, base1+lo*width) < 0)
                SORT_SWAP(base1, width, mid, lo);
        }
        //SORT_SWAP(base1, width, lo, mid);  // make pivot=0
        do {
            for(i+=1; i<n; ++i) {
                if(cmp(base1+pivot*width, base1+i*width) < 0)
                    break;
            }
            for(j-=1; j>0; --j) {
                if(cmp(base1+j*width, base1+pivot*width) < 0)
                    break;
            }
            if(i < j)
                SORT_SWAP(base1, width, i, j);
            //    sort_swap(base1, width, i, j);
        } while(i < j);
        if(j > 0)
            SORT_SWAP(base1, width, pivot, j);
            //sort_swap(base1, width, pivot, j);
        quick_sort(base1, j, width, cmp);
        quick_sort(base1+(j+1)*width, n-1-j, width, cmp);
    }
}


static void adjust_heap(void *base, size_t n, size_t width, size_t root,
        int (*cmp)(const void *, const void *))
{
    size_t parent, child;
    char *base1 = (char*)base;
    char *temp = malloc(width);

    parent = root;
    memcpy(temp, base1+parent*width, width);
    child = 2 * parent + 1;
    while(child < n) {
        if((child+1 < n) && (cmp(base1+child*width, base1+(child+1)*width) < 0))
            child += 1;
        if(cmp(temp, base1+child*width) < 0) {
            memcpy(base1+parent*width, base1+child*width, width);
            parent = child;
            child = 2 * parent + 1;
        }
        else
            break;
    }
    memcpy(base1+parent*width, temp, width);
    free(temp);
}

void heap_sort(void *base, size_t n, size_t width, 
        int (*cmp)(const void *, const void *))
{
    size_t i;
    char *base1 = (char*)base;

    // construct a max-heap (if sort ascendingly) inplace. O(n)
    for(i=(n-2)/2; i>0; --i)
        adjust_heap(base1, n, width, i, cmp);
    adjust_heap(base1, n, width, 0, cmp);
    // O(nlog2(n))
    for(i=n-1; i>0; --i) {
        SORT_SWAP(base1, width, 0, i);
        //sort_swap(base1, width, 0, i);
        adjust_heap(base1, i, width, 0, cmp);
    }
}


static void merge_each(void *base, void *sort, size_t i, size_t m, size_t n, size_t width,
        int (*cmp)(const void*, const void*))
{
    // base[i...m-1], base[m...n-1]  => sort[i...n-1]
    size_t j,k,l;
    char *base1 = (char*)base;
    char *sort1 = (char*)sort;
    j = i;
    k = m;
    l = i;
    while(j<m && k<n) {
        if(cmp(base1+j*width, base1+k*width) < 0) {
            memcpy(sort1+l*width, base1+j*width, width);
            ++j;
            ++l;
        }
        else {
            memcpy(sort1+l*width, base1+k*width, width);
            ++k;
            ++l;
        }
    }
    while(j < m) {
        memcpy(sort1+l*width, base1+j*width, width);
        ++j;
        ++l;
    }
    while(k < n) {
        memcpy(sort1+l*width, base1+k*width, width);
        ++k;
        ++l;
    }
}
static void merge(void *base, void *sort, size_t n, size_t width, size_t length,
        int (*cmp)(const void*, const void*))
{
    size_t i;
    char *base1 = (char*)base;
    char *sort1 = (char*)sort;

    for(i=0; n>=2*length && i<(n-2*length); i+=(2*length))
        merge_each(base1, sort1, i, i+length, i+2*length, width, cmp);
    if(i+length >= n)
        merge_each(base1, sort1, i, n, n, width, cmp);
    else
        merge_each(base1, sort1, i, i+length, n, width, cmp);
}
void merge_sort(void *base, size_t n, size_t width,
        int (*cmp)(const void *, const void *))
{
    size_t length=1;
    char *temp = malloc(width*n);
    char *base1 = (char*)base;

    while(length <= n) {
        merge(base1, temp, n, width, length, cmp);
        length *= 2;
        merge(temp, base1, n, width, length, cmp);
        length *= 2;
    }

    free(temp);
}

#undef SORT_SWAP


