/*=============================================================================
#     FileName: mdeque.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 13:28:53
#   LastChange: 2014-08-04 12:06:38
#      History: 
=============================================================================*/
#include "mdeque.h"
#include "mlist.h"

DequePtr deque_new(void *(*copy_func)(const void *), void (*free_func)(void *))
{
    return list_new(copy_func, free_func);
}

DequePtr deque_copy(const DequePtr p)
{
    return list_copy(p);
}

void deque_clear(DequePtr p)
{
    list_clear(p);
}

void deque_free(DequePtr *p)
{
    list_free(p);
}

int deque_size(const DequePtr p)
{
    return list_size(p);
}

BOOL deque_empty(const DequePtr p)
{
    return list_empty(p);
}

void deque_push_front(DequePtr p, void *src)
{
    list_push_front(p, src);
}

void deque_push_back(DequePtr p, void *src)
{
    list_push_back(p, src);
}

void deque_pop_front(DequePtr p)
{
    list_pop_front(p);
}

void deque_pop_back(DequePtr p)
{
    list_pop_back(p);
}

void *deque_front(DequePtr p)
{
    return list_value(list_begin(p));
}

void *deque_back(DequePtr p)
{
    return list_value(list_end(p));
}

/*
#include "memory.h"

struct Deque {
    int _size;
    void *(*_copy)(const void*);
    void (*_free)(void*);
    struct DequeNode
    {
        void *_data;
        struct DequeNode *_next;
        struct DequeNode *_prev;
    } *_first, *_last;
};

DequePtr deque_new(void *(*copy_func)(const void *), void (*free_func)(void *))
{
    DequePtr dp;
    NEW(dp);
    dp->_size = 0;
    dp->_copy = copy_func;
    dp->_free = free_func;
    dp->_first = dp->_last = NULL;
    return dp;
}

void deque_clear(DequePtr dp)
{
    if(dp && dq->_size) {
        struct DequeNode *node;
        while(dp->_first) {
            node = dp->_first;
            dp->_first = dp->_first->_next;
            if(dp->_free)
                dp->_free(node->_data);
            FREE(node);
        }
        dp->_first = dp->_last = NULL;
        dp->_size = 0;
    }
}
void deque_free(DequePtr dp)
{
    if(dp) {
        deque_clear(p);
        FREE(dp);
    }
}

DequePtr deque_copy(const DequePtr dp)
{
    DequePtr dp2;
    struct DequeNode *node1, *node2, *node3;
#if defined(DO_CHECK)
    CHECK_NULL_POINTER(dp, "deque_copy");
#endif
    dp2 = deque_new(dp->_copy, dp->_free);
    node2 = node3 = NULL;
    for(node1=dp->_first; node1; node1=node1->_next) {
        NEW(node2);
        if(dp2->_copy)
            node2->_data = dp2->_copy(node1->_data);
        else
            node2->_data = node1->_data;
        node2->_next = NULL;
        node2->_prev = NULL;
        if(node3) {
            node3->_next = node2;
            node2->_prev = node3;
        } else
            dp2->_first = node2;
        node3 = node2;
    }
    dp2->_last = node2;
    return dp2;
}

int deque_size(const DequePtr dp)
{
#if defined(DO_CHECK)
    CHECK_NULL_POINTER(dp, "deque_size");
#endif
    return dp->_size;
}

BOOL deque_empty(const DequePtr dp)
{
#if defined(DO_CHECK)
    CHECK_NULL_POINTER(dp, "deque_empty");
#endif
    return (dp->_size==0)?TRUE:FALSE;
}

void deque_push_front(DequePtr dp, void *src)
{
    struct DequeNode *node;
#if defined(DO_CHECK)
    CHECK_NULL_POINTER(dp, "deque_push_front");
#endif
    NEW(node);
    if(dp->_copy)
        node->_data = dp->_copy(src);
    else
        node->_data = src;
    node->_next = dp->_first;
    node->_prev = NULL;
    dp->_first = node;
    if(dp->_size++ == 0)
        dp->_last = node;
}

void deque_push_back(DequePtr dp, void *src)
{
    struct DequeNode *node;
#if defined(DO_CHECK)
    CHECK_NULL_POINTER(dp, "deque_push_back");
#endif
    NEW(node);
    if(dp->_copy)
        node->_data = dp->_copy(src);
    else
        node->_data = src;
    node->_prev = dp->_last;
    node->_next = NULL;
    dp->_last = node;
    if(dp->_size++ == 0)
        dp->_first = node;
}

void deque_pop_front(DequePtr dp)
{
    struct DequeNode *node;
    if(dp==NULL || dp->_size==0)
        return ;
    node = dp->_first;
    dp->_first = dp->_first->_next;
    if(dp->_free)
        dp->_free(node->_data);
    FREE(node);
    if(--(dp->_size))
        dp->_first->_prev = NULL;
    else
        dp->_last = NULL;
}

void deque_pop_back(DequePtr p)
{
    struct DequeNode *node;
    if(dp==NULL || dp->_size==0)
        return ;
    node = dp->_last;
    dp->_last = dp->_last->_prev;
    if(dp->_free)
        dp->_free(node->_data);
    FREE(node);
    if(--(dp->_size))
        dp->_last->_next = NULL;
    else
        dp->_first = NULL;
}

void *deque_front(DequePtr dp)
{
#if defined(DO_CHECK)
    CHECK_NULL_POINTER(dp, "deque_front");
#endif
    return (dp->_size==0)?NULL:(dp->_first->_data);
}

void *deque_back(DequePtr dp)
{
#if defined(DO_CHECK)
    CHECK_NULL_POINTER(dp, "deque_back");
#endif
    return (dp->_size==0)?NULL:(dp->_last->_data);
}
*/
