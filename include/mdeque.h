/*=============================================================================
#     FileName: mdeque.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 13:28:53
#   LastChange: 2014-08-04 12:04:23
#      History: 
=============================================================================*/
#ifndef  MY_DEQUE_H_INC
#define  MY_DEQUE_H_INC

#include "mconfig.h"
#include "mlist.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef ListPtr DequePtr;

DequePtr deque_new(void *(*copy_func)(const void *), void (*free_func)(void *));

DequePtr deque_copy(const DequePtr p);

void deque_clear(DequePtr p);

void deque_free(DequePtr *p);

int deque_size(const DequePtr p);
BOOL deque_empty(const DequePtr p);

void deque_push_front(DequePtr p, void *src);

void deque_push_back(DequePtr p, void *src);

void deque_pop_front(DequePtr p);
void deque_pop_back(DequePtr p);

void *deque_front(DequePtr p);
void *deque_back(DequePtr p);

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_DEQUE_H_INC  ----- */

