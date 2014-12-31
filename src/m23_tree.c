/*=============================================================================
#     FileName: m23_tree.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-05 15:14:50
#   LastChange: 2014-11-10 16:15:08
#      History: 
#=============================================================================*/
#include "mconfig.h"
#include "memory.h"
#include "m23_tree.h"
#include "mtools.h"
#include "mstack.h"

struct Two3Node
{
    void *_dl;
    void *_dr;
    Two3NodePtr _left;
    Two3NodePtr _middle;
    Two3NodePtr _right;
};

struct Two3Tree
{
    int _size;
    void *(*_copy)(const void*);
    void (*_free)(void *);
    int (*_cmp)(const void*, const void*);
    Two3NodePtr _root;
};

Two3TreePtr tt_new(void *(*copy_func)(const void*), void (*free_func)(void*),
        int (*cmp_func)(const void*, const void*))
{
    Two3TreePtr t;

    CHECK_NULL_POINTER(cmp_func, "tt_new");
    NEW(t);
    t->_size = 0;
    t->_copy = (copy_func==NULL)?adt_copy_nothing:copy_func;
    t->_free = (free_func==NULL)?adt_free_nothing:free_func;
    t->_cmp = cmp_func;
    t->_root = NULL;

    return t;
}

static void tt_copy_node(Two3NodePtr *dest, Two3NodePtr src,
        void *(*_copy)(const void*))
{
    if(src) {
        NEW(*dest);
        (*dest)->_dl = NULL;
        (*dest)->_dr = NULL;
        if(src->_dl)
            (*dest)->_dl = _copy(src->_dl);
        if(src->_dr)
            (*dest)->_dr = _copy(src->_dl);
        tt_copy_node(&(*dest)->_left, src->_left, _copy);
        tt_copy_node(&(*dest)->_middle, src->_middle, _copy);
        tt_copy_node(&(*dest)->_right, src->_right, _copy);
    }
    else
        *dest = NULL;
}

Two3TreePtr tt_copy(const Two3TreePtr t)
{
    Two3TreePtr t2;

    CHECK_NULL_POINTER(t, "tt_copy");

    t2 = tt_new(t->_copy, t->_free, t->_cmp);
    t2->_size = t->_size;
    tt_copy_node(&t2->_root, t->_root, t2->_copy);

    return t2;
}

static void tt_clear_node(Two3NodePtr *root, void (*_free)(void *))
{
    if(*root) {
        tt_clear_node(&(*root)->_left, _free);
        tt_clear_node(&(*root)->_middle, _free);
        tt_clear_node(&(*root)->_right, _free);
        if((*root)->_dl)
            _free((*root)->_dl);
        if((*root)->_dr)
            _free((*root)->_dr);
        FREE(*root);
    }
}

void tt_clear(Two3TreePtr t)
{
    if(t && t->_size) {
        tt_clear_node(&t->_root, t->_free);
        t->_size = 0;
    }
}

void tt_free(Two3TreePtr *t)
{
    if(t) {
        tt_clear(*t);
        FREE(*t);
    }
}

int tt_size(const Two3TreePtr t)
{
    CHECK_NULL_POINTER(t, "tt_size");
    return t->_size;
}

/*
 * let y=src
 *  y         root=[y| ]
 * p q   -->      /  |
 *               p   q
 * then, let p=root
 */
static void tt_new_root(Two3NodePtr *p, void *src, Two3NodePtr q)
{
    Two3NodePtr root;
    NEW(root);
    root->_dl = src;
    root->_dr = NULL;
    root->_left = *p;
    root->_middle = q;
    root->_right = NULL;
    *p = root;
}

struct _23_node_stack
{
    Two3NodePtr node;
    int direction;  // -1: left; 0: middle; +1: right
};

