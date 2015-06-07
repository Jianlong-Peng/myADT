/*=============================================================================
#     FileName: mstring.h
#         Desc:
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-03 22:02:45
#   LastChange: 2015-06-07 21:08:57
#      History: 
#=============================================================================*/
#ifndef  MY_MSTRING_H_INC
#define  MY_MSTRING_H_INC

#include <stddef.h>
#include "mvector.h"

/*
 * Attention:
 *   using size_t to record the size of string??
 *   if so, index cannot be negative!!
 */

#ifdef __cplusplus
extern "C" {
#endif

struct String;
typedef struct String* StringPtr;

StringPtr string_new();

StringPtr string_from_cstr(const char *s);

StringPtr string_copy(const StringPtr s);

void string_free(StringPtr *s);

size_t string_size(const StringPtr s);

StringPtr string_concatenate(const StringPtr s1, const StringPtr s2);
StringPtr string_concatenate_cstr(const StringPtr s1, const char *s2);

// you need to free the return value
char *string_to_cstr(const StringPtr s);

void string_expand(StringPtr s1, const StringPtr s2);
void string_expand_cstr(StringPtr s1, const char *s2);

// if not found, return string_size(s)
size_t string_char(const StringPtr s, char c);
int *calc_failure(const char *s, int n);
size_t string_find(const StringPtr s1, const StringPtr s2);
size_t string_find_v2(const StringPtr s1, const StringPtr s2, const int *failure);
size_t string_find_cstr(const StringPtr s1, const char *s2);
size_t string_find_cstr_v2(const StringPtr s1, const char *s2, const int *failure);

char string_get(const StringPtr s, int i);
void string_set(StringPtr s, char c, int i);

// return Vector of String
VectorPtr string_split(const StringPtr s, char sep);

void string_reverse(StringPtr s);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// the following function is for c-style string

// return Vector os c-style strings
VectorPtr cstr_split(const char *s, char sep);

// return a c-style string by removing the blank characters in the begining or end of 's'
char *cstr_strip(char *s);

// the blank characters in the end of 's' will be ignored.
// 's' will be changed!!!!
void cstr_rstrip(char *s);

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_MSTRING_H_INC  ----- */

