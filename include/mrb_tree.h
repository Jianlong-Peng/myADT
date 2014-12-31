/*=============================================================================
#     FileName: mrb_tree.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-08 14:59:22
#   LastChange: 2014-11-21 10:59:46
#      History: 
#=============================================================================*/
#ifndef  MY_MRB_TREE_H_INC
#define  MY_MRB_TREE_H_INC

#include "mconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

struct RBNode;
typedef struct RBNode* RBNodePtr;

struct RBTree;
typedef struct RBTree* RBTreePtr;

typedef enum {RED, BLACK} RBCOLOR;

RBTreePtr rb_new(void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*));

RBTreePtr rb_copy(const RBTreePtr t);

void rb_clear(RBTreePtr t);

void rb_free(RBTreePtr *t);

int rb_size(const RBTreePtr t);
RBNodePtr rb_root(RBTreePtr t);
RBNodePtr rb_left(RBNodePtr node);
RBNodePtr rb_right(RBNodePtr node);
void *rb_data(RBNodePtr node);
RBCOLOR rb_node_color(RBNodePtr node);
BOOL rb_is_nil(const RBTreePtr t, const RBNodePtr node);

RBNodePtr rb_has(RBTreePtr t, void *src);

BOOL rb_insert(RBTreePtr t, void *src);

void rb_delete(RBTreePtr t, void *src);

void rb_for_each(RBTreePtr t, void (*func)(void **elem, void *args), void *args);

// func: user-defined function to display invalid nodes
// not fully implemented!!!!
BOOL rb_valid(const RBTreePtr t, void (*func)(const void *));

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_MRB_TREE_H_INC  ----- */

