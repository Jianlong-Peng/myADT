/*=============================================================================
#     FileName: mstack.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 18:27:18
#   LastChange: 2014-11-10 16:46:54
#      History: 
=============================================================================*/
#include <stddef.h>
#include "mstack.h"
#include "memory.h"
#include "mtools.h"

struct StackNode
{
    void *_data;
    struct StackNode *_prev;
};

struct Stack
{
    int _size;
    void *(*_copy)(const void*);
    void (*_free)(void *);
    struct StackNode *_top;
};

StackPtr stack_new(void *(*copy_func)(const void *), void (*free_func)(void *))
{
    StackPtr s;
    NEW(s);
    s->_size = 0;
    s->_copy = (copy_func==NULL)?adt_copy_nothing:copy_func;
    s->_free = (free_func==NULL)?adt_free_nothing:free_func;
    s->_top = NULL;
    return s;
}

StackPtr stack_copy(const StackPtr s)
{
    StackPtr s2;
    struct StackNode *node1, *node2, *node3;
    
    CHECK_NULL_POINTER(s, "stack_copy");
    s2 = stack_new(s->_copy, s->_free);
    node3 = NULL;
    for(node1=s2->_top; node1; node1=node1->_prev) {
        NEW(node2);
        node2->_data = s2->_copy(node1->_data);
        node2->_prev = NULL;
        if(node3)
            node3->_prev = node2;
        else
            s2->_top = node2;
        node3 = node2;
        s2->_size++;
    }
    return s2;
}

void stack_clear(StackPtr s)
{
    if(s && s->_size) {
        struct StackNode *node;
        while(s->_top) {
            node = s->_top;
            s->_top = s->_top->_prev;
            s->_free(node->_data);
            FREE(node);
        }
        s->_size = 0;
    }
}

void stack_free(StackPtr *s)
{
    if(s && *s) {
        stack_clear(*s);
        FREE(*s);
    }
}

int stack_size(const StackPtr s)
{
    CHECK_NULL_POINTER(s, "stack_size");
    return s->_size;
}

BOOL stack_empty(const StackPtr s)
{
    CHECK_NULL_POINTER(s, "stack_empty");
    return (s->_size==0)?TRUE:FALSE;
}

void stack_push(StackPtr s, void *src)
{
    struct StackNode *node;
    
    CHECK_NULL_POINTER(s, "stack_push");
    NEW(node);
    node->_data = s->_copy(src);
    node->_prev = s->_top;
    s->_top = node;
    s->_size++;
}

void stack_pop(StackPtr s)
{
    struct StackNode *node;
    
    CHECK_NULL_POINTER(s, "stack_pop");
    if(s->_top) {
        node = s->_top;
        s->_top = s->_top->_prev;
        s->_free(node->_data);
        FREE(node);
        s->_size--;
    }
}

void *stack_top(StackPtr s)
{
    CHECK_NULL_POINTER(s, "stack_top");
    CHECK_EMPTY(stack_empty(s), "stack_top");
    return (stack_empty(s))?NULL:(s->_top->_data);
}

void stack_display(const StackPtr s, FILE *outf, void (*display_data)(FILE*, const void*))
{
    struct StackNode *node;

    CHECK_NULL_POINTER(s, "stack_display");

    if(stack_empty(s)) {
        fprintf(outf, "stack is empty!\n");
        return ;
    }
    
    fprintf(outf, "[");
    display_data(outf, s->_top->_data);
    fprintf(outf, "]");
    for(node=s->_top->_prev; node; node=node->_prev) {
        fprintf(outf, " -> [");
        display_data(outf, node->_data);
        fprintf(outf, "]");
    }
    fprintf(outf, "\n");
}

