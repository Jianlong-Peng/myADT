/*=============================================================================
#     FileName: tree_tools.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-08 17:03:17
#   LastChange: 2014-11-21 10:59:27
#      History: 
#=============================================================================*/
#include <stdarg.h>
#include "mconfig.h"
#include "tree_tools.h"
#include "mavl_tree.h"
#include "mrb_tree.h"
#include "m23_tree.h"
#include "mqueue.h"
#include "mtools.h"
#include "memory.h"


void *avl_get_left(void *root)
{
    AVLNodePtr temp = (AVLNodePtr)root;
    return (void*)avl_left(temp);
}

void *avl_get_right(void *root)
{
    AVLNodePtr temp = (AVLNodePtr)root;
    return (void*)avl_right(temp);
}

void *rb_get_left(void *root)
{
    RBNodePtr temp = (RBNodePtr)root;
    return (void*)rb_left(temp);
}

void *rb_get_right(void *root)
{
    RBNodePtr temp = (RBNodePtr)root;
    return (void*)rb_right(temp);
}

void *tt_get_left(void *root)
{
    Two3NodePtr temp = (Two3NodePtr)root;
    return (void*)tt_left(temp);
}

void *tt_get_middle(void *root)
{
    Two3NodePtr temp = (Two3NodePtr)root;
    return (void*)tt_middle(temp);
}

void *tt_get_right(void *root)
{
    Two3NodePtr temp = (Two3NodePtr)root;
    return (void*)tt_right(temp);
}

static void tree_height_inorder(void *root, int *max_h, int h,
    int n, adt_get_child *funcs)
{
    if(root) {
        BOOL is_leaf = TRUE;
        int i;
        for(i=0; i<n; ++i) {
            if(funcs[i](root) != NULL) {
                is_leaf = FALSE;
                break;
            }
        }
        ++h;
        if(is_leaf) {
            if(h > *max_h)
                *max_h = h;
        }
        else {
            for(i=0; i<n; ++i)
                tree_height_inorder(funcs[i](root), max_h, h, n, funcs);
        }
    }
}

int tree_height(void *root, int n, ...)
{
    int max_h=0, h=0, i;
    adt_get_child *funcs;
    va_list var_arg;

    NEWC(funcs, n);
    va_start(var_arg, n);
    for(i=0; i<n; ++i)
        funcs[i] = va_arg(var_arg, adt_get_child);
    va_end(var_arg);
    
    tree_height_inorder(root, &max_h, h, n, funcs);
    FREE(funcs);
    
    return max_h;
}


struct EachNode
{
    int _parent;
    int _current;
    int _dir;
    void *_data;
};

void tree_to_dot(void *root, FILE *outf,
        void (*display_data)(FILE*, const void*), int n, ...)
{
    QueuePtr q;
    struct EachNode *node1, *node2;
    adt_get_child *funcs;
    int i;
    //int h, height;
    va_list var_arg;
    //char *temp;
    int count;
    
    CHECK_NULL_POINTER(root, "tree_to_dot");

    NEWC(funcs, n);
    va_start(var_arg, n);
    for(i=0; i<n; ++i)
        funcs[i] = va_arg(var_arg, adt_get_child);
    va_end(var_arg);
    
    /*
    h=0; height=0;
    tree_height_inorder(root, &height, h, n, funcs);

    fprintf(outf, "digraph G {\n"
            "label=\"tree of height %d\";\n"
            "node[shape=record];\n", height);
    */
    fprintf(outf, "digraph G {\n"
            "node[shape=record];\n");
    
    q = queue_new(NULL, adt_free);
    NEW(node1);
    node1->_data = root;
    node1->_parent = 0;
    node1->_current = 1;
    queue_push(q, node1);

    count = 1;
    //printf("in tree_to_dot\n");
    while(!queue_empty(q)) {
        node2 = queue_front(q);
        //temp = data_to_str(node2->_data);
        fprintf(outf, "N%d[label=\"", node2->_current);
        display_data(outf, node2->_data);
        fprintf(outf, "\"];\n");
        //fprintf(outf, "N%d[label=\"%s\"];\n",node2->_current,temp);
        //FREE(temp);
        // add edge from parent to child
        if(node2->_parent != 0) {
            if(n == 2) {
                if(node2->_dir == 0)
                    fprintf(outf, "N%d:sw->N%d;\n", node2->_parent, node2->_current);
                else
                    fprintf(outf, "N%d:se->N%d;\n", node2->_parent, node2->_current);
            }
            else if(n == 3) {
                if(node2->_dir == 0)
                    fprintf(outf, "N%d:sw->N%d;\n", node2->_parent, node2->_current);
                else if(node2->_dir == 1)
                    fprintf(outf, "N%d:s->N%d;\n", node2->_parent, node2->_current);
                else
                    fprintf(outf, "N%d:se->N%d;\n", node2->_parent, node2->_current);
            } else
                fprintf(outf, "N%d->N%d;\n", node2->_parent, node2->_current);
        }
        // add children
        for(i=0; i<n; ++i) {
            if(funcs[i](node2->_data) == NULL)
                continue;
            NEW(node1);
            node1->_data = funcs[i](node2->_data);
            node1->_parent = node2->_current;
            node1->_current = ++count;
            node1->_dir = i;
            queue_push(q, node1);
        }
        queue_pop(q);
    }
    fprintf(outf, "}\n");

    queue_free(&q);
    FREE(funcs);
}

