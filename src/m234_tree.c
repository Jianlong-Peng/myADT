/*=============================================================================
#     FileName: m234_tree.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-06 21:03:44
#   LastChange: 2014-11-10 16:11:44
#      History: 
#=============================================================================*/
#include "memory.h"
#include "m234_tree.h"
#include "adt_tools.h"

struct Two34Node
{
    void *_dl;
    void *_dm;
    void *_dr;
    Two34NodePtr _left;
    Two34NodePtr _left_middle;
    Two34NodePtr _right_middle;
    Two34NodePtr _right;
};

struct Two34Tree
{
    int _size;
    void *(*_copy)(const void*);
    void (*_free)(void *);
    int (*_cmp)(const void*, const void*);
    Two34NodePtr _root;
};

Two34TreePtr ttf_new(void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*))
{
    Two34TreePtr t;
    NEW(t);
    t->_size = 0;
    t->_copy = (copy_func==NULL)?adt_copy_nothing:copy_func;
    t->_free = (free_func==NULL)?adt_free_nothing:free_func;
    t->_cmp = cmp_func;
    t->_root = NULL;

    return t;
}

static void ttf_copy_node(Two34NodePtr *dest, Two34NodePtr src,
        void *(*_copy)(const void*))
{
    if(src) {
        NEW(*dest);
        (*dest)->_dl = (*dest)->_dm = (*dest)->_dr = NULL;
        if(src->_dl)
            (*dest)->_dl = _copy(src->_dl);
        if(src->_dm)
            (*dest)->_dm = _copy(src->_dm);
        if(src->_dr)
            (*dest)->_dr = _copy(src->_dr);
        ttf_copy_node(&(*dest)->_left, src->_left, _copy);
        ttf_copy_node(&(*dest)->_left_middle, src->_left_middle, _copy);
        ttf_copy_node(&(*dest)->_right_middle, src->_right_middle, _copy);
        ttf_copy_node(&(*dest)->_right, src->_right, _copy);
    }
}

Two34TreePtr ttf_copy(const Two34TreePtr t)
{
    Two34TreePtr t2;

    CHECK_NULL_POINTER(t, "ttf_copy");

    t2 = ttf_new(t->_copy, t->_free, t->_cmp);
    ttf_copy_node(&t2->_root, t->_root, t2->_copy);
    return t2;
}

static void ttf_clear_node(Two34NodePtr *root, void (*_free)(void*))
{
    if(root) {
        if((*root)->_dl)
            _free((*root)->_dl);
        if((*root)->_dm)
            _free((*root)->_dm);
        if((*root)->_dr)
            _free((*root)->_dr);
        ttf_clear_node(&(*root)->_left, _free);
        ttf_clear_node(&(*root)->_left_middle, _free);
        ttf_clear_node(&(*root)->_right_middle, _free);
        ttf_clear_node(&(*root)->_right, _free);
        FREE(*root);
    }
}
void ttf_clear(Two34TreePtr t)
{
    if(t && t->_size) {
        ttf_clear_node(&t->_root, t->_free);
        t->_size = 0;
    }
}

void ttf_free(Two34TreePtr *t)
{
    if(t) {
        ttf_clear(*t);
        FREE(*t);
    }
}

int ttf_size(const Two34TreePtr t)
{
    CHECK_NULL_POINTER(t, "ttf_size");
    return t->_size;
}

BOOL ttf_insert(Two34TreePtr t, void *src)
{
    DISPLAY_ERROR_EXIT("not implemented yest", "ttf_insert");
    return FALSE;
}

Two3NodePtr ttf_has(Two34TreePtr t, void *src)
{
    int val1,val2,val3;
    Two34NodePtr node;

    CHECK_NULL_POINTER(t, "ttf_has");
    CHECK_NULL_POINTER(src, "ttf_has");

    node = t->_root;
    while(node) {
        val1 = t->_cmp(src, node->_dl);
        if(val1 == 0)
            break;
        else if(val1 < 0)
            node = node->_left;
        else {
            if(node->_dm) {
                val2 = t->_cmp(src, node->_dm);
                if(val2 == 0)
                    break;
                else if(val2 < 0)
                    node = node->_left_middle;
                else {
                    if(node->_dr) {
                        val3 = t->_cmp(src, node->_dr);
                        if(val3 == 0)
                            break;
                        else if(val3 < 0)
                            node = node->_right_middle;
                        else
                            node = node->_right;
                    }
                }
            }
        }
    }
    return node;
}

void ttf_delete(Two34TreePtr t, void *src)
{
    DISPLAY_ERROR_EXIT("not implemented yet", "ttf_delete");
}


static void ttf_for_each_inorder(Two34NodePtr root, void (*func)(void **elem, void *args), void *args)
{
    if(root) {
        if(root->_dl)
            func(&root->_dl, args);
        if(root->_dm)
            func(&root->_dm, args);
        if(root->_dr)
            func(&root->_dr, args);
        ttf_for_each_inorder(root->_left, func, args);
        ttf_for_each_inorder(root->_left_middle, func, args);
        ttf_for_each_inorder(root->_right_middle, func, args);
        ttf_for_each_inorder(root->_right, func, args);
    }
}
void ttf_for_each(Two34TreePtr t, void (*func)(void **elem, void *args), void *args)
{
    TEST_NULL_POINTER(t, "ttf_for_each");
    ttf_for_each_inorder(t->_root, func, args);
}

