/*=============================================================================
#     FileName: marray.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1 
#      Created: 2014-07-25 12:37:01
#   LastChange: 2014-11-10 16:22:54
#      History: 
============================================================================= */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "memory.h"
#include "marray.h"
#include "mtools.h"

struct Array {
    int _size;
    int _capacity;
    void **_data;
    void *(*_copy)(const void *);
    void (*_free)(void *);
};


ArrayPtr array_new(void *(*copy_func)(const void*), void (*free_func)(void *))
{
    return array_new_n(10, copy_func, free_func);
}

ArrayPtr array_new_n(int n, void *(*copy_func)(const void*), void (*free_func)(void *))
{
    ArrayPtr p;
    NEW(p);
    NEWC(p->_data, n);
    p->_size = 0;
    p->_capacity = n;
    p->_copy = (copy_func==NULL)?adt_copy_nothing:copy_func;
    p->_free = (free_func==NULL)?adt_free_nothing:free_func;
    return p;
}

void array_clear(ArrayPtr p)
{
    if(p && p->_size) {
        int i;
        for(i=0; i<p->_size; ++i) {
            p->_free(p->_data[i]);
            p->_data[i] = NULL;
        }
        p->_size = 0;
    }
}

void array_free(ArrayPtr *p)
{
    if(p && *p) {
        array_clear(*p);
        FREE((*p)->_data);
        FREE(*p);
    }
}

int array_size(const ArrayPtr p)
{
    CHECK_NULL_POINTER(p, "array_size");
    return p->_size;
}

int array_capacity(const ArrayPtr p)
{
    CHECK_NULL_POINTER(p, "array_capacity");
    return p->_capacity;
}

BOOL array_empty(const ArrayPtr p)
{
    CHECK_NULL_POINTER(p, "array_empty");
    return (p->_size==0)?TRUE:FALSE;
}

void *array_get(ArrayPtr p, int i)
{
    int index = (i<0)?(p->_size+i):i;
    CHECK_NULL_POINTER(p, "array_get");
    CHECK_INDEX_OUT_OF_RANGE((index<0 || index>=p->_size), "array_get");
    return p->_data[index];
}

void *array_front(ArrayPtr p)
{
    CHECK_NULL_POINTER(p, "array_front");
    return (p->_size==0)?NULL:(p->_data[0]);
}
void *array_back(ArrayPtr p)
{
    CHECK_NULL_POINTER(p, "array_back");
    return (p->_size==0)?NULL:(p->_data[p->_size-1]);
}

void array_set(ArrayPtr p, void *d, int i)
{
    int index = (i<0)?(p->_size+i):i;
    CHECK_NULL_POINTER(p, "array_set");
    CHECK_INDEX_OUT_OF_RANGE((index<0 || index>=p->_size), "array_set");
    p->_data[index] = p->_copy(d);
}

void array_resize(ArrayPtr p, int n)
{
    CHECK_NULL_POINTER(p, "array_resize");
#if defined(DEBUG)
    printf("(DEBUG information from array_resize: %d --> %d)\n", p->_capacity, n);
#endif
    RESIZE(p->_data, sizeof(void*)*n);
    p->_capacity = n;
}

void array_push_back(ArrayPtr p, void *d)
{
    CHECK_NULL_POINTER(p, "array_push_back");
    if(p->_size == p->_capacity)
        array_resize(p, p->_capacity*2);
    p->_data[p->_size] = p->_copy(d);
    p->_size++;
}

void array_pop_back(ArrayPtr p)
{
    CHECK_NULL_POINTER(p, "array_pop_back");

    if(p->_size) {
        p->_size--;
        p->_free(p->_data[p->_size]);
        p->_data[p->_size] = NULL;
    }
}

void array_swap(ArrayPtr p, int i, int j)
{
    void *tmp;
    int index1 = (i<0)?(p->_size+i):i;
    int index2 = (j<0)?(p->_size+j):j;
    CHECK_NULL_POINTER(p, "array_swap");
    CHECK_INDEX_OUT_OF_RANGE((index1<0 || index1>=p->_size), "array_swap");
    CHECK_INDEX_OUT_OF_RANGE((index2<0 || index2>=p->_size), "array_swap");
    tmp = p->_data[index1];
    p->_data[index1] = p->_data[index2];
    p->_data[index2] = tmp;
}

static void array_qsort(void **data, int n, int (*_cmp)(const void*, const void*))
{
    int pivot, i, j;

    if(n == 2) {
        if(_cmp(data[1], data[0]) < 0)
            ADT_SWAP(data[0], data[1]);
    }
    else {
        // let median{data[0], data[n-1], data[n/2]} as pivot
        int lo=0, hi=n-1, mid=(lo+hi)/2;
        if(_cmp(data[lo], data[mid]) < 0)
            ADT_SWAP(data[lo], data[mid]);
        if(_cmp(data[hi], data[lo]) < 0) {
            ADT_SWAP(data[hi], data[lo]);
            if(_cmp(data[lo], data[mid]) < 0)
                ADT_SWAP(data[lo], data[mid]);
        }
        i=0; j=n; pivot=0;
        do {
            for(i+=1; i<n && _cmp(data[i], data[pivot])<0; ++i)
                ;
            for(j-=1; j>0 && _cmp(data[pivot], data[j])<0; --j)
                ;
            if(i < j)
                ADT_SWAP(data[i], data[j]);
        } while(i < j);
        if(j > 0)
            ADT_SWAP(data[j], data[pivot]);
        array_qsort(data, j, _cmp);
        array_qsort(data+j+1, n-1-j, _cmp);
    }
}
void array_sort(ArrayPtr p, int (*_cmp)(const void*, const void*))
{
    CHECK_NULL_POINTER(p, "array_sort");
    if(p->_size <= 1)
        return ;
    array_qsort(p->_data, p->_size, _cmp);
    //quick_sort(p->_data, p->_size, sizeof(void*), _cmp);
}

void array_to_complete_tree(ArrayPtr p, FILE *outf, void (*display_data)(FILE*, const void*))
{
    int i,level,count,end,parent;

    CHECK_NULL_POINTER(p, "array_to_complete_tree");

    fprintf(outf, "digraph G {\n"
            "node[shape=ellipse];\n");
    i = 0;
    level = 1;
    count = 1;
    while(i < p->_size) {
        end = (int)(pow(2, level))-1;
        end = (end < p->_size)?end:(p->_size);
        for(; i<end; ++i) {
            fprintf(outf, "N%d[label=\"", count);
            display_data(outf, p->_data[i]);
            fprintf(outf, "\"];\n");
            if(count > 1) {
                parent = count / 2;
                if(count&1)
                    fprintf(outf, "N%d:se->N%d;\n", parent, count);
                else
                    fprintf(outf, "N%d:sw->N%d;\n", parent, count);
            }
            ++count;
        }
        ++level;
    }
    fprintf(outf, "}\n");
}