void rb_tree_to_dot(RBTreePtr t, FILE *outf, void (*display_data)(FILE*, const void*))
{
    QueuePtr q;
    struct EachNode *node1, *node2;
    //adt_get_child *funcs;
    adt_get_child *funcs;
    int i, h, height;
    //va_list var_arg;
    //char *temp;
    int count;
    
    CHECK_NULL_POINTER(t, "rb_tree_to_dot");

    NEWC(funcs, 2);
    funcs[0] = rb_left;
    funcs[1] = rb_right;
    h=0; height=0;
    tree_height_inorder((void*)rb_root(t), &height, h, 2, funcs);
    --height;

    fprintf(outf, "digraph G {\n"
            "label=\"red-black tree of height %d\";\n"
            "node[shape=ellipse, fontcolor=white, style=filled];\n", height);

    q = queue_new(NULL, adt_free);
    NEW(node1);
    node1->_data = (void*)rb_root(t);
    node1->_parent = 0;
    node1->_current = 1;
    queue_push(q, node1);

    count = 1;
    while(!queue_empty(q)) {
        node2 = queue_front(q);
        fprintf(outf, "N%d[label=\"", node2->_current);
        display_data(outf, node2->_data);
        if(rb_node_color((RBNodePtr)(node2->_data)) == BLACK)
            fprintf(outf, "\", fillcolor=\"black\"];\n");
        else
            fprintf(outf, "\", fillcolor=\"red\"];\n");
        // add edge from parent to child
        if(node2->_parent != 0) {
            if(node2->_dir == 0)
                fprintf(outf, "N%d:sw->N%d;\n", node2->_parent, node2->_current);
            else
                fprintf(outf, "N%d:se->N%d;\n", node2->_parent, node2->_current);
        }
        // add children
        for(i=0; i<2; ++i) {
            if(rb_is_nil(t, funcs[i](node2->_data)))
                continue;
            NEW(node1);
            node1->_data = funcs[i](node2->_data);
            node1->_parent = node2->_current;
            node1->_current = ++count;
            node1->_dir = i;
            queue_push(q, node1);
        }
        queue_pop(q);
    }
    fprintf(outf, "}\n");

    queue_free(&q);
    FREE(funcs);
}


struct EachLevel
{
    int _level;
    void *_data;
};

void tree_display(void *root, FILE *outf,
        void (*print_data)(FILE*, const void*), int n, ...)
{
    QueuePtr q;
    struct EachLevel *node1, *node2;
    adt_get_child *funcs;
    int prev_level;
    int i, h, height;
    va_list var_arg;
    
    CHECK_NULL_POINTER(root, "tree_display");

    NEWC(funcs, n);
    va_start(var_arg, n);
    for(i=0; i<n; ++i)
        funcs[i] = va_arg(var_arg, adt_get_child);
    va_end(var_arg);
    
    h=0; height=0;
    tree_height_inorder(root, &height, h, n, funcs);
    if(height == 0) {
        DISPLAY_WARNING("empty tree", "btree_display");
        return ;
    }
    fprintf(outf, "\n=====================================\n");
    fprintf(outf, "# Contents of tree of height %-7d#\n", height);
    fprintf(outf, "# [N] indicates NULL                #\n");
    fprintf(outf, "=====================================\n");

    q = queue_new(NULL, adt_free);
    NEW(node1);
    node1->_level = 1;
    node1->_data = root;
    queue_push(q, node1);

    prev_level = 0;
    while(!queue_empty(q)) {
        node2 = queue_front(q);
        if(node2->_level != prev_level) {
            fprintf(outf, "\nlevel %-3d:", node2->_level);
            //for(i=0; i<height-node2->_level; ++i)
            //    fprintf(outf, "    ");
        }
        prev_level = node2->_level;
        fprintf(outf, " ");
        if(node2->_data)
            print_data(outf, node2->_data);
        else
            fprintf(outf, "[N]");
        // add children
        if(node2->_level < height) {
            for(i=0; i<n; ++i) {
                NEW(node1);
                node1->_level = node2->_level + 1;
                node1->_data = funcs[i](node2->_data);
                queue_push(q, node1);
            }
        }
        queue_pop(q);
    }
    fprintf(outf, "\n");

    queue_free(&q);
}

