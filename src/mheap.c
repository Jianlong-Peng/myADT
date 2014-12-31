/*=============================================================================
#     FileName: mheap.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 18:50:03
#   LastChange: 2014-11-10 16:42:23
#      History:
=============================================================================*/
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "mheap.h"
#include "marray.h"
#include "memory.h"
#include "mtools.h"


struct Heap
{
    ArrayPtr _ap;
    int (*_cmp)(const void*, const void*);
};


HeapPtr heap_new(void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*))
{
    return heap_new_n(10, copy_func, free_func, cmp_func);
}

HeapPtr heap_new_n(int n, void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*))
{
    HeapPtr hp;
    NEW(hp);
    hp->_ap = array_new_n(n, copy_func, free_func);
    hp->_cmp = cmp_func;
    return hp;
}

void heap_clear(HeapPtr hp)
{
    CHECK_NULL_POINTER(hp, "heap_clear");
    array_clear(hp->_ap);
}

void heap_free(HeapPtr *hp)
{
    if(hp) {
        array_free(&(*hp)->_ap);
        FREE(*hp);
    }
}

int heap_size(const HeapPtr hp)
{
    CHECK_NULL_POINTER(hp, "heap_size");
    return array_size(hp->_ap);
}

BOOL heap_empty(const HeapPtr hp)
{
    CHECK_NULL_POINTER(hp, "heap_empty");
    return array_empty(hp->_ap);
}

void heap_insert(HeapPtr hp, void *src)
{
    int c,p;
    CHECK_NULL_POINTER(hp, "heap_insert");
    array_push_back(hp->_ap, src);
    //bottom->up
    c = array_size(hp->_ap) - 1;
    while(c > 0) {
        p = (c-1) / 2;
        if(hp->_cmp(array_get(hp->_ap, c), array_get(hp->_ap, p)) < 0) {
            array_swap(hp->_ap, c, p);
            c = p;
        }
        else
            break;
    }
}

void *heap_top(HeapPtr hp)
{
    CHECK_NULL_POINTER(hp, "heap_top");
    CHECK_EMPTY(array_empty(hp->_ap), "heap_top");
    //return array_get(hp->_ap, 0);
    return (array_empty(hp->_ap))?NULL:array_get(hp->_ap,0);
}

void heap_pop(HeapPtr hp)
{
    int p,c,size;
    CHECK_NULL_POINTER(hp, "heap_pop");
    size = array_size(hp->_ap);
    array_swap(hp->_ap, 0, size-1);
    array_pop_back(hp->_ap);
    --size;
    //top->bottom
    p = 0;
    c = 2*p+1;
    while(c < size) {
        if(c+1<size && hp->_cmp(array_get(hp->_ap, c+1), array_get(hp->_ap, c))<0)
            c += 1;
        if(hp->_cmp(array_get(hp->_ap, c), array_get(hp->_ap, p)) < 0) {
            array_swap(hp->_ap, c, p);
            p = c;
            c = 2*p+1;
        }
        else
            break;
    }
}

void heap_display(HeapPtr hp, FILE *outf, void (*print_data)(FILE*, const void*))
{
    int level, level_max;
    int i;

    i = 0;
    level = 0;
    level_max = -1;
    while(i < array_size(hp->_ap)) {
        if(i > level_max) {
            ++level;
            level_max = (int)pow(2,level)-2;
            fprintf(outf, "\nlevel %-3d:  ", level);
        }
        fprintf(outf, " ");
        print_data(outf, array_get(hp->_ap, i));
        ++i;
    }
    fprintf(outf, "\n");
}

void heap_to_dot(HeapPtr hp, FILE *outf, void (*display_data)(FILE*, const void*))
{
    array_to_complete_tree(hp->_ap, outf, display_data);
}

BOOL heap_valid(const HeapPtr hp, void (*func)(const void*))
{
    int level;
    int max_level, n;
    int start, end, lc, rc;
    BOOL valid=TRUE;

    n = array_size(hp->_ap);
    max_level = (int)(log(n) / log(2)) + 1;
    start = 0;
    for(level=1; level<max_level; ++level) {
        end = (int)pow(2,level)-1;
        for(; start<end; ++start) {
            lc = 2*start+1;
            rc = 2*start+2;
            if(lc<n && hp->_cmp(array_get(hp->_ap,lc), array_get(hp->_ap,start))<0) {
                printf("_cmp( node(");
                func(array_get(hp->_ap, lc));
                printf("), node(");
                func(array_get(hp->_ap, start));
                printf(") ) < 0");
                valid = FALSE;
            }
            if(rc<n && hp->_cmp(array_get(hp->_ap, rc), array_get(hp->_ap,start))<0) {
                printf("_cmp( node(");
                func(array_get(hp->_ap, rc));
                printf("), node(");
                func(array_get(hp->_ap, start));
                printf(") ) < 0");
                valid = FALSE;
            }
        }
    }

    return valid;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct MinMaxHeap
{
    ArrayPtr _ap;
    int (*_cmp)(const void*, const void*);
};

MinMaxHeapPtr mheap_new(void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*))
{
    return mheap_new_n(10, copy_func, free_func, cmp_func);
}