// top->down, to find the leaf node, and record the path
// return TRUE, if 'src' is already in the tree
static BOOL tt_find_node(Two3NodePtr node, void *src, 
        StackPtr _23_top, 
        int (*_cmp)(const void*, const void*))
{
    int val1, val2;
    struct _23_node_stack *stack_node;
    while(node) {
        NEW(stack_node);
        stack_node->node = node;
        stack_node->direction = -1;
        val1 = _cmp(src, node->_dl);
        if(val1 == 0)
            return TRUE;
        else if(val1 < 0) {
            stack_node->direction = -1;
            node = node->_left;
        }
        else {
            if(node->_dr) {
                val2 = _cmp(src, node->_dr);
                if(val2 == 0)
                    return TRUE;
                else if(val2 < 0) {
                    stack_node->direction = 0;
                    node = node->_middle;
                }
                else {
                    stack_node->direction = +1;
                    node = node->_right;
                }
            } else {
                stack_node->direction = 0;
                node = node->_middle;
            }
        }
        stack_push(_23_top, stack_node);
    }
    return FALSE;
}

/*
 * Attention: if q is not NULL, then q > y
 *
 * CASE 1.   y > p->_dl
 *   p= [13| ]  y=20             p= [13|20]
 *     /   |    q           -->    /   |   \
 *   [L]  [M]                    [L]  [M]   q
 *
 * CASE 2.   y < p->_dl
 *   p= [13| ]  y=10             p= [10|13]
 *     /   |    q           -->    /   |   \
 *   [L]  [M]                    [L]   q   [M]
 */
static void tt_put_in(Two3NodePtr p, Two3NodePtr q, void *src, 
        int (*_cmp)(const void*, const void*))
{
    if(_cmp(p->_dl, src) < 0) {
        p->_dr = src;
        p->_right = q;
    }
    else {
        p->_dr = p->_dl;
        p->_dl = src;
        p->_right = p->_middle;
        p->_middle = q;
    }
}

/*
 * CASE 1. y(src) is from left child
 *     p= [13|20]  + y=10                   p= [10| ]  new_q= [20| ]    y=13
 *       /   |   \   q=[11| ]    -->          /   |          /   |
 *    [9|]  [M]  [R]                        [9|] [11|]      M   [R]
 *  (old_p)                               (old_p) (old_q)
 *
 * new_q->_left=p->_middle; new_q->_middle=p->_right; new_q->_right=NULL;
 * p->_middle=old_q; p->_right=NULL
 *
 *
 * CASE 2. y(src) is from middle child
 *    p= [13|30]   + y=15                   p= [13| ]  new_q=[30| ]     y=15
 *      /   |   \    q=[20| ]    -->          /   |         /   |
 *    [L] [14|] [R]                         [L] [14|]    [20|] [R]
 *        (old_p)                              (old_p)    (q)
 *
 * new_q->_left=old_q; new_q->_middle=p->_right; new_q->_right=NULL;
 * p->_right=NULL;
 *
 *
 * CASE 3. y(src) is from right child
 *   p= [13|30]   + y=35                    p= [13| ] new_q=[35| ]     y=30
 *     /   |   \    q=[42|]      -->          /   |        /   |
 *   [L]  [M] [32|]                         [L]  [M]    [32|] [42|]
 *           (old_p)                                   (old_p) (q)
 *             [R]                                       [R]
 *
 * new_q->_left=p->_right; new_q->-middle=old_q; new_q->_right=NULL;
 * p->_right=NULL
 *
 *
 * CASE 4. the node to be splitted is the leaf
 *     some_node + y=11                       some_node                y=10
 *        |        q = NULL                      |
 *   p= [9|10]                   -->        p= [9| ]  new_q=[11| ]
 * in this case, any of CASE 1~3 can be applied.
 */
static void tt_split_node(Two3NodePtr p, Two3NodePtr *q, void **src, int direction, 
        int (*_cmp)(const void*, const void*))
{
    Two3NodePtr new_q;
    void *l, *y, *r;
    NEW(new_q);
    // get the min, meadian, and max
    if(_cmp(p->_dl, *src) < 0) {
        if(_cmp(*src, p->_dr) < 0) {
            l = p->_dl;
            y = *src;
            r = p->_dr;
        }
        else {
            l = p->_dl;
            y = p->_dr;
            r = *src;
        }
    }
    else {
        l = *src;
        y = p->_dl;
        r = p->_dr;
    }
    // split p
    p->_dl = l;
    p->_dr = NULL;
    new_q->_dl = r;
    new_q->_dr = NULL;
    *src = y;
	// CASE 1 -- `src` is from left child of `p`
	if(direction == -1) {
		new_q->_left = p->_middle;
		new_q->_middle = p->_right;
		new_q->_right = NULL;
		p->_middle = *q;
		p->_right = NULL;
	}
    // CASE 2  -- `src` is from middle child of `p`
    else if(direction == 0) {
        new_q->_left = *q;
        new_q->_middle = p->_right;
        new_q->_right = NULL;
        p->_right = NULL;
    }
    // CASE 3 -- `src` is from right child of `p`
    else {
        new_q->_left = p->_right;
        new_q->_middle = *q;
        new_q->_right = NULL;
        p->_right = NULL;
    }
    *q = new_q;
}

