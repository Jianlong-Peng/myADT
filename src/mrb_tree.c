/*=============================================================================
#     FileName: mrb_tree.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-08 15:04:03
#   LastChange: 2014-11-21 11:00:30
#      History: 
#=============================================================================*/
#include "mrb_tree.h"
#include "mtools.h"
#include "memory.h"


struct RBNode
{
    RBCOLOR _color;
    void *_data;
    RBNodePtr _left;
    RBNodePtr _right;
    RBNodePtr _parent;
};

struct RBTree
{
    int _size;
    void *(*_copy)(const void*);
    void (*_free)(void *);
    int (*_cmp)(const void*, const void*);
    RBNodePtr _root;
    RBNodePtr _nil;
};

RBTreePtr rb_new(void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*))
{
    RBTreePtr t;
    NEW(t);
    t->_size = 0;
    t->_copy = (copy_func==NULL)?adt_copy_nothing:copy_func;
    t->_free = (free_func==NULL)?adt_free_nothing:free_func;
    t->_cmp = cmp_func;
    //t->_root = NULL;
    NEW(t->_nil);
    t->_nil->_color = BLACK;
    t->_nil->_data = NULL;
    t->_nil->_left = t->_nil->_right = t->_nil->_parent = NULL;
    t->_root = t->_nil;
    return t;
}

static void rb_copy_node(RBNodePtr *node2, RBNodePtr nil2, 
    RBNodePtr node1, void *(*_copy)(const void*))
{
    if(node1->_left || node1->_right) {
        NEW(*node2);
        (*node2)->_color = node1->_color;
        (*node2)->_data = _copy(node1->_data);
        rb_copy_node(&(*node2)->_left, nil2, node1->_left, _copy);
        if((*node2)->_left != nil2)
            (*node2)->_left->_parent = *node2;
        rb_copy_node(&(*node2)->_right, nil2, node1->_right, _copy);
        if((*node2)->_right != nil2)
            (*node2)->_right->_parent = *node2;
    }
    else
        *node2 = nil2;
}
RBTreePtr rb_copy(const RBTreePtr t)
{
    RBTreePtr t2;
    CHECK_NULL_POINTER(t, "rb_copy");
    t2 = rb_new(t->_copy, t->_free, t->_cmp);
    rb_copy_node(&t2->_root, t2->_nil, t->_root, t2->_copy);
    t2->_root->_parent = t->_nil;
    t2->_size = t->_size;
    return t2;
}

static void rb_clear_node(RBNodePtr *root, void (*_free)(void*))
{
    if((*root)->_left || (*root)->_right) {
        _free((*root)->_data);
        rb_clear_node(&(*root)->_left, _free);
        rb_clear_node(&(*root)->_right, _free);
        FREE(*root);
    }
}
void rb_clear(RBTreePtr t)
{
    if(t && t->_size) {
        rb_clear_node(&t->_root, t->_free);
        t->_root = t->_nil;
        t->_size = 0;
    }
}

void rb_free(RBTreePtr *t)
{
    if(t && *t) {
        rb_clear(*t);
        FREE((*t)->_nil);
        FREE(*t);
    }
}


int rb_size(const RBTreePtr t)
{
    CHECK_NULL_POINTER(t, "rb_size");
    return t->_size;
}

RBNodePtr rb_root(RBTreePtr t)
{
    CHECK_NULL_POINTER(t, "rb_root");
    return (t->_root==t->_nil)?NULL:(t->_root);
}

RBNodePtr rb_left(RBNodePtr node)
{
    CHECK_NULL_POINTER(node, "rb_left");
    return node->_left;
}

RBNodePtr rb_right(RBNodePtr node)
{
    CHECK_NULL_POINTER(node, "rb_right");
    return node->_right;
}

RBCOLOR rb_node_color(RBNodePtr node)
{
    CHECK_NULL_POINTER(node, "rb_node_color");
    return node->_color;
}

void *rb_data(RBNodePtr node)
{
    CHECK_NULL_POINTER(node, "rb_data");
    CHECK_NULL_POINTER(node->_data, "rb_data");
    return (node==NULL)?NULL:(node->_data);
}

