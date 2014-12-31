/*=============================================================================
#     FileName: mqueue.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 13:28:53
#   LastChange: 2014-08-04 12:05:00
#      History: 
=============================================================================*/
#ifndef  MY_QUEUE_H_INC
#define  MY_QUEUE_H_INC

#include "mconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Queue;
typedef struct Queue* QueuePtr;

QueuePtr queue_new(void *(*copy_func)(const void *), void (*free_func)(void *));

QueuePtr queue_copy(const QueuePtr p);

void queue_clear(QueuePtr p);

void queue_free(QueuePtr *p);

int queue_size(const QueuePtr p);
BOOL queue_empty(const QueuePtr p);

void queue_push(QueuePtr p, void *src);

void queue_pop(QueuePtr p);

void *queue_front(QueuePtr p);

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_QUEUE_H_INC  ----- */