MinMaxHeapPtr mheap_new_n(int n, void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*))
{
    MinMaxHeapPtr mp;
    NEW(mp);
    mp->_ap = array_new_n(n, copy_func, free_func);
    mp->_cmp = cmp_func;
    return mp;
}

void mheap_clear(MinMaxHeapPtr mp)
{
    CHECK_NULL_POINTER(mp, "mheap_clear");
    array_clear(mp->_ap);
}

void mheap_free(MinMaxHeapPtr *mp)
{
    if(mp) {
        array_free(&(*mp)->_ap);
        FREE(*mp);
    }
}

int mheap_size(const MinMaxHeapPtr mp)
{
    CHECK_NULL_POINTER(mp, "mheap_size");
    return array_size(mp->_ap);
}

BOOL mheap_empty(const MinMaxHeapPtr mp)
{
    CHECK_NULL_POINTER(mp, "mheap_empty");
    return array_empty(mp->_ap);
}

static BOOL in_min_level(int i)
{
    int level = (int)floor(log(i+1) / log(2));
    return (level&1)?FALSE:TRUE;
}
static BOOL in_max_level(int i)
{
    int level = (int)floor(log(i+1) / log(2));
    return (level&1)?TRUE:FALSE;
}

// bottom->up
static void mheap_verfiy_min(MinMaxHeapPtr mp, int c)
{
    int gp;
    gp = (c-3)/4;
    while(gp >= 0) {
        if(mp->_cmp(array_get(mp->_ap, c), array_get(mp->_ap, gp)) < 0) {
            array_swap(mp->_ap, c, gp);
            c = gp;
            gp = (c-3)/4;
        }
        else
            break;
    }
}
// bottom->up
static void mheap_verfiy_max(MinMaxHeapPtr mp, int c)
{
    int gp;
    gp = (c-3) / 4;
    while(gp > 0) {
        if(mp->_cmp(array_get(mp->_ap, c), array_get(mp->_ap, gp)) > 0) {
            array_swap(mp->_ap, c, gp);
            c = gp;
            gp = (c-3) / 4;
        }
        else
            break;
    }
}
void mheap_insert(MinMaxHeapPtr mp, void *src)
{
    int p,c;
    CHECK_NULL_POINTER(mp, "mheap_insert");
    array_push_back(mp->_ap, src);
    c = array_size(mp->_ap) - 1;
    if(c == 0)
        return ;
    if(in_min_level(c)) {
        p = (c-1) / 2;
        if(mp->_cmp(array_get(mp->_ap, p), array_get(mp->_ap, c)) < 0) {
            array_swap(mp->_ap, p, c);
            mheap_verfiy_max(mp, p);
        }
        else
            mheap_verfiy_min(mp, c);
    }
    else {
        p = (c-1) / 2;
        if(mp->_cmp(array_get(mp->_ap, p), array_get(mp->_ap, c)) > 0) {
            array_swap(mp->_ap, p, c);
            mheap_verfiy_min(mp, p);
        }
        else
            mheap_verfiy_max(mp, c);
    }
}

void *mheap_min(MinMaxHeapPtr mp)
{
    CHECK_NULL_POINTER(mp, "mheap_min");
    CHECK_EMPTY(array_empty(mp->_ap), "mheap_min");
    //return array_get(mp->_ap, 0);
    return (array_empty(mp->_ap))?NULL:array_get(mp->_ap,0);
}

void *mheap_max(MinMaxHeapPtr mp)
{
	int size;

    CHECK_NULL_POINTER(mp, "mheap_max");
    CHECK_EMPTY(array_empty(mp->_ap), "mheap_max");

    size = array_size(mp->_ap);
    if(size == 0)
        return NULL;
    else if(size == 1)
        return array_get(mp->_ap, 0);
    else if(size == 2)
        return array_get(mp->_ap, 1);
    else {
        void *data1 = array_get(mp->_ap, 1);
        void *data2 = array_get(mp->_ap, 2);
        return (mp->_cmp(data1, data2)>0)?data1:data2;
    }
}

