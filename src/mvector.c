/*=============================================================================
#     FileName: mvector.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-03 14:21:24
#   LastChange: 2014-11-10 16:32:29
#      History: 
=============================================================================*/
#include <stddef.h>
#include "mvector.h"
#include "memory.h"
#include "mtools.h"

struct Vector {
    int _size;
    int _capacity;
    int _first;
    int _last;
    void **_data;
    void *(*_copy)(const void *);
    void (*_free)(void *);
};

VectorPtr vector_new(void *(*copy_func)(const void*), void (*free_func)(void*))
{
    return vector_new_n(10, copy_func, free_func);
}

VectorPtr vector_new_n(int n, void *(*copy_func)(const void*), void (*free_func)(void*))
{
    VectorPtr v;
    NEW(v);
    NEWC(v->_data, n);
    v->_capacity = n;
    v->_size = 0;
    v->_first = v->_last = -1;
    v->_copy = (copy_func==NULL)?adt_copy_nothing:copy_func;
    v->_free = (free_func==NULL)?adt_free_nothing:free_func;
    return v;
}

VectorPtr vector_copy(const VectorPtr v)
{
    int i;
    VectorPtr v2;
    
    CHECK_NULL_POINTER(v, "vector_copy");
    v2 = vector_new_n(v->_capacity, v->_copy, v->_free);
    v2->_first = v->_first;
    v2->_last = v->_last;
    v2->_size = v->_size;
    if(v2->_first != -1) {
        for(i=v2->_first; i!=v2->_last; ) {
            v2->_data[i] = v2->_copy(v->_data[i]);
            i = (i+1) % (v2->_capacity);
        }
        v2->_data[i] = v2->_copy(v->_data[i]);
    }
    return v2;
    
}

void vector_clear(VectorPtr v)
{
    if(v && v->_size) {
        int i;
        for(i=v->_first; i!=v->_last; ) {
            v->_free(v->_data[i]);
            v->_data[i] = NULL;
            i = (i+1) % (v->_capacity);
        }
        v->_free(v->_data[i]);
        v->_data[i] = NULL;
        v->_first = v->_last = -1;
        v->_size = 0;
    }    
}

void vector_free(VectorPtr *v)
{
    if(v && *v) {
        vector_clear(*v);
        FREE(*v);
    }
}

int vector_size(const VectorPtr v)
{
    CHECK_NULL_POINTER(v, "stack_copy");
    return v->_size;
}

BOOL vector_empty(const VectorPtr v)
{
    CHECK_NULL_POINTER(v, "vector_empty");
    return (v->_size==0)?TRUE:FALSE;
}

BOOL vector_full(const VectorPtr v)
{
    CHECK_NULL_POINTER(v, "vector_full");
    return (v->_size==v->_capacity)?TRUE:FALSE;
}

void vector_resize(VectorPtr v, int n)
{
    int i, delta;
    
    CHECK_NULL_POINTER(v, "vector_resize");
    if(v->_capacity >= n)
        return ;
#if defined(DEBUG)
    printf("(DEBUG information from vector_resize: %d --> %d)\n", v->_capacity, n);
#endif
    RESIZE(v->_data, sizeof(void*)*n);
    if(v->_first > v->_last) {
        delta = n - v->_capacity;
        for(i=v->_capacity-1; i>=v->_first; --i)
            v->_data[i+delta] = v->_data[i];
        v->_first += delta;
    }
    v->_capacity = n;
}

void vector_push_front(VectorPtr v, void *src)
{
    CHECK_NULL_POINTER(v, "vector_push_front");
    if(vector_empty(v))
        v->_first = v->_last = 0;
    else {
        if(vector_full(v))
            vector_resize(v, v->_capacity*2);
        v->_first = (v->_first-1)%(v->_capacity);
    }
    v->_data[v->_first] = v->_copy(src);
    v->_size++;

}
void vector_push_back(VectorPtr v, void *src)
{
    CHECK_NULL_POINTER(v, "vector_push_back");
    if(vector_empty(v))
        v->_first = v->_last = 0;
    else{
        if(vector_full(v))
            vector_resize(v, v->_capacity*2);
        v->_last = (v->_last+1)%(v->_capacity);        
    }
    v->_data[v->_last] = v->_copy(src);
    v->_size++;
    
}

void vector_pop_front(VectorPtr v)
{
    CHECK_NULL_POINTER(v, "vector_pop_front");
    if(v->_size) {
        v->_free(v->_data[v->_first]);
        v->_data[v->_first] = NULL;
        if(--(v->_size))
            v->_first = (v->_first+1)%(v->_capacity);
        else
            v->_first = v->_last = -1;
    }
}

