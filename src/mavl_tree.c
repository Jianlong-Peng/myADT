/*=============================================================================
#     FileName: mavl_tree.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-27 22:18:49
#   LastChange: 2014-11-10 16:25:22
#      History:
=============================================================================*/
#include <stddef.h>
#include "mavl_tree.h"
#include "memory.h"
#include "mtools.h"
#include "tree_tools.h"


struct AVLNode
{
    //int _bf;  // balance factor
    void *_data;
    struct AVLNode *_left;
    struct AVLNode *_right;
};

struct AVLTree
{
    char _unbalanced;
    int _size;
    void *(*_copy)(const void*);
    void (*_free)(void *);
    int (*_cmp)(const void*, const void*);
    struct AVLNode *_root;
};

AVLTreePtr avl_new(void *(*copy_func)(const void*), void (*free_func)(void*), 
        int (*cmp_func)(const void*, const void*))
{
    AVLTreePtr t;
    NEW(t);
    t->_size = 0;
    t->_copy = (copy_func==NULL)?adt_copy_nothing:copy_func;
    t->_free = (free_func==NULL)?adt_free_nothing:free_func;
    t->_cmp = cmp_func;
    t->_root = NULL;
    return t;
}

static void avl_free_node(AVLNodePtr *root, void (*_free)(void*))
{
    if(root) {
        avl_free_node(&(*root)->_left, _free);
        avl_free_node(&(*root)->_right, _free);
        _free((*root)->_data);
        FREE(*root);
    }
}

void avl_clear(AVLTreePtr p)
{
    if(p && p->_size) {
        avl_free_node(&p->_root, p->_free);
        p->_size = 0;
        //p->_root = NULL;
    }
}
void avl_free(AVLTreePtr *p)
{
    if(p && *p) {
        avl_clear(*p);
        FREE(*p);
    }
}

int avl_size(const AVLTreePtr p)
{
    CHECK_NULL_POINTER(p, "avl_size");
    return p->_size;
}

BOOL avl_empty(const AVLTreePtr p)
{
    CHECK_NULL_POINTER(p, "avl_empty");
    return (p->_size==0)?TRUE:FALSE;
}

AVLNodePtr avl_root(AVLTreePtr p)
{
    CHECK_NULL_POINTER(p, "avl_root");
    return p->_root;
}

AVLNodePtr avl_left(AVLNodePtr node)
{
    CHECK_NULL_POINTER(node, "avl_left");
    return node->_left;
}

AVLNodePtr avl_right(AVLNodePtr node)
{
    CHECK_NULL_POINTER(node, "avl_right");
    return node->_right;
}

void *avl_data(AVLNodePtr node)
{
    return (node==NULL)?NULL:(node->_data);
}

static AVLNodePtr avl_create_node(void *src, void *(*_copy)(const void*))
{
    AVLNodePtr node;
    NEW(node);
    node->_data = _copy(src);
    node->_left = node->_right = NULL;
    //node->_bf = 0;
    return node;
}

/*
 * case 1:
 *      p (bf=+2)                 c  (bf=0)
 *     /                         / \
 *    c (bf=+1)    --LL-->     gc   p
 *   / \                           / \
 * gc  (cr)                     (cr) (pr)
 * if pr!=NULL, then cr!=NULL, only one of gcl and gcr is not NULL ==> bf(p)=0, bf(c)=0
 * if pr==NULL, then cr=gcl=gcr=NULL ==> bf(p)=0, bf(c)=0
 *
 * case 2:
 *      p (bf=+2)                 gc (bf=0)
 *     /                         /   \
 *    c (bf=-1)    --LR-->      c        p
 *   / \                      /  \      / \
 * (cl) gc                  (cl) (gcl) (gcr) (pr)
 * if pr!=NULL, then cl!=NULL, only one of gcl and gcr is not NULL ==> bf(p)=0/-1, bf(c)=0/+1
 * if pr==NULL, then cl=gcl=gcr=NULL  ==> bf(p)=0, bf(c)=0
 */