BOOL tt_insert(Two3TreePtr t, void *data)
{
    Two3NodePtr p,q;
    struct _23_node_stack *top;
    StackPtr _23_top;
    int direction;
    void *src;

    CHECK_NULL_POINTER(t, "tt_insert");
    CHECK_NULL_POINTER(data, "tt_insert");

    _23_top = stack_new(NULL, adt_free);
    src = t->_copy(data);
    if(t->_root == NULL)
        tt_new_root(&t->_root, src, NULL);
    else {
        if(tt_find_node(t->_root, src, _23_top, t->_cmp)) {
            DISPLAY_WARNING("the tree already has the element", "tt_insert");
            stack_free(&_23_top);
            return FALSE;
        }
        q = NULL;
        while(!stack_empty(_23_top)) {
            top = (struct _23_node_stack*)stack_top(_23_top);
            p = top->node;
            direction = top->direction;
            stack_pop(_23_top);
            if(p->_dr == NULL) {
                tt_put_in(p, q, src, t->_cmp);
                break;
            }
            else {
                tt_split_node(p, &q, &src, direction, t->_cmp);
                if(p == t->_root) {
                    tt_new_root(&p, src, q);
                    t->_root = p;
                    break;
                }
            }
        }
        stack_free(&_23_top);
    }

    return TRUE;
}

Two3NodePtr tt_has(Two3TreePtr t, void *src)
{
    Two3NodePtr node;
    int result1, result2;

    CHECK_NULL_POINTER(t, "tt_has");
    CHECK_NULL_POINTER(src, "tt_has");

    node = t->_root;
    while(node) {
        if(node->_dl) {
            result1 = t->_cmp(src, node->_dl);
            if(result1 == 0)
                break;
            else if(result1 < 0)
                node = node->_left;
            else {
                if(node->_dr) {
                    result2 = t->_cmp(src, node->_dr);
                    if(result2 == 0)
                        break;
                    else if(result2 < 0)
                        node = node->_middle;
                    else
                        node = node->_right;
                } else
                    node = node->_middle;
            }
        }
    }
    return node;
}

/*
 * CASE 1.1
 *       r= [rl|rr]                  r= [rr | ]
 *         /   |   \        -->        /    |  \       FREE(q)
 *        [] [ql|] [R]            [rl|ql]  [R]  NULL
 *       (p)  (q)                    |   \
 *                                 qleft qmiddle
 * 
 * CASE 1.2
 *       r= [rl|rr]                  r= [  ql   |  rr ]
 *         /   |   \        -->        /        |    \
 *       [] [ql|qr] [R]            [rl|]     [qr|]    [R]
 *      (p)  (q)                      |      /  |
 *                                qleft  qmiddle qright
 *
 * CASE 2.1
 *       r= [rl|rr]                  r= [rr  | ]
 *         /   |   \        -->        /     |  \      FREE(p)
 *     [ql|]  []   [R]             [ql|rr]  [R]  NULL
 *     (q)    (p)                         \
 *                                       pleft
 *
 * CASE 2.2
 *       r= [rl|rr]                  r= [    qr    | rr ]
 *         /   |   \        -->        /           |    \
 *     [ql|qr] []  [R]             [ql|]       [rl | ]  [R]
 *      (q)    (p)                     \       /   |
 *                                     NULL  qright pleft
 *
 * CASE 3.1
 *       r= [rl | rr]                  r= [rl |  ]
 *         /    |    \       -->         /    |   \      FREE(p)
 *       [L] [ql|]   []                [L] [ql|rr] NULL
 *            (q)    (p)                         \
 *                                               pleft
 * CASE 3.2
 *       r= [rl | rr]                  r= [rl |   qr  ]
 *         /    |    \        -->        /    |       \
 *       [L] [ql|qr]  []               [L] [ql|]      [rr  | ]
 *            (q)     (p)                       \     /    |
 *                                             NULL qright pleft
 */
