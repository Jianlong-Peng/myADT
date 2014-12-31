/*=============================================================================
#     FileName: mtools.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 15:33:28
#   LastChange: 2014-11-10 16:38:50
#      History: 
=============================================================================*/
#ifndef  MY_MTOOLS_H_INC
#define  MY_MTOOLS_H_INC

#include <stdio.h>
#include <stdlib.h>
#include "mconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

ADT_API void *adt_copy_char(const void *);
ADT_API void *adt_copy_int(const void *);
ADT_API void *adt_copy_float(const void *);
ADT_API void *adt_copy_double(const void *);
ADT_API void *adt_copy_long(const void *);

ADT_API void  adt_free(void *);

#define DECLEAR_ADT_COPY(type) \
    void *adt_copy_##type(const void *);

#define DEFINE_ADT_COPY(type) \
    void *adt_copy_##type(const void *data1) { \
        type *data2; \
        NEW(data2); \
        memcpy(data2, data1, sizeof(type)); \
        return (void*)data2; \
    }


/*
 * default copy and free function
 * - adt_copy_nothing:  {return src}
 * - adt_free_nothing:  {}
 * whenever NULL is passed to a constructor, they will be used instead!
 */
ADT_API void *adt_copy_nothing(const void *src);
ADT_API void  adt_free_nothing(void *src);


#define ADT_SWAP(d1, d2) \
do { \
    void *__temp = (d1); \
    (d1) = (d2); \
    (d2) = __temp; \
} while(0)


// macros for display warning or errors
#define DISPLAY_WARNING(reason, func_name) \
do { \
    printf("Warning: %s\n", reason); \
    printf("  raised in %s at %s:%d\n", func_name, __FILE__, __LINE__); \
} while(0)


#define DISPLAY_ERROR_EXIT(reason, func_name) \
do { \
    printf("Error: %s\n", reason); \
    printf("  raised in %s at %s:%d\n", func_name, __FILE__, __LINE__); \
    exit(EXIT_FAILURE); \
} while(0)


#define ERROR_NOT_IMPLEMENTED(func_name) DISPLAY_ERROR_EXIT("not implemented", func_name)

// macros for checking
#if defined(DO_CHECK) || defined(DEBUG)

#define CHECK_NULL_POINTER(p, func_name) \
do { \
    if((p) == NULL) \
        DISPLAY_ERROR_EXIT("NULL pointer given", func_name); \
} while(0)


#define CHECK_INDEX_OUT_OF_RANGE(expression, func_name) \
do { \
    if(expression) \
        DISPLAY_ERROR_EXIT("index out of range", func_name); \
} while(0)

#define CHECK_EMPTY(expression, func_name) \
do { \
    if(expression) \
        DISPLAY_ERROR_EXIT("content empty", func_name); \
} while(0)

#else

#define CHECK_NULL_POINTER(p, func_name)
#define CHECK_INDEX_OUT_OF_RANGE(expression, func_name)

#endif

/*
 * OBJ: macros for calling constructor for simple built-in types,
 *      e.g. char, int, float, double, long etc.
 * Usage:  to create a stack of integers
 *      StackPtr s = ADT_NEW_1(int, stack_new);
 * Attention:
 *   1. you can use 'DECLEAR_ADT_COPY' and 'DEFINE_ADT_FREE' to
 *      make your own copy function: adt_copy_##type.
 */
#define ADT_NEW_1(type, constructor) constructor(adt_copy_##type, adt_free)
#define ADT_NEWC_1(type, n, constructor) constructor(n, adt_copy_##type, adt_free)
#define ADT_NEW_2(type, constructor, func) constructor(adt_copy_##type, adt_free, func)
#define ADT_NEWC_2(type, n, constructor, func) constructor(n, adt_copy_##type, adt_free, func)


/* OBJ: macro for calling function 'void func(ADT_type p, void *data)' of simple built-in types,
 *      e.g. char, int, float, double, long etc.
 * Usage: (example)
 *   to create a min-max heap containing integers 0~9
 *   MinMaxHeapPtr h = ADT_NEW_1(int, mheap_new);
 *   ...
 *   for(int i=0; i<10; ++i)
 *       ADT_FUNC_1(int, mheap_insert, h, i);
 * Parameters:
 *   func: function to be called, func(ADT_TYPE *p, void *src)
 *   p   : pointer, *p is of ADT type
 *   data: should be simple built-in types, e.g. char, int, float, double, long, or literal.
 */
#define ADT_FUNC_1(type, func, p, data) \
do { \
    type __src = (data); \
    func(p, &__src); \
} while(0)

/* OBJ: macro for calling function 'void func(ADT_type p, void *src, another_arg)' of simple built-in types
 *      e.g. char, int, float, double, long etc.
 * Usage: (example)
 *
 * Paremeters:
 *   func: function to be called, func(ADT_TYPE *p, void *src)
 *   p   : pointer, *p is of ADT type
 *   data: should be simple built-in types, e.g. char, int, float, double, long, or literal.
 *   arg : addition argument passed to `func`
 */
#define ADT_FUNC_2(type, func, p, data, arg) \
do { \
    type __src = (data); \
    func(p, &__src, arg); \
} while(0)



/*
  // for gcc, the macro could return value returned by 'func'
  #define ADT_FUNC_1(type, func, p, data) \
  ({ \
      type __src = (data); \
      func(p, &__src); \
  })
*/
/*
 * OBJ: macro for calling function 'ReturnType func(ADT_Type p, void *data)' of simple built-in types
 *      e.g. char, int, float, double, long etc.
 * Usage: (example)
 *   AVLTreePtr at;
 *   AVLNodePtr retval;
 *   ...
 *   if(ADT_FUNC_RET_1(int, avl_has, at, 34, retval))
 *       // do something if 'at' contains 34...
 *   OR
 *   ADT_FUNC_RET(int, avl_has, at, 34, retval);
 *   // to do something with 'retval'
 */
#define ADT_FUNC_RET(type, func, p, data, retval) \
do { \
    type __src = (data); \
    retval = func(p, &__src); \
} while(0)

#define ADT_FUNC_RET_1(type, func, p, data, retval) (ADT_FUNC_RET(type, func, p, data, retval), retval)




/*
 * OBJ: macro for calling function which will return its element
 * Usage: (example)
 *   VectorPtr v = ADT_NEW_1(int, vector_new);
 *   for(i=0; i<n; ++i)
 *       ADT_FUNC_1(int, vector_push_back, v, i);
 *   for(i=0; i<n; ++i)
 *       printf(" %d", ADT_GET_2(int*, vector_get, v, i));
 *   printf("\n");
 */
#define ADT_GET_1(typeP, func, p) (typeP)func(p)

#define ADT_GET_2(typeP, func, p, arg) (typeP)func(p, arg)


#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_MTOOLS_H_INC  ----- */

