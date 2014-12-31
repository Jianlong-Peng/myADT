/*=============================================================================
#     FileName: m234_tree.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-06 21:01:00
#   LastChange: 2014-11-05 14:53:08
#      History: 
#=============================================================================*/
#ifndef  MY_M234_TREE_H_INC
#define  MY_M234_TREE_H_INC

#include "mconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Two34Node;
typedef struct Two34Node* Two34NodePtr;

struct Two34Tree;
typedef struct Two34Tree* Two34TreePtr;

Two34TreePtr ttf_new(void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*));

Two34TreePtr ttf_copy(const Two34TreePtr t);

void ttf_clear(Two34TreePtr t);

void ttf_free(Two34TreePtr *t);

int ttf_size(const Two34TreePtr t);

// to be implemented
BOOL ttf_insert(Two34TreePtr t, void *src);

Two3NodePtr ttf_has(Two34TreePtr t, void *src);

// to be implemented
void ttf_delete(Two34TreePtr t, void *src);

void ttf_for_each(Two34TreePtr t, void (*func)(void **elem, void *args), void *args);


#ifdef __cplusplus
}
#endif


#endif   /* ----- #ifndef MY_M234_TREE_H_INC  ----- */