BOOL rb_is_nil(const RBTreePtr t, const RBNodePtr node)
{
    CHECK_NULL_POINTER(t, "rb_is_nil");
    CHECK_NULL_POINTER(node, "rb_is_nil");
    return (node==t->_nil)?TRUE:FALSE;
}

RBNodePtr rb_has(RBTreePtr t, void *src)
{
    RBNodePtr node;
    CHECK_NULL_POINTER(t, "rb_has");
    CHECK_NULL_POINTER(src, "rb_has");
    
    node = t->_root;
    while(node != t->_nil) {
        int val = t->_cmp(src, node->_data);
        if(val == 0)
            return node;
        else if(val < 0)
            node = node->_left;
        else
            node = node->_right;
    }
    return NULL;
}

/*
 *   x                       y
 *  / \                     / \
 * a   y       -->         x   c
 *    / \                 / \
 *   b   c               a   b
 */
static void rb_left_rotation(RBNodePtr x)
{
    RBNodePtr y = x->_right;
    if(y->_left->_parent)
        y->_left->_parent = x;
    x->_right = y->_left;
    y->_left = x;
    if(x == x->_parent->_left)
        x->_parent->_left = y;
    else if(x == x->_parent->_right)
        x->_parent->_right = y;
    else  // in case x->_parent == _nil
        ;
    y->_parent = x->_parent;
    x->_parent = y;
}

/*
 *       x                y
 *      / \              / \
 *     y   c    -->     a   x
 *    / \                  / \
 *   a   b                b   c
 */
static void rb_right_rotation(RBNodePtr x)
{
    RBNodePtr y = x->_left;
    if(y->_right->_parent)
        y->_right->_parent = x;
    x->_left = y->_right;
    y->_right = x;
    if(x == x->_parent->_left)
        x->_parent->_left = y;
    else if(x == x->_parent->_right)
        x->_parent->_right = y;
    else  // in case that y->_parent == _nil
        ;
    y->_parent = x->_parent;
    x->_parent = y;
}

static RBNodePtr rb_create_node(RBTreePtr t, void *src)
{
    RBNodePtr p;
    NEW(p);
    p->_color = RED;
    p->_data = t->_copy(src);
    p->_left = p->_right = p->_parent = t->_nil;
    return p;
}

/*
 * the followings is only for the cases when pz is left child of ppz.
 * for the case that pz is right child of ppz, same operations cane be applied
 * except 'left' and 'right' being exchanged.
 *
 * CASE 1
 *       ppz (B)                   ppz (R)
 *      /   \                     /   \
 * (R) pz    y (R)    -->    (B) pz    y (B)
 *     |                         |
 * (R) z                     (R) z
 *
 * CASE 2
 *       ppz (B)                     ppz (B)
 *      /   \         left          /   \
 * (R) pz    y (B)    ---->    (R) pz    y (B)     ---->  CASE 3
 *       \            rotate      /
 *    (R) z                  (R) z
 *
 * CASE 3
 *         ppz (B)                 ppz (R)                      pz (B)
 *        /   \                   /   \         right          /   \
 *   (R) pz    y (B)  -->    (B) pz    y (B)    ---->     (R) z     ppz (R)
 *      /                       /  \            rotate             /   \
 * (R) z                   (R) z    a                             a     y
 */
static void rb_insert_fixup(RBTreePtr t, RBNodePtr q)
{
    RBNodePtr z,pz,ppz,y,tmp;
    // fix-up
    z = q;
    pz = z->_parent;
    while(pz->_color == RED) {
        ppz = pz->_parent;
        if(pz == ppz->_left) {
            y = ppz->_right;
            // CASE 1
            if(y->_color == RED) {
                pz->_color = BLACK;
                y->_color = BLACK;
                ppz->_color = RED;
                // update z and pz
                z = ppz;
                pz = z->_parent;
            }
            else {
                // CASE 2
                if(z == pz->_right) {
                    rb_left_rotation(pz);
                    tmp = z;
                    z = pz;
                    pz = tmp;
                }
                // CASE 3
                pz->_color = BLACK;
                ppz->_color = RED;
                rb_right_rotation(ppz);
                if(ppz == t->_root)
                    t->_root = pz;
                // no more fixing-up is needed
            }
        }
        else {
            y = ppz->_left;
            if(y->_color == RED) {
                pz->_color = BLACK;
                y->_color = BLACK;
                ppz->_color = RED;
                // update z and pz
                z = ppz;
                pz = z->_parent;
            }
            else {
                // CASE 2
                if(z == pz->_left) {
                    rb_right_rotation(pz);
                    tmp = z;
                    z = pz;
                    pz = tmp;
                }
                // CASE 3
                pz->_color = BLACK;
                ppz->_color = RED;
                rb_left_rotation(ppz);
                if(ppz == t->_root)
                    t->_root = pz;
                // no more fixing-up is needed
            }
        }
    }
}