static void avl_left_rotation(AVLNodePtr *p, char *unbalanced)
{
    AVLNodePtr c = (*p)->_left;
    int hl,hr;

    hl = tree_height(c->_left, 2, avl_left, avl_right);
    hr = tree_height(c->_right, 2, avl_left, avl_right);

    if(hl-hr == 1) {
        (*p)->_left = c->_right;
        c->_right = *p;
        *p = c;
    }
    else if(hl-hr == -1) {
        AVLNodePtr gc = c->_right;
        c->_right = gc->_left;
        gc->_left = c;
        (*p)->_left = gc->_right;
        gc->_right = *p;
        *p = gc;
    }
    else
        ;
    *unbalanced = 0;

    /*
    if(c->_bf == 1) {
        (*p)->_left = c->_right;
        c->_right = *p;
        c->_bf = 0;
        (*p)->_bf = 0;
        *p = c;
    }
    else if(c->_bf == -1) {
        AVLNodePtr gc = c->_right;
        c->_right = gc->_left;
        gc->_left = c;
        (*p)->_left = gc->_right;
        gc->_right = *p;
        if((*p)->_right) {
            if((*p)->_left) {
                (*p)->_bf = 0;
                c->_bf = +1;
            } else {
                (*p)->_bf = -1;
                c->_bf = 0;
            }
        } else
            (*p)->_bf = c->_bf = 0;
        *p = gc;
    }
    else
        ;
    (*p)->_bf = 0;
    *unbalanced = 0;
    */
}

/*
 * case 1:
 *     p (-2)                          c (bf=0)
 *   /  \                            /  \
 * (pl)  c (-1)   --RR-->   (bf=0)  p    gc
 *      / \                        / \
 *    (cl) gc                   (pl) (cl)
 * if pl!=NULL, then cl!=NULL, only one of gcl and gcr is not NULL   ==> bf(p)=0, bf(c)=0
 * if pl==NULL, then cl=gcl=gcr=NULL  ==> bf(p)=0, bf(c)=0
 *
 * case 2:
 *     p (-2)                         gc  (bf=0)
 *   /  \                            /  \
 * (pl)  c (+1)   --RL-->           p         c
 *      / \                        / \      /   \
 *     gc (cr)                  (pl) (gcl) (gcr) (cr)
 * if pl!=NULL, then cr!=NULL, only one of gcl and gcr is not NULL ==> bf(p)=0/+1, bf(c)=0/-1
 * if pl==NULL, then cr=gcl=gcr=NULL  ==> bf(p)=bf(c)=0
 */
static void avl_right_rotation(AVLNodePtr *p, char *unbalanced)
{
    AVLNodePtr c = (*p)->_right;
    int hl,hr;

    hl = tree_height(c->_left, 2, avl_left, avl_right);
    hr = tree_height(c->_right, 2, avl_left, avl_right);

    if(hl-hr == -1) {
        (*p)->_right = c->_left;
        c->_left = *p;
        *p = c;
    }
    else if(hl-hr == +1) {
        AVLNodePtr gc = c->_left;
        (*p)->_right = gc->_left;
        gc->_left = *p;
        c->_left = gc->_right;
        gc->_right = c;
        *p = gc;
    }
    else
        ;
    *unbalanced = 0;

    /*
    if(c->_bf == -1) {
        (*p)->_right = c->_left;
        c->_left = *p;
        c->_bf = 0;
        (*p)->_bf = 0;
        *p = c;
    }
    else if(c->_bf == +1) {
        AVLNodePtr gc = c->_left;
        (*p)->_right = gc->_left;
        gc->_left = *p;
        c->_left = gc->_right;
        gc->_right = c;
        if((*p)->_left) {
            if((*p)->_right) {
                (*p)->_bf = 0;
                c->_bf = -1;
            } else {
                (*p)->_bf = +1;
                c->_bf = 0;
            }
        } else
            (*p)->_bf = c->_bf = 0;
        *p = gc;
    }
    else
        ;
    (*p)->_bf = 0;
    *unbalanced = 0;
    */
}

