/*=============================================================================
#     FileName: mlist.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-27 16:54:38
#   LastChange: 2014-11-05 13:59:53
#      History:
=============================================================================*/
#include <stddef.h>
#include "memory.h"
#include "mlist.h"
#include "mtools.h"

struct ListNode
{
    void *_data;
    struct ListNode *_next;
    struct ListNode *_prev;
};

struct List
{
    int _size;
    void *(*_copy)(const void*);
    void (*_free)(void *);
    struct ListNode *_head;
    struct ListNode *_tail;
};

ListPtr list_new(void *(*copy_func)(const void*), void (*free_func)(void *))
{
    ListPtr l;
    NEW(l);
    l->_size = 0;
    l->_copy = (copy_func==NULL)?adt_copy_nothing:copy_func;
    l->_free = (free_func==NULL)?adt_free_nothing:free_func;
    l->_head = l->_tail = NULL;
    return l;
}

ListPtr list_copy(const ListPtr l)
{
    ListPtr l2;
    struct ListNode *node1, *node2, *node3;
    
    CHECK_NULL_POINTER(l, "list_copy");
    l2 = list_new(l->_copy, l->_free);
    node2 = node3 = NULL;
    for(node1=l->_head; node1; node1=node1->_next) {
        NEW(node2);
        node2->_data = l2->_copy(node1->_data);
        node2->_next = NULL;
        node2->_prev = NULL;
        if(node3) {
            node3->_next = node2;
            node2->_prev = node3;
        } else
            l2->_head = node2;
        node3 = node2;
    }
    l2->_tail = node2;
    return l2;
}

void list_clear(ListPtr l)
{
    if(l && l->_size) {
        ListIter p;
        while(l->_head) {
            p = l->_head;
            l->_head = l->_head->_next;
            l->_free(p->_data);
            FREE(p);
        }
        l->_head = l->_tail = NULL;
        l->_size = 0;
    }
}
void list_free(ListPtr *l)
{
    if(l) {
        list_clear(*l);
        FREE(*l);
    }
}

int list_size(const ListPtr l)
{
    CHECK_NULL_POINTER(l, "list_size");
    return l->_size;
}

BOOL list_empty(const ListPtr l)
{
    CHECK_NULL_POINTER(l, "list_empty");
    return (l->_size==0)?TRUE:FALSE;
}

ListIter list_begin(ListPtr l)
{
    CHECK_NULL_POINTER(l, "list_begin");
    return l->_head;
}

ListIter list_next(ListIter iter)
{
    CHECK_NULL_POINTER(iter, "list_next");
    return iter->_next;
}

ListIter list_prev(ListIter iter)
{
    CHECK_NULL_POINTER(iter, "list_prev");
    return iter->_prev;
}

ListIter list_end(ListPtr l)
{
    CHECK_NULL_POINTER(l, "list_end");
    return l->_tail;
}

void *list_value(ListIter iter)
{
    CHECK_NULL_POINTER(iter, "list_value");
    return iter->_data;
}

static ListIter list_create_node(void *src, void *(*_copy)(const void*))
{
    ListIter node;
    NEW(node);
    node->_data = _copy(src);
    node->_next = NULL;
    node->_prev = NULL;
    return node;
}
void list_push_back(ListPtr l, void *src)
{
    ListIter node;
    
    CHECK_NULL_POINTER(l, "list_push_back");
    node = list_create_node(src, l->_copy);
    if(l->_head == NULL)
        l->_head = l->_tail = node;
    else {
        l->_tail->_next = node;
        node->_prev = l->_tail;
        l->_tail = node;
    }
    l->_size++;
}

void list_push_front(ListPtr l, void *src)
{
    ListIter node;
    
    CHECK_NULL_POINTER(l, "list_push_front");
    node = list_create_node(src, l->_copy);
    if(l->_head == NULL)
        l->_head = l->_tail = node;
    else {
        node->_next = l->_head;
        l->_head->_prev = node;
        l->_head = node;
    }
    l->_size++;
}

// to insert 'src' after 'iter'
void list_insert(ListPtr l, void *src, ListIter iter)
{
    ListIter node;
    
    CHECK_NULL_POINTER(l, "list_insert");
    CHECK_NULL_POINTER(iter, "list_insert");
    node = list_create_node(src, l->_copy);
    node->_next = iter->_next;
    if(iter->_next)
        iter->_next->_prev = node;
    iter->_next = node;
    node->_prev = iter;
    l->_size++;
}

void list_pop_back(ListPtr l)
{
    ListIter node;

    CHECK_NULL_POINTER(l, "list_pop_back");
    if(l->_tail==NULL)
        return ;
    node = l->_tail->_prev;
    l->_free(l->_tail->_data);
    FREE(l->_tail);
    l->_tail = node;
    if(l->_tail)
        l->_tail->_next = NULL;
    l->_size--;
}

void list_pop_front(ListPtr l)
{
    ListIter node;

    CHECK_NULL_POINTER(l, "list_pop_front");
    if(l->_head==NULL)
        return ;
    node = l->_head->_next;
    l->_free(l->_head->_data);
    FREE(l->_head);
    l->_head = node;
    if(l->_head)
        l->_head->_prev = NULL;
    l->_size--;
}
// to delete 'iter'
void list_delete(ListPtr l, ListIter iter)
{
    CHECK_NULL_POINTER(l, "list_delete");
    CHECK_NULL_POINTER(iter, "list_delete");

    if(iter->_prev)
        iter->_prev->_next = iter->_next;
    if(iter->_next)
        iter->_next->_prev = iter->_prev;
    l->_free(iter->_data);
    FREE(iter);
    l->_size--;
}

void list_sort(ListPtr l, int (*_cmp)(const void*, const void*))
{
    ListIter p,q,tmp;
    CHECK_NULL_POINTER(l, "list_sort");
    if(l->_size == 0)
        return ;
    p = l->_head->_next;
    while(p) {
        for(q=p->_prev; q && _cmp(p->_data, q->_data)<0; q=q->_prev)
            ;
        // p is already in the correct order
        if(q == p->_prev) {
            p = p->_next;
            continue;
        }
        // otherwise, move p to be the next node of q if q is not NULL
        // or move p as header
        tmp = p->_next;
        p->_prev->_next = p->_next;
        if(p->_next)
            p->_next->_prev = p->_prev;
        else
            l->_tail = p->_prev;
        if(q) {
            q->_next->_prev = p;
            p->_next = q->_next;
            q->_next = p;
            p->_prev = q;
        }
        else {
            p->_next = l->_head;
            p->_prev = NULL;
            l->_head->_prev = p;
            l->_head = p;
        }
        p = tmp;
    }
}