BOOL rb_insert(RBTreePtr t, void *src)
{
    RBNodePtr p,pp,q;
    int val;

    CHECK_NULL_POINTER(t, "rb_insert");
    CHECK_NULL_POINTER(src, "rb_insert");

    // find the leaf where to insert 'src'
    p = t->_root;
    pp = p;
    while(p != t->_nil) {
        pp = p;
        val = t->_cmp(src, p->_data);
        if(val == 0) {
            DISPLAY_WARNING("the element has already in the RB-tree", "rb_insert");
            return FALSE;
        }
        else if(val < 0)
            p = p->_left;
        else
            p = p->_right;
    }
    // create and insert node
    q = rb_create_node(t, src);
    if(t->_root == t->_nil)
        t->_root = q;
    else {
        q->_parent = pp;
        if(val < 0)
            pp->_left = q;
        else
            pp->_right = q;
    }
    t->_size++;
    
    // fix-up
    rb_insert_fixup(t, q);
    t->_root->_color = BLACK;

    return TRUE;
}


/*
 * the followings is only for the cases when X is left child of p[x].
 * for the case that X is right child of p[x], same operations cane be applied
 * except 'left' and 'right' being exchanged.
 *
 * CASE 1.
 *    p[x]                     p[x]  (R)        left            W (B)
 *   /    \       -->         /    \            -->           /   \
 *  X      W                 X      W           rotate  (R) p[x]  WR (B)
 * (B)    (R)               (B)    (B)                     /   \
 *                                                     (B) X    WL (B)  --> (W)
 *
 * CASE 2.
 *    p[x] (R)                 p[x] (R)  --> (X)
 *   /    \                   /    \
 *  X      W (B)  -->        X      W (R)
 * (B)    / \                      / \
 *       WL WR                    WL WR
 *      (B) (B)                  (B) (B)
 *
 * CASE 3.
 *    p[x] (R)                 p[x] (R)                         p[x] (R)
 *   /    \                   /    \            right          /    \
 *  X      W      -->    (B) X      W (R)       -->        (B) X     WL (B)   -->(W)
 * (B)    / \                      / \          rotation               \
 *       WL WR                    WL WR                                 W (R)
 *      (R) (B)                  (B) (B)                                  \
 *                                                                        WR (B)
 * CASE 4.
 *    p[x] (R)                 p[x] (B)                           W
 *   /    \                   /    \             left           /   \
 *  X      W (B)    -->      X      W (R)        -->       (B) p[x]  WR (B)   --> X = t->_root
 * (B)      \               (B)       \          rotate       /
 *           WR (R)                    WR(B)              (B) X
 */
static void rb_delete_fixup(RBTreePtr t, RBNodePtr x)
{
    RBNodePtr w;
    while(x!=t->_root && x->_color==BLACK) {
        if(x == x->_parent->_left) {
            w = x->_parent->_right;
            // CASE 1
            if(w->_color == RED) {
                w->_color = BLACK;
                x->_parent->_color = RED;
                rb_left_rotation(x->_parent);
                if(x->_parent == t->_root)
                    t->_root = w;
                w = x->_parent->_right;
            }
            // CASE 2
            if(w->_left->_color==BLACK && w->_right->_color==BLACK) {
                w->_color = RED;
                x = x->_parent;
            }
            else { 
                // CASE 3
                if(w->_right->_color == BLACK) {
                    w->_left->_color = BLACK;
                    w->_color = RED;
                    rb_right_rotation(w);
                    w = x->_parent->_right;
                }
                // CASE 4
                w->_color = x->_parent->_color;
                x->_parent->_color = BLACK;
                w->_right->_color = BLACK;
                rb_left_rotation(x->_parent);
                if(x->_parent == t->_root)
                    t->_root = w;
                x = t->_root;
            }
        }
        else {
            w = x->_parent->_left;
            // CASE 1
            if(w->_color == RED) {
                w->_color = BLACK;
                x->_parent->_color = RED;
                rb_right_rotation(x->_parent);
                if(x->_parent == t->_root)
                    t->_root = w;
                w = x->_parent->_left;
            }
            // CASE 2
            if(w->_right->_color==BLACK && w->_left->_color==BLACK) {
                w->_color = RED;
                x = x->_parent;
            }
            else {
                // CASE 3
                if(w->_left->_color == BLACK) {
                    w->_left->_color = BLACK;
                    w->_color = RED;
                    rb_left_rotation(w);
                    w = x->_parent->_left;
                }
                // CASE 4
                w->_color = x->_parent->_color;
                x->_parent->_color = BLACK;
                w->_left->_color = BLACK;
                rb_right_rotation(x->_parent);
                if(x->_parent == t->_root)
                    t->_root = w;
                x = t->_root;
            }
        }
    }
    x->_color = BLACK;
}

