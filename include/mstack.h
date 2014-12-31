/*=============================================================================
#     FileName: mstack.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 18:27:18
#   LastChange: 2014-11-10 15:11:16
#      History: 
=============================================================================*/
#ifndef  MY_STACK_H_INC
#define  MY_STACK_H_INC

#include <stdio.h>
#include "mconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Stack;
typedef struct Stack* StackPtr;

StackPtr stack_new(void *(*copy_func)(const void *), void (*free_func)(void *));

StackPtr stack_copy(const StackPtr p);

void stack_clear(StackPtr s);

void stack_free(StackPtr *s);

int stack_size(const StackPtr s);
BOOL stack_empty(const StackPtr s);

void stack_push(StackPtr s, void *src);

void stack_pop(StackPtr s);

void *stack_top(StackPtr s);

void stack_display(const StackPtr s, FILE *outf, void (*display_data)(FILE*, const void*));

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_STACK_H_INC  ----- */