// v < 0: to find the node with minimum value
// v > 0: to find the node with maximum value
static int mheap_child_grandchild(MinMaxHeapPtr mp, int i, int v)
{
    int indices[6];
    int size;
    int j,k;

    indices[0] = 2*(i+1) - 1;
    indices[1] = 2*(i+1);
    indices[2] = 2*(indices[0]+1) - 1;
    indices[3] = 2*(indices[0]+1);
    indices[4] = 2*(indices[1]+1) - 1;
    indices[5] = 2*(indices[1]+1);

    size = array_size(mp->_ap);
    // if i has no child
    if(indices[0] > size-1)
        return -1;
    j = 0;
    for(k=j+1; k<6 && indices[k]<size; ++k) {
        if(mp->_cmp(array_get(mp->_ap, k), array_get(mp->_ap, j)) * v > 0)
            j = k;
    }
    return j;
}

void mheap_pop_min(MinMaxHeapPtr mp)
{
    int size, i, m, p;
    CHECK_NULL_POINTER(mp, "mheap_pop_min");
    if(array_empty(mp->_ap))
        return ;
    size = array_size(mp->_ap);
    array_swap(mp->_ap, 0, size-1);
    i = 0;
    while(i < size-1) {
        m = mheap_child_grandchild(mp, i, -1);
        if(m == -1)  // has no more child
            break;
        if(mp->_cmp(array_get(mp->_ap, i), array_get(mp->_ap, m)) < 0)
            break;
        array_swap(mp->_ap, i, m);
        // if m is in max-level
        if(in_max_level(m))
            break;
        // if m is in min-level
        p = (m-1) / 2;
        if(mp->_cmp(array_get(mp->_ap, p), array_get(mp->_ap, m)) < 0)
            array_swap(mp->_ap, p, m);
        i = m;
    }
    array_pop_back(mp->_ap);
}

void mheap_pop_max(MinMaxHeapPtr mp)
{
    int size;
    int i,m,p;

    CHECK_NULL_POINTER(mp, "mheap_pop_max");

    size = array_size(mp->_ap);
    if(size <= 1)
        array_pop_back(mp->_ap);
    else {
        i = 1;
        if(size>=3 && mp->_cmp(array_get(mp->_ap,i),array_get(mp->_ap,i+1))<0)
            i += 1;
        array_swap(mp->_ap, i, size-1);
        while(i < size-1) {
            m = mheap_child_grandchild(mp, i, +1);
            if(m == -1)  // if i has no more child
                break;
            if(mp->_cmp(array_get(mp->_ap, i), array_get(mp->_ap, m)) > 0)
                break;
            array_swap(mp->_ap, i, m);
            // if m is in min-level
            if(in_min_level(m))
                break;
            // if m is in max-level
            p = (m-1) / 2;
            if(mp->_cmp(array_get(mp->_ap, p), array_get(mp->_ap, m)) > 0)
                array_swap(mp->_ap, p, m);
            i = m;
        }
        array_pop_back(mp->_ap);
    }
}

void mheap_to_dot(MinMaxHeapPtr mp, FILE *outf, void (*display_data)(FILE*, const void*))
{
    array_to_complete_tree(mp->_ap, outf, display_data);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct Deap
{
    ArrayPtr _ap;
    int (*_cmp)(const void*, const void*);
};

DeapPtr deap_new(void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*))
{
    return deap_new_n(10, copy_func, free_func, cmp_func);
}

DeapPtr deap_new_n(int n, void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*))
{
    DeapPtr dp;
    NEW(dp);
    dp->_ap = array_new_n(n, copy_func, free_func);
    dp->_cmp = cmp_func;
    return dp;
}

void deap_clear(DeapPtr dp)
{
    CHECK_NULL_POINTER(dp, "deap_clear");
    array_clear(dp->_ap);
}

void deap_free(DeapPtr *dp)
{
    if(dp) {
        array_free(&(*dp)->_ap);
        FREE(*dp);
    }
}

int deap_size(const DeapPtr dp)
{
    CHECK_NULL_POINTER(dp, "deap_size");
    return array_size(dp->_ap)-1;
}

BOOL deap_empty(const DeapPtr dp)
{
    CHECK_NULL_POINTER(dp, "deap_empty");
    return (deap_size(dp)==0)?TRUE:FALSE;
}

// where: indicates where the partner locates
// where == 1 : in max-subtree
// where == -1: in min-subtree
static int deap_partner(int i, int *where, int last)
{
    int level = (int)floor(log(i+1) / log(2));
    int total_num = (int)pow(2, level);
    int partner;

    // i in min-subtree
    if(i <= total_num/2) {
        *where = 1;
        partner = i + total_num/2;
        if(partner > last)
            partner = (partner-1) / 2;
    }
    // i in max-subtree
    else {
        *where = -1;
        partner = i - total_num/2;
    }

    return partner;
}