void rb_delete(RBTreePtr t, void *src)
{
    RBNodePtr x,p,y;

    CHECK_NULL_POINTER(t, "rb_delete");
    CHECK_NULL_POINTER(src, "rb_delete");

    // 1. find the node containing 'src'
    p = t->_root;
    while(p != t->_nil) {
        int val = t->_cmp(src, p->_data);
        if(val == 0)
            break;
        else if(val < 0)
            p = p->_left;
        else
            p = p->_right;
    }
    if(p == t->_nil)
        DISPLAY_WARNING("the element is not in the tree","rb_delete");

    // 2. get the the node with maximum element in p's left sub-tree if it has one.
    if(p->_left == t->_nil)
        y = p;
    else {
        y = p->_left;
        while(y->_right != t->_nil)
            y = y->_right;
    }
    // 3. try to delete the node 'y', and connect between y->parent and y->child
    if(y->_left != t->_nil)
        x = y->_left;
    else
        x = y->_right;
    x->_parent = y->_parent;
    if(y->_parent == t->_nil)
        t->_root = x;
    else {
        if(y == y->_parent->_left)
            y->_parent->_left = x;
        else
            y->_parent->_right = x;
    }
    t->_free(p->_data);
    if(y != p)
        p->_data = y->_data;
    // do fix-up
    if(y->_color == BLACK)
        rb_delete_fixup(t, x);
    FREE(y);
    t->_size--;
}

static void rb_for_each_inorder(RBNodePtr root, RBNodePtr _nil, 
    void (*func)(void **elem, void *args), void *args)
{
    if(root != _nil) {
        func(&root->_data, args);
        rb_for_each_inorder(root->_left, _nil, func, args);
        rb_for_each_inorder(root->_right, _nil, func, args);
    }
}
void rb_for_each(RBTreePtr t, void (*func)(void **elem, void *args), void *args)
{
    CHECK_NULL_POINTER(t, "rb_for_each");
    rb_for_each_inorder(t->_root, t->_nil, func, args);
}

static void rb_check_red(RBNodePtr root, BOOL *valid, void (*func)(const void*))
{
    if(root) {
        if(root->_color == RED) {
            if(root->_left && root->_left->_color==RED) {
                printf("RED node(");
                func(root->_data);
                printf(") has RED left child!\n");
                *valid = FALSE;
            }
            if(root->_right && root->_right->_color==RED) {
                printf("RED node(");
                func(root->_data);
                printf(") has RED right child!\n");
                *valid = FALSE;
            }
        }
        rb_check_red(root->_left, valid, func);
        rb_check_red(root->_right, valid, func);
    }
}

BOOL rb_valid(const RBTreePtr t, void (*func)(const void *))
{
    int valid;

    CHECK_NULL_POINTER(t, "rb_valid");

    // 1. check if root is BLACK
    if(t->_root->_color != BLACK) {
        printf("root node(");
        func(t->_root->_data);
        printf(") has color other than BLACK!\n");
        valid = FALSE;
    }

    // 2. check if each RED root has no RED child
    valid = TRUE;
    rb_check_red(t->_root, &valid, func);

    // 3. check if every path from a given node to any of its descendant leaves
    //    contains same number of BLACK nodes
    
    return valid;
}

