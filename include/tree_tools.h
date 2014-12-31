/*=============================================================================
#     FileName: tree_tools.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-08 16:59:45
#   LastChange: 2014-11-21 10:58:42
#      History: 
#=============================================================================*/
#ifndef  MY_TREE_TOOLS_H_INC
#define  MY_TREE_TOOLS_H_INC

#include <stdio.h>
#include "mavl_tree.h"
#include "m23_tree.h"
#include "mrb_tree.h"

#ifdef __cplusplus
extern "C" {
#endif

void *avl_get_left(void *root);
void *avl_get_right(void *root);
void *rb_get_left(void *root);
void *rb_get_right(void *root);
void *tt_get_left(void *root);
void *tt_get_middle(void *root);
void *tt_get_right(void *root);


typedef void *(*adt_get_child)(void *);

/*
 * OBJ:
 *   to calcualte the height of a given tree
 *
 * Parameters:
 *   root: root of tree, be of type xxNodePtr
 *   n: maximum number of possible children for each node
 *   ...: void *(*adt_get_child)(void*)
 */
int tree_height(void *root, int n, ...);
#define avl_tree_height(t) tree_height((void*)avl_root(t), 2, avl_get_left, avl_get_right)
#define rb_tree_height(t) (tree_height((void*)rb_root(t), 2, rb_get_left, rb_get_right)-1)
#define tt_tree_height(t) tree_height((void*)tt_root(t), 3, tt_get_left, tt_get_middle, tt_get_right)



/*
 * OBJ:
 *   to display contents of a given tree
 *
 * Parameters:
 *   root: root of tree, be of type xxNodePtr
 *   outf: where to display the tree
 *   print_data: display data. the 2nd parameter should be of type xxNodePtr
 *   n: number of maximum children for each node
 *   ...: void *(*adt_get_child)(void*); to get a child for each node
 */
void tree_display(void *root, FILE *outf,
        void (*print_data)(FILE*, const void*), int n, ...);
#define avl_tree_display(t, outf, print_data) \
    tree_display((void*)avl_root(t), outf, print_data, 2, avl_get_left, avl_get_right)

/*
#define rb_tree_display(t, outf, print_data) \
    tree_display((void*)rb_root(t), outf, print_data, 2, rb_get_left, rb_get_right)
*/
/*
 * display_data: to show the data in the node.
 *              if the node contains only one element, then just fprintf(outf, "element"),
 *              otherwise, should be fprintf(outf, "elem1 | elem2 |...")
 */
void tree_to_dot(void *root, FILE *outf,
        void (*display_data)(FILE*, const void*), int n, ...);
#define avl_tree_to_dot(t, outf, display_data) \
    tree_to_dot((void*)avl_root(t), outf, display_data, 2, avl_get_left, avl_get_right);
#define tt_tree_to_dot(t, outf, display_data) \
    tree_to_dot((void*)tt_root(t), outf, display_data, 3, tt_get_left, tt_get_middle, tt_get_right)

void rb_tree_to_dot(RBTreePtr t, FILE *outf, void (*display_data)(FILE*, const void*));


#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_TREE_TOOLS_H_INC  ----- */