static void tt_rotation(Two3NodePtr p, Two3NodePtr r)
{
    Two3NodePtr q;
    if(p == r->_left) {
        q = r->_middle;
        // CASE 1.2
        if(q->_dr) {
            p->_dl = r->_dl;
            r->_dl = q->_dl;
            q->_dl = q->_dr;
            q->_dr = NULL;
            p->_middle = q->_left;
            q->_left = q->_middle;
            q->_middle = q->_right;
            q->_right = NULL;
        }
        // CASE 1.1
        else {
            p->_dl = r->_dl;
            p->_dr = q->_dl;
            r->_dl = r->_dr;
            r->_dr = NULL;
            p->_middle = q->_left;
            p->_right = q->_middle;
            r->_middle = r->_right;
            r->_right = NULL;
            FREE(q);
        }
    }
    else if(p == r->_middle) {
        q = r->_left;
        // CASE 2.2
        if(q->_dr) {
            p->_dl = r->_dl;
            r->_dl = q->_dr;
            q->_dr = NULL;
            p->_middle = p->_left;
            p->_left = q->_right;
            q->_right = NULL;
        }
        // CASE 2.1
        else {
            q->_dr = r->_dl;
            r->_dl = r->_dr;
            r->_dr = NULL;
            q->_right = p->_left;
            r->_middle = r->_right;
            r->_right = NULL;
            FREE(p);
        }
    }
    else {
        q = r->_middle;
        // CASE 3.2
        if(q->_dr) {
            p->_dl = r->_dr;
            r->_dr = q->_dr;
            q->_dr = NULL;
            p->_middle = p->_left;
            p->_left = q->_right;
            q->_right = NULL;
        }
        // CASE 3.1
        else {
            q->_dr = r->_dr;
            r->_dr = NULL;
            q->_right = p->_left;
            r->_right = NULL;
            FREE(p);
        }
    }
}

void tt_delete(Two3TreePtr t, void *src)
{
    StackPtr _23_top;
    struct _23_node_stack *_top, *_top_tmp, *_stack_node;
    Two3NodePtr p,q;
    int target;

    CHECK_NULL_POINTER(t, "tt_delete");
    CHECK_NULL_POINTER(src, "tt_delete");

    _23_top = stack_new(NULL, adt_free);
    if(!tt_find_node(t->_root, src, _23_top, t->_cmp)) {
        DISPLAY_WARNING("the element is not in the 23-tree", "tt_delete");
        stack_free(&_23_top);
        return ;
    }
    _top = (struct _23_node_stack*)stack_top(_23_top);
    // find the node contains maximum element in the left/middle sub-tree of 'q'
    q = _top->node;
    if(t->_cmp(q->_dl, src) == 0) {
        p = q->_left;
        target = -1;
    }
    else {
        p = q->_middle;
        target = +1;
    }
    if(p) {
        while(TRUE) {
            NEW(_stack_node);
            _stack_node->node = p;
            _stack_node->direction = 0;
            stack_push(_23_top, _stack_node);
            if(p->_middle == NULL)
                break;
            if(p->_right)
                p = p->_right;
            else
                p = p->_middle;
        }
        if(target == -1) {
            t->_free(q->_dl);
            if(p->_dr) {
                q->_dl = p->_dr;
                p->_dr = NULL;
            }
            else {
                q->_dl = p->_dl;
                p->_dl = NULL;
            }
        }
        else {
            t->_free(q->_dr);
            if(p->_dr) {
                q->_dr = p->_dr;
                p->_dr = NULL;
            }
            else {
                q->_dr = p->_dl;
                p->_dl = NULL;
            }
        }
    }
    else {
        if(target == -1) {
            t->_free(q->_dl);
            q->_dl = q->_dr;
            q->_dr = NULL;
        }
        else {
            t->_free(q->_dr);
            q->_dr = NULL;
        }
    }
    // bottom-up check
    // 'p' now is the first node to be checked
    _top = (struct _23_node_stack*)stack_top(_23_top);
    p = _top->node;
    while(p!=t->_root && p->_dl==NULL) {
        stack_pop(_23_top);
        _top_tmp = (struct _23_node_stack*)stack_top(_23_top);
        tt_rotation(p, _top_tmp->node);
        p = _top_tmp->node;
    }
    if(p==t->_root && p->_dl==NULL) {
        t->_root = p->_left;
        FREE(p);
    }

    stack_free(&_23_top);
}