// bottom-up
static void deap_verfiy_min(DeapPtr dp, int c)
{
    int p;

    p = (c-1) / 2;
    while(p <= 1) {
        if(dp->_cmp(array_get(dp->_ap, c), array_get(dp->_ap, p)) < 0) {
            array_swap(dp->_ap, c, p);
            c = p;
            p = (c-1) / 2;
        }
        else
            break;
    }
}
// bottom-up
static void deap_verfiy_max(DeapPtr dp, int c)
{
    int p;

    p = (c-1) / 2;
    while(p <= 2) {
        if(dp->_cmp(array_get(dp->_ap, c), array_get(dp->_ap, p)) > 0) {
            array_swap(dp->_ap, c, p);
            c = p;
            p = (c-1) / 2;
        }
        else
            break;
    }
}

void deap_insert(DeapPtr dp, void *src)
{
    int i, m, where;
    
    CHECK_NULL_POINTER(dp, "deap_insert");
    if(array_empty(dp->_ap))
        array_push_back(dp->_ap, src);
    array_push_back(dp->_ap, src);

    i = array_size(dp->_ap) - 1;
    if(i == 1)
        return ;

    m = deap_partner(i, &where, i);
    // i is in min-subtree, m is in max-subtree
    if(where == 1) {
        if(dp->_cmp(array_get(dp->_ap, m), array_get(dp->_ap, i)) < 0) {
            array_swap(dp->_ap, i, m);
            deap_verfiy_max(dp, m);
        }
        else
            deap_verfiy_min(dp, i);
    }
    // i is in max-subtree, m is in min-subtree
    else {
        if(dp->_cmp(array_get(dp->_ap, i), array_get(dp->_ap, m)) < 0) {
            array_swap(dp->_ap, i, m);
            deap_verfiy_min(dp, m);
        }
        else
            deap_verfiy_max(dp, i);
    }
}


void *deap_min(DeapPtr dp)
{
    CHECK_NULL_POINTER(dp, "deap_min");
    CHECK_EMPTY(deap_empty(dp), "deap_min");
    return (deap_empty(dp))?NULL:array_get(dp->_ap,1);
}

void *deap_max(DeapPtr dp)
{
    int size;
    
    CHECK_NULL_POINTER(dp, "deap_max");
    CHECK_EMPTY(deap_empty(dp), "deap_max");
    size = deap_size(dp);
    switch(size) {
        case 0: return NULL;
        case 1: return array_get(dp->_ap, 1);
        default: return array_get(dp->_ap, 2); 
    }
}

void deap_pop_min(DeapPtr dp)
{
    int size;
    int c,p,which,m;
    
    CHECK_NULL_POINTER(dp, "deap_pop_min");
    if(deap_empty(dp))
        return ;
    size = deap_size(dp);
    if(size > 1)
        array_swap(dp->_ap, 1, size);
    array_pop_back(dp->_ap);
    if(size == 1)
        return ;
    p = 1;
    c = 2*p+1;
    while(c < size) {
        if(c+1<size && dp->_cmp(array_get(dp->_ap, c+1), array_get(dp->_ap, c))<0)
            c += 1;
        if(dp->_cmp(array_get(dp->_ap, c), array_get(dp->_ap, p)) < 0) {
            array_swap(dp->_ap, c, p);
            p = c;
            c = 2*p+1;
        }
        else
            break;
    }
    m = deap_partner(p, &which, size-1);
    if(dp->_cmp(array_get(dp->_ap,m), array_get(dp->_ap, p)) < 0) {
        array_swap(dp->_ap, m, p);
        deap_verfiy_max(dp, m);
    }
}

void deap_pop_max(DeapPtr dp)
{
    int size;
    int c,p,which,m;
    
    CHECK_NULL_POINTER(dp, "deap_pop_max");
    if(deap_empty(dp))
        return ;
    size = deap_size(dp);
    if(size > 2)
        array_swap(dp->_ap, 2, size);
    array_pop_back(dp->_ap);
    if(size==1 || size==2)
        return ;
    p = 2;
    c = 2*p+1;
    while(c < size) {
        if(c+1<size && dp->_cmp(array_get(dp->_ap, c+1), array_get(dp->_ap, c))>0)
            c += 1;
        if(dp->_cmp(array_get(dp->_ap, c), array_get(dp->_ap, p)) > 0) {
            array_swap(dp->_ap, c, p);
            p = c;
            c = 2*p+1;
        }
        else
            break;
    }
    m = deap_partner(p, &which, size-1);
    if(dp->_cmp(array_get(dp->_ap,m), array_get(dp->_ap, p)) > 0) {
        array_swap(dp->_ap, m, p);
        deap_verfiy_min(dp, m);
    }
}

void deap_to_dot(DeapPtr dp, FILE *outf, void (*display_data)(FILE*, const void*))
{
    array_to_complete_tree(dp->_ap, outf, display_data);
}

