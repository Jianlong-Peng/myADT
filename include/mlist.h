/*=============================================================================
#     FileName: mlist.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-27 16:48:05
#   LastChange: 2014-11-05 14:00:10
#      History:
=============================================================================*/
#ifndef  MY_LIST_H_INC
#define  MY_LIST_H_INC

#include "mconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ListNode;
typedef struct ListNode* ListIter;

struct List;
typedef struct List* ListPtr;

ListPtr list_new(void *(*copy_func)(const void*), void (*free_func)(void *));

ListPtr list_copy(const ListPtr l);

void list_clear(ListPtr l);

void list_free(ListPtr *l);

int list_size(const ListPtr l);
BOOL list_empty(const ListPtr l);

ListIter list_begin(ListPtr l);
ListIter list_next(ListIter iter);
ListIter list_prev(ListIter iter);
ListIter list_end(ListPtr l);
void *list_value(ListIter iter);

void list_push_back(ListPtr l, void *src);

void list_push_front(ListPtr l, void *src);

// to insert 'src' after 'iter'
void list_insert(ListPtr l, void *src, ListIter iter);
#define LIST_INSERT(type, l, data, iter) \
do { \
    type __src = data; \
    list_insert(l, &__src, iter); \
} while(0)

void list_pop_back(ListPtr l);
void list_pop_front(ListPtr l);

// to delete 'iter' from list
void list_delete(ListPtr l, ListIter iter);

// _cmp(v1, v2) < 0 ==> v1 has priority over v2
void list_sort(ListPtr l, int (*_cmp)(const void*, const void*));

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_LIST_H_INC  ----- */