static void avl_insert_node(AVLNodePtr *root, AVLNodePtr node,
       char *unbalanced, int (*_cmp)(const void*, const void*))
{
    int result;

    if(*root == NULL) {
        *root = node;
        *unbalanced = 1;
    }
    else {
        result = _cmp(node->_data, (*root)->_data);
        if(result == 0) {
            DISPLAY_WARNING("the AVL-tree already contains the element", "avl_insert_node");
            return ;
        }
        else if(result < 0) {
            avl_insert_node(&(*root)->_left, node, unbalanced, _cmp);
            if(*unbalanced == 1) {
                int hl = tree_height((*root)->_left, 2, avl_left, avl_right);
                int hr = tree_height((*root)->_right, 2, avl_left, avl_right);
                switch(hl-hr) {
                    case  0: *unbalanced=0; break;
                    case +1: break;
                    case +2: avl_left_rotation(root, unbalanced); break;
                }
                /*
                if(hl-hr == 2)
                    avl_left_rotation(root, unbalanced);
                switch((*root)->_bf) {
                    case  0: (*root)->_bf=1; break;
                    case +1: avl_left_rotation(root, unbalanced); break;
                    case -1: (*root)->_bf=0; *unbalanced=0; break;
                }
                */
            }
        }
        else {
            avl_insert_node(&(*root)->_right, node, unbalanced, _cmp);
            if(*unbalanced == 1) {
                int hl = tree_height((*root)->_left, 2, avl_left, avl_right);
                int hr = tree_height((*root)->_right, 2, avl_left, avl_right);
                switch(hr-hl) {
                    case  0: *unbalanced=0; break;
                    case +1: break;
                    case +2: avl_right_rotation(root, unbalanced); break;
                }
                /*
                switch((*root)->_bf) {
                    case  0: (*root)->_bf=-1; break;
                    case +1: (*root)->_bf=0; *unbalanced=0; break;
                    case -1: avl_right_rotation(root, unbalanced); break;
                }
                */
            }
        }
    }
}

void avl_insert(AVLTreePtr t, void *src)
{
    AVLNodePtr node;
    CHECK_NULL_POINTER(t, "avl_insert");
    node = avl_create_node(src, t->_copy);
    avl_insert_node(&(t->_root), node, &(t->_unbalanced), t->_cmp);
}

AVLNodePtr avl_has(const AVLTreePtr p, void *src)
{
    AVLNodePtr node;
    CHECK_NULL_POINTER(p, "avl_has");
    CHECK_NULL_POINTER(src, "avl_has");

    node = p->_root;
    while(node) {
        int result = p->_cmp(src, node->_data);
        if(result < 0)
            node = node->_left;
        else if(result > 0)
            node = node->_right;
        else
            return node;
    }
    return NULL;
}

struct AVLInfo
{
    int _location; // -1 - left; +1 - right
    AVLNodePtr _node;
    struct AVLInfo *_prev;
};

static void avl_clear_stack(struct AVLInfo **_avl_top)
{
    struct AVLInfo *_each;
    while(*_avl_top) {
        _each = *_avl_top;
        _avl_top = &(*_avl_top)->_prev;
        FREE(_each);
    }
    *_avl_top = NULL;
}

