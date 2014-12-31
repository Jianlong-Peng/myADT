/*=============================================================================
#     FileName: mmap.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-30 16:41:14
#   LastChange: 2014-11-05 14:59:20
#      History: 
=============================================================================*/
#ifndef  MY_MAP_H_INC
#define  MY_MAP_H_INC

#include "mconfig.h"
#include "mtools.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Pair
{
    void *_key;
    void *_value;
};
typedef struct Pair* PairPtr;

struct Map;
typedef struct Map* MapPtr;

// cmp_func: make comparision between two 'PairPtr'
MapPtr map_new(void *(*_copy_key)(const void*), void (*_free_key)(void*), 
        void *(*_copy_value)(const void*), void (*_free_vlaue)(void*),
        int (*cmp_func)(const void*, const void*));
#define MAP_NEW(keytype, valuetype, cmp_func) \
    map_new(adt_copy_##keytype, adt_free, adt_copy_##valuetype, adt_free, cmp_func)

void map_clear(MapPtr m);

void map_free(MapPtr *m);

int map_size(const MapPtr m);

// insert pair(key,value) to 'm'
// if key already exists, a warning will be displayed, and
// nothing will be done to the map
BOOL map_insert(MapPtr m, void *key, void *value);
#define MAP_INSERT(keytype, valuetype, m, key, value) \
do { \
    keytype dk = key; \
    valuetype dv = value; \
    map_insert(m, &dk, &dv); \
} while(0)

// m[key] = value
// if key not exists, a warning will be displayed, and
// the pair(key,value) will be inserted.
void map_set(MapPtr m, void *key, void *value);
#define MAP_SET(keytype, valuetype, m, key, value) \
do { \
    keytype dk = key; \
    valuetype dv = value; \
    map_set(m, &dk, &dv); \
} while(0)

// m[key]
// can be used to test if 'm' has 'key'
// return NULL if 'm' has not 'key'
PairPtr map_get(MapPtr m, void *key);

// delete the pair with 'key'
// if key not exists, a warning will be displayed, and
// nothing will be done to the map.
void map_delete(MapPtr m, void *key);

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_MAP_H_INC  ----- */

