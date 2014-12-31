/*=============================================================================
#     FileName: mqueue.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 18:22:30
#   LastChange: 2014-11-05 15:00:16
#      History: 
=============================================================================*/
#include <stddef.h>
#include "mqueue.h"
#include "memory.h"
#include "mtools.h"

struct QueueNode
{
    void *_data;
    struct QueueNode *_next;
};

struct Queue
{
    int _size;
    void *(*_copy)(const void*);
    void (*_free)(void *);
    struct QueueNode *_head;
    struct QueueNode *_tail;
};

QueuePtr queue_new(void *(*copy_func)(const void *), void (*free_func)(void *))
{
    QueuePtr q;
    NEW(q);
    q->_size = 0;
    q->_copy = (copy_func==NULL)?adt_copy_nothing:copy_func;
    q->_free = (free_func==NULL)?adt_free_nothing:free_func;
    q->_head = q->_tail = NULL;
    return q;
}

QueuePtr queue_copy(const QueuePtr q)
{
    QueuePtr q2;
    struct QueueNode *node1, *node2, *node3;
    
    CHECK_NULL_POINTER(q, "queue_copy");
    q2 = queue_new(q->_copy, q->_free);
    node1 = q->_head;
    node3 = NULL;
    for(node1=q->_head; node1; node1=node1->_next) {
        NEW(node2);
        node2->_data = q2->_copy(node1->_data);
        node2->_next = NULL;
        if(node3)
            node3->_next = node2;
        else
            q2->_head = node2;
        node3 = node2;
        q2->_size++;
    }
    q->_tail = node3;
    return q2;
}

void queue_clear(QueuePtr q)
{
    if(q && q->_size) {
        struct QueueNode *node;
        while(q->_head) {
            node = q->_head;
            q->_head = q->_head->_next;
            q->_free(node->_data);
            FREE(node);
        }
        q->_size = 0;
        q->_head = q->_tail = NULL;
    }
}

void queue_free(QueuePtr *q)
{
    if(q && *q) {
        queue_clear(*q);
        FREE(*q);
    }
}

int queue_size(const QueuePtr q)
{
    CHECK_NULL_POINTER(q, "queue_size");
    return q->_size;
}

BOOL queue_empty(const QueuePtr q)
{
    CHECK_NULL_POINTER(q, "queue_empty");
    return (q->_size==0)?TRUE:FALSE;
}

void queue_push(QueuePtr q, void *src)
{
    struct QueueNode *node;
    
    CHECK_NULL_POINTER(q, "queue_push");
    NEW(node);
    node->_data = q->_copy(src);
    node->_next = NULL;
    if(q->_tail == NULL)
        q->_head = q->_tail = node;
    else {
        q->_tail->_next = node;
        q->_tail = node;
    }
    q->_size++;
}

void queue_pop(QueuePtr q)
{
    CHECK_NULL_POINTER(q, "queue_pop");
    if(q->_head) {
        struct QueueNode *node = q->_head;
        q->_head = q->_head->_next;
        q->_free(node->_data);
        FREE(node);
        q->_size--;
    }
}

void *queue_front(QueuePtr q)
{
    CHECK_NULL_POINTER(q, "queue_front");
    return (q->_size==0)?NULL:(q->_head->_data);
}

