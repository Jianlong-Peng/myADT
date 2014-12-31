/*=============================================================================
#     FileName: mavl_tree.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-27 22:11:52
#   LastChange: 2014-11-05 14:55:14
#      History:
=============================================================================*/
#ifndef  MY_AVL_TREE_H_INC
#define  MY_AVL_TREE_H_INC

#include "mconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

struct AVLNode;
typedef struct AVLNode* AVLNodePtr;

struct AVLTree;
typedef struct AVLTree* AVLTreePtr;

AVLTreePtr avl_new(void *(*copy_func)(const void*), void (*free_func)(void*), 
        int (*cmp_func)(const void*, const void*));

void avl_clear(AVLTreePtr p);

void avl_free(AVLTreePtr *p);

int avl_size(const AVLTreePtr p);
BOOL avl_empty(const AVLTreePtr p);

AVLNodePtr avl_root(AVLTreePtr p);
AVLNodePtr avl_left(AVLNodePtr node);
AVLNodePtr avl_right(AVLNodePtr node);
void *avl_data(AVLNodePtr node);

void avl_insert(AVLTreePtr p, void *src);

AVLNodePtr avl_has(const AVLTreePtr p, void *src);

void avl_delete(AVLTreePtr p, void *src);

// apply 'func' to each element in 'p'
// args: user-defined arguments passed to 'func'
void avl_for_each(AVLTreePtr p, void (*func)(void **element, void *args), void *args);

// to check if the AVL-tree `p` is valid.
// `func`: to display data element of invalid node(s) without '\n'!!!!
BOOL avl_valid(const AVLTreePtr p, void (*func)(const void*));


#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_AVL_TREE_H_INC  ----- */

