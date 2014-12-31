/*=============================================================================
#     FileName: m23_tree.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-05 15:08:07
#   LastChange: 2014-11-05 14:53:43
#      History: 
#=============================================================================*/
#ifndef  MY_23_TREE_H_INC
#define  MY_23_TREE_H_INC

#include "mconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

ADT_API struct Two3Node;
typedef struct Two3Node* Two3NodePtr;

struct Two3Tree;
typedef struct Two3Tree* Two3TreePtr;

Two3TreePtr tt_new(void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*));

Two3TreePtr tt_copy(const Two3TreePtr t);

void tt_clear(Two3TreePtr t);

void tt_free(Two3TreePtr *t);

int tt_size(const Two3TreePtr t);

BOOL tt_insert(Two3TreePtr t, void *src);

Two3NodePtr tt_has(Two3TreePtr t, void *src);

void tt_delete(Two3TreePtr t, void *src);

void tt_for_each(Two3TreePtr t, void (*func)(void **elem, void *args), void *args);

Two3NodePtr tt_root(Two3TreePtr t);
Two3NodePtr tt_left(Two3NodePtr node);
Two3NodePtr tt_middle(Two3NodePtr node);
Two3NodePtr tt_right(Two3NodePtr node);
void *tt_left_elem(Two3NodePtr node);
void *tt_right_elem(Two3NodePtr node);

// to check if 23-tree is valid
// `func`: to display data element of invalid node(s) without '\n'!!!!
BOOL tt_valid(const Two3TreePtr t, void (*func)(const void*));

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_23_TREE_H_INC  ----- */

