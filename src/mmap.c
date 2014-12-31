/*=============================================================================
#     FileName: mmap.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-30 16:42:45
#   LastChange: 2014-11-05 14:32:57
#      History: 
=============================================================================*/
#include <stddef.h>
#include "memory.h"
#include "mmap.h"
//#include "mavl_tree.h"
#include "mrb_tree.h"
#include "mtools.h"

struct Map
{
    void *(*_copy_key)(const void*);
    void (*_free_key)(void*);
    void *(*_copy_value)(const void*);
    void (*_free_value)(void*);
    int (*_cmp)(const void*, const void*);
    //AVLTreePtr _at;
    RBTreePtr _at;
};

MapPtr map_new(void *(*copy_key)(const void*), void (*free_key)(void*), 
        void *(*copy_value)(const void*), void (*free_value)(void*),
        int (*cmp_func)(const void*, const void*))
{
    MapPtr m;
    NEW(m);
    m->_copy_key = (copy_key==NULL)?adt_copy_nothing:copy_key;
    m->_free_key = (free_key==NULL)?adt_free_nothing:free_key;
    m->_copy_value = (copy_value==NULL)?adt_copy_nothing:copy_value;
    m->_free_value = (free_value==NULL)?adt_free_nothing:free_value;
    m->_cmp = cmp_func;
    //m->_at = avl_new(NULL, NULL, cmp_func);
    m->_at = rb_new(NULL, NULL, cmp_func);
    return m;
}

static void map_free_pair(void **elem, void *others)
{
    PairPtr *p = (PairPtr*)elem;
    MapPtr m = (MapPtr)others;
    if((*p)->_key)
        m->_free_key((*p)->_key);
    if((*p)->_value)
        m->_free_value((*p)->_value);
    FREE(*p);
}

void map_clear(MapPtr m)
{
    if(m) {
        rb_for_each(m->_at, map_free_pair, m);
        rb_clear(m->_at);
        //avl_for_each(m->_at, map_free_pair, m);
        //avl_clear(m->_at);
    }
}
void map_free(MapPtr *m)
{
    if(m && *m) {
        rb_for_each((*m)->_at, map_free_pair, m);
        rb_free(&(*m)->_at);
        //avl_for_each((*m)->_at, map_free_pair, m);
        //avl_free(&(*m)->_at);
        FREE(*m);
    }
}

int map_size(const MapPtr m)
{
    CHECK_NULL_POINTER(m, "map_size");
    return rb_size(m->_at);
    //return avl_size(m->_at);
}

BOOL map_insert(MapPtr m, void *key, void *value)
{
    PairPtr p;
    
    CHECK_NULL_POINTER(m, "map_insert");
    CHECK_NULL_POINTER(key, "map_insert");

    NEW(p);
    p->_key = m->_copy_key(key);
    if(value)
        p->_value = m->_copy_value(value);
    else
        p->_value = NULL;
    if(!rb_insert(m->_at, p)) {
        FREE(p);
        return FALSE;
    }
    else
        return TRUE;
}

void map_set(MapPtr m, void *key, void *value)
{
    // struct Pair p;
    PairPtr p;
    void *result;
    
    CHECK_NULL_POINTER(m, "map_set");
    CHECK_NULL_POINTER(key, "map_set");

    NEW(p);
    p->_key = m->_copy_key(key);
    if(value)
        p->_value = m->_copy_value(value);
    else
        p->_value = NULL;
    result = rb_has(m->_at, p);
    //p._key = key;
    //p._value = value;
    //result = rb_has(m->_at, &p);
    if(result == NULL) {
        DISPLAY_WARNING("the key appears not in the map yet, and try to insert the pair", "map_set");
        rb_insert(m->_at, p);
        //map_insert(m, key, value);
    }
    else {
        PairPtr p2 = (PairPtr)result;
        m->_free_value(p2->_value);
        p2->_value = m->_copy_value(value);
        m->_free_key(p->_key);
        if(p->_value)
            m->_free_value(p->_value);
        FREE(p);
    }
}

PairPtr map_get(MapPtr m, void *key)
{
    struct Pair p;
    void *result;
    
    CHECK_NULL_POINTER(m, "map_get");
    CHECK_NULL_POINTER(key, "map_get");
    p._key = key;
    result = rb_has(m->_at, &p);
    //result = avl_has(m->_at, &p);
    return (result==NULL)?NULL:((PairPtr)result);
}

// delete the pair with 'key'
void map_delete(MapPtr m, void *key)
{
    struct Pair p;
    void *result;
    
    CHECK_NULL_POINTER(m, "map_delete");
    CHECK_NULL_POINTER(key, "map_delete");
    p._key = key;
    result = rb_has(m->_at, &p);
    //result = avl_has(m->_at, &p);
    if(result == NULL)
        DISPLAY_WARNING("the key to be deleted appears not in the map", "map_delete");
    else {
        PairPtr p2 = (PairPtr)result;
        rb_delete(m->_at, &p);
        //avl_delete(m->_at, &p);
        if(p2->_key)
            m->_free_key(p2->_key);
        if(p2->_value)
            m->_free_value(p2->_value);
    }
}