void vector_pop_back(VectorPtr v)
{
    CHECK_NULL_POINTER(v, "vector_pop_back");
    if(v->_size) {
        v->_free(v->_data[v->_last]);
        v->_data[v->_last] = NULL;
        if(--(v->_size))
            v->_last = (v->_last-1)%(v->_capacity);
        else
            v->_first = v->_last = -1;    
    }
}

void *vector_front(VectorPtr v)
{
    CHECK_NULL_POINTER(v, "vector_front");
    return (v->_size==0)?NULL:(v->_data[v->_first]);
}

void *vector_back(VectorPtr v)
{
    CHECK_NULL_POINTER(v, "vector_back");
    return (v->_size==0)?NULL:(v->_data[v->_last]);
}

void *vector_get(VectorPtr v, int i)
{
    int index = (i<0)?(v->_size+i):i;
    CHECK_NULL_POINTER(v, "vector_get");
    CHECK_INDEX_OUT_OF_RANGE((index<0 || index>=v->_size), "vector_get");
    index = (v->_first + index)%(v->_capacity);
    return v->_data[index];
}

void vector_set(VectorPtr v, void *src, int i)
{
    int index = (i<0)?(v->_size+i):i;
    CHECK_NULL_POINTER(v, "vector_set");
    CHECK_INDEX_OUT_OF_RANGE((index<0 || index>=v->_size), "vector_set");
    index = (v->_first + index)%(v->_capacity);
    v->_free(v->_data[index]);
    v->_data[index] = v->_copy(src);
}

void vector_swap(VectorPtr v, int i, int j)
{
    int index1 = (i<0)?(v->_size+i):i;
    int index2 = (j<0)?(v->_size+j):j;
    void *temp;
    
    CHECK_NULL_POINTER(v, "vector_swap");
    CHECK_INDEX_OUT_OF_RANGE((index1<0 || index1>=v->_size), "vector_swap");
    CHECK_INDEX_OUT_OF_RANGE((index1<0 || index2>=v->_size), "vector_swap");
    index1 = (v->_first + index1)%(v->_capacity);
    index2 = (v->_last + index2)%(v->_capacity);
    temp = v->_data[index1];
    v->_data[index1] = v->_data[index2];
    v->_data[index2] = temp;
}

// pay attention to the indices !!!!!!!!!!!!!!!!!!!!!!!
// BUG here.
static void vector_qsort(void **data, int first, int last, int capacity,
        int (*_cmp)(const void*, const void*))
{
    int i,j,pivot;
    int n;
    BOOL goon;

    n = (last>=first)?(last-first+1):(capacity-first+last+1);
    if(n <= 2) {
        if(_cmp(data[last], data[first]) < 0)
            ADT_SWAP(data[first], data[last]);
    }
    else {
        pivot = first;
        i=first; j=last+1;
        goon = FALSE;
        do {
            for(i=(i+1)%capacity; i!=last && _cmp(data[i], data[pivot])<0; i=(i+1)%capacity)
                ;
            if(i==last && _cmp(data[i], data[pivot])<0)
                i = (i+1)%capacity;
            for(j=(j-1)%capacity; j!=first && _cmp(data[pivot], data[j])<0; j=(j-1)%capacity)
                ;
            if(j==first && _cmp(data[pivot], data[j])<0)
                j = (j-1)%capacity;

            if(first<=last)
                goon = (i<j)?TRUE:FALSE;
            else {
                int tmpi=i, tmpj=j;
                if(tmpi < last)
                    tmpi += capacity;
                if(tmpj < last)
                    tmpj += capacity;
                goon = (tmpi<tmpj)?TRUE:FALSE;
            }
            if(goon)
                ADT_SWAP(data[i], data[j]);
        } while(goon);
        if((j+1)%capacity != first)
            ADT_SWAP(data[j], data[pivot]);
        vector_qsort(data, first, (j-1)%capacity, capacity, _cmp);
        vector_qsort(data, (j+1)%capacity, last, capacity, _cmp);
    }
}
void vector_sort(VectorPtr v, int (*_cmp)(const void*, const void*))
{
    CHECK_NULL_POINTER(v, "vector_sort");
    if(v->_size <= 1)
        return ;
    vector_qsort(v->_data, v->_first, v->_last, v->_capacity, _cmp);
}