static void tt_for_each_inorder(Two3NodePtr root, void (*func)(void **elem, void *args), void *args)
{
    if(root) {
        if(root->_dl)
            func(&root->_dl, args);
        if(root->_dr)
            func(&root->_dr, args);
        tt_for_each_inorder(root->_left, func, args);
        tt_for_each_inorder(root->_middle, func, args);
        tt_for_each_inorder(root->_right, func, args);
    }
}
void tt_for_each(Two3TreePtr t, void (*func)(void **elem, void *args), void *args)
{
    CHECK_NULL_POINTER(t, "tt_for_each");
    tt_for_each_inorder(t->_root, func, args);
}

Two3NodePtr tt_root(Two3TreePtr t)
{
    CHECK_NULL_POINTER(t, "tt_root");
    return t->_root;
}

Two3NodePtr tt_left(Two3NodePtr node)
{
    CHECK_NULL_POINTER(node, "tt_left");
    return node->_left;
}

Two3NodePtr tt_middle(Two3NodePtr node)
{
    CHECK_NULL_POINTER(node, "tt_middle");
    return node->_middle;
}

Two3NodePtr tt_right(Two3NodePtr node)
{
    CHECK_NULL_POINTER(node, "tt_right");
    return node->_right;
}

void *tt_left_elem(Two3NodePtr node)
{
    CHECK_NULL_POINTER(node, "tt_left_elem");
    return node->_dl;
}

void *tt_right_elem(Two3NodePtr node)
{
    CHECK_NULL_POINTER(node, "tt_right_elem");
    return node->_dr;
}

static void tt_valid_each_node(Two3NodePtr node, BOOL *valid,
        void (*func)(const void*),
        int (*_cmp)(const void*, const void*))
{
    if(node) {
        if(node->_left && _cmp(node->_dl, node->_left->_dl)<0) {
            printf("  node->_dl(");
            func(node->_dl);
            printf(") < node->_left->_dl(");
            func(node->_left->_dl);
            printf(")\n");
            *valid = FALSE;
        }
        if(node->_middle && _cmp(node->_middle->_dl, node->_dl)<0) {
            printf("  node->_dl(");
            func(node->_dl);
            printf(") > node->_middle->_dl(");
            func(node->_middle->_dl);
            printf(")\n");
            *valid = FALSE;
        }
        if(node->_dr) {
            if(_cmp(node->_dr, node->_dl) < 0) {
                printf("  node->_dl(");
                func(node->_dl);
                printf(") > node->_dr(");
                func(node->_dr);
                printf(")\n");
                *valid = FALSE;
            }
            if(node->_middle && _cmp(node->_dr, node->_middle->_dl) < 0) {
                printf("  node->_dr(");
                func(node->_dr);
                printf(") < node->_middle->_dl(");
                func(node->_middle->_dl);
                printf(")\n");
                *valid = FALSE;
            }
            if(node->_right && _cmp(node->_right->_dl, node->_dr)<0) {
                printf("  node->_dr(");
                func(node->_dl);
                printf(") > node->_right->_dl(");
                func(node->_right->_dr);
                printf(")\n");
                *valid =FALSE;
            }
        }

        tt_valid_each_node(node->_left, valid, func, _cmp);
        tt_valid_each_node(node->_middle, valid, func, _cmp);
        tt_valid_each_node(node->_right, valid, func, _cmp);
    }
}
BOOL tt_valid(const Two3TreePtr t, void (*func)(const void*))
{
    BOOL valid;

    CHECK_NULL_POINTER(t, "tt_valid");

    valid = TRUE;
    tt_valid_each_node(t->_root, &valid, func, t->_cmp);
    return valid;
}