void avl_delete(AVLTreePtr t, void *src)
{
    struct AVLInfo *temp;
    AVLNodePtr node, target;
    void *tmp_data;
    BOOL stop;
    struct AVLInfo *_avl_top=NULL;
    CHECK_NULL_POINTER(t, "avl_delete");
    CHECK_NULL_POINTER(src, "avl_delete");

    // 1. find the node contains 'src'
    node = t->_root;
    while(node) {
        int val = t->_cmp(src, node->_data);
        NEW(temp);
        temp->_node = node;
        temp->_prev = _avl_top;
        _avl_top = temp;
        if(val == 0) {
            _avl_top->_location = 0;
            break;
        }
        else if(val < 0) {
            node = node->_left;
            _avl_top->_location = -1;
        }
        else {
            node = node->_right;
            _avl_top->_location = +1;
        }
    }
    if(node == NULL) {
        DISPLAY_WARNING("the AVL-tree doesn't contains the element", "avl_delete");
        avl_clear_stack(&_avl_top);
        return ;
    }
    // 2. get the node with maximum value in the left-subtree
    if(node->_left == NULL) {
        temp = _avl_top;
        _avl_top = _avl_top->_prev;
        if(_avl_top->_location == -1)
            _avl_top->_node->_left = node->_right;
        else
            _avl_top->_node->_right = node->_right;
        t->_free(node->_data);
        FREE(node);
        FREE(temp);
    }
    else {
        _avl_top->_location = -1;
        target = node;
        node = node->_left;
        while(node->_right) {
            NEW(temp);
            temp->_node = node;
            temp->_location = 1;
            temp->_prev = _avl_top;
            _avl_top = temp;
            node = node->_right;
        }
        tmp_data = target->_data;
        target->_data = node->_data;
        t->_free(tmp_data);
        temp = _avl_top;
        _avl_top = _avl_top->_prev;
        FREE(temp);
        _avl_top->_node->_right = node->_left;
    }
    // 3. verfiy
    stop = FALSE;
    while(_avl_top || stop) {
        node = _avl_top->_node;
        if(_avl_top->_location == -1) {
            int hl = tree_height(node->_left, 2, avl_left, avl_right);
            int hr = tree_height(node->_right, 2, avl_left, avl_right);
            switch(hl-hr) {
                case -1: t->_unbalanced=0; stop=TRUE; break;
                case -2: avl_right_rotation(&node, &(t->_unbalanced)); break;
                case  0: break;
            }
            /*
            switch(node->_bf) {
                case  0: node->_bf=-1; t->_unbalanced=0; stop=TRUE; break;
                case -1: avl_right_rotation(&node, &(t->_unbalanced)); break;
                case +1: node->_bf=0; break;
            }
            */
        }
        else {
            int hl = tree_height(node->_left, 2, avl_left, avl_right);
            int hr = tree_height(node->_right, 2, avl_left, avl_right);
            switch(hl-hr) {
                case +1: t->_unbalanced=0; stop=TRUE; break;
                case  0: break;
                case +2: avl_left_rotation(&node, &(t->_unbalanced)); break;
            }
            /*
            switch(node->_bf) {
                case  0: node->_bf=1; t->_unbalanced=0; stop=TRUE; break;
                case -1: node->_bf=0; break;
                case +1: avl_left_rotation(&node, &(t->_unbalanced)); break;
            }
            */
        }
        temp = _avl_top;
        _avl_top = _avl_top->_prev;
        FREE(temp);
    }
    // clear _avl_top
    avl_clear_stack(&_avl_top);
}

static void avl_for_each_inorder(AVLNodePtr node, void (*func)(void **element, void *others), void *others)
{
    if(node) {
        avl_for_each_inorder(node->_left, func, others);
        func(&node->_data, others);
        avl_for_each_inorder(node->_right, func, others);
    }
}

void avl_for_each(AVLTreePtr p, void (*func)(void **element, void *others), void *others)
{
    CHECK_NULL_POINTER(p, "avl_for_each");
    avl_for_each_inorder(p->_root, func, others);
}


static int avl_valid_each_node(AVLNodePtr node, BOOL *valid, 
        int (*_cmp)(const void*, const void*),
        void (*func)(const void*))
{
    if(node) {
        int lh, rh;
        lh = avl_valid_each_node(node->_left, valid, _cmp, func);
        rh = avl_valid_each_node(node->_right, valid, _cmp, func);
        /*
        if(lh-rh != node->_bf) {
            printf("  node(");
            func(node->_data);
            printf(") lh=%d, rh=%d, _bf=%d\n", lh, rh, node->_bf);
            *valid = FALSE;
        }
        */
        if(abs(lh-rh) > 1) {
            printf("  node(");
            func(node->_data);
            printf(") has left and right subtree of height %d, %d respectively\n", 
                    lh, rh);
            *valid = FALSE;
        }
        if(node->_left && _cmp(node->_data, node->_left->_data)<0) {
            printf("  node(");
            func(node->_data);
            printf(") < node->_left(");
            func(node->_left->_data);
            printf(")\n");
            *valid = FALSE;
        }
        if(node->_right && _cmp(node->_right->_data, node->_data)<0) {
            printf("  node(");
            func(node->_data);
            printf(") > node->_right(");
            func(node->_right->_data);
            printf(")\n");
            *valid = FALSE;
        }
        if(node->_left && node->_right && _cmp(node->_right->_data, node->_left->_data)<0) {
            printf("  node->_left(");
            func(node->_left->_data);
            printf(") > node->_right(");
            func(node->_right->_data);
            printf(")\n");
            *valid = FALSE;
        }
        return ((lh>rh)?lh:rh)+1;
    }
    else
        return 0;
}

BOOL avl_valid(const AVLTreePtr p, void (*func)(const void*))
{
    int height;
    BOOL valid;

    CHECK_NULL_POINTER(p, "avl_valid");
    valid = TRUE;
    height = avl_valid_each_node(p->_root, &valid, p->_cmp, func);
    return valid;
}

