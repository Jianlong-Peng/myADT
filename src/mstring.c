/*=============================================================================
#     FileName: mstring.c
#         Desc:
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-03 22:14:19
#   LastChange: 2015-06-07 21:50:12
#      History: 
#=============================================================================*/
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "memory.h"
#include "mstring.h"
#include "mtools.h"

struct String
{
    size_t _size;
    char *_s;     // no '\0' added
};

StringPtr string_new()
{
    StringPtr s;
    NEW(s);
    s->_size = 0;
    s->_s = NULL;
    return s;
}

StringPtr string_from_cstr(const char *s)
{
    StringPtr s2 = string_new();
    NEWC(s2->_s, strlen(s));
    memcpy(s2->_s, s, strlen(s));
    s2->_size = strlen(s);
    return s2;
}

StringPtr string_copy(const StringPtr s)
{
    StringPtr s2 = string_new();
    s2->_size = s->_size;
    NEWC(s2->_s, s->_size);
    memcpy(s2->_s, s->_s, s->_size);

    return s2;
}

void string_free(StringPtr *s)
{
    if(s) {
        if((*s)->_s)
            FREE((*s)->_s);
        FREE(*s);
    }
}

size_t string_size(const StringPtr s)
{
    return s->_size;
}

StringPtr string_concatenate(const StringPtr s1, const StringPtr s2)
{
    StringPtr s = string_new();
    s->_size = s1->_size + s2->_size;
    NEWC(s->_s, s->_size);
    memcpy(s->_s, s1->_s, s1->_size);
    memcpy(s->_s+s1->_size, s2->_s, s2->_size);
    return s;
}

StringPtr string_concatenate_cstr(const StringPtr s1, const char *s2)
{
    StringPtr s = string_new();
    size_t len2 = strlen(s2);
    s->_size = s1->_size + len2;
    NEWC(s->_s, s->_size);
    memcpy(s->_s, s1->_s, s1->_size);
    memcpy(s->_s+s1->_size, s2, len2);
    return s;
}

// you need to free the return value
char *string_to_cstr(const StringPtr s)
{
    char *s2;
    if(s->_size == 0)
        return NULL;
    NEWC(s2, s->_size+1);
    memcpy(s2, s->_s, s->_size);
    s2[s->_size] = '\0';
    return s2;
}

void string_expand(StringPtr s1, const StringPtr s2)
{
    RESIZE(s1->_s, s1->_size+s2->_size);
    memcpy(s1->_s+s1->_size, s2->_s, s2->_size);
    s1->_size += s2->_size;
}

void string_expand_cstr(StringPtr s1, const char *s2)
{
    size_t len2 = strlen(s2);
    RESIZE(s1->_s, s1->_size+len2);
    memcpy(s1->_s+s1->_size, s2, len2);
    s1->_size += len2;
}

size_t string_char(const StringPtr s, char c)
{
    size_t i;
    for(i=0; i<s->_size; ++i)
        if(s->_s[i] == c)
            return i;
    return i;
}

// KMP algorithm - O(m+n)
static int *calc_failure(const char *s, int n)
{
    int i,j;
    int *failure = (int*)malloc(sizeof(int)*n);
    failure[0] = -1;
    for(j=1; j<n; ++j) {
        i = failure[j-1];
        while((s[j]!=s[i+1]) && (i>=0))
            i = failure[i];
        if(s[j] == s[i+1])
            failure[j] = i+1;
        else
            failure[j] = -1;
    }
    return failure;
}
int *calc_failure_string(const StringPtr s1)
{
    return calc_failure(s1->_s, s1->_size);
}
int *calc_failure_cstr(const char *s)
{
    return calc_failure(s, strlen(s));
}
size_t string_find_cstr(const StringPtr s1, const char *s2)
{
    int *failure = calc_failure_cstr(s2);
    size_t val = string_find_cstr_v2(s1, s2, failure);
    free(failure);
    return val;
}
size_t string_find_cstr_v2(const StringPtr s1, const char *s2, const int *failure)
{
    size_t i=0, j=0;
    size_t lasts = s1->_size, lastp = strlen(s2);

    while(i<lasts && j<lastp) {
        if(s1->_s[i] == s2[j]) {
            ++i;
            ++j;
        } else {
            if(j == 0) ++i;
            else i = failure[j-1] + 1;
        }
    }
    return (j==lastp)?(i-lastp):lasts;
}

size_t string_find_v2(const StringPtr s1, const StringPtr s2, const int *failure)
{
    return string_find_cstr_v2(s1, s2->_s, failure);
}
size_t string_find(const StringPtr s1, const StringPtr s2)
{
    return string_find_cstr(s1, s2->_s);
}

/*
// O(m*n)
size_t string_find_cstr(const StringPtr s1, const char *s2)
{
    size_t i,j,start;
    size_t lasts = s1->_size, lastp = strlen(s2);

    for(i=0; i<lasts-lastp; ++i) {
        for(j=0,start=i; j<lastp && s1->_s[start]==s2[j]; ++j,++start)
            ;
        if(j == lastp)
            return i;
    }
    return lasts;
}

size_t string_find(const StringPtr s1, const StringPtr s2)
{
    return string_find_cstr(s1, s2->_s);
}
*/

char string_get(const StringPtr s, int i)
{
    int index = (i<0)?(i+s->_size):i;
    
    CHECK_NULL_POINTER(s, "string_get");
    CHECK_INDEX_OUT_OF_RANGE((index<0 || index>=s->_size), "string_get");
    return s->_s[index];
}

void string_set(StringPtr s, char c, int i)
{
    int index = (i<0)?(i+s->_size):i;
    
    CHECK_NULL_POINTER(s, "string_get");
    CHECK_INDEX_OUT_OF_RANGE((index<0 || index>=s->_size), "string_get");
    s->_s[index] = c;
}


static void free_string(void *src)
{
    StringPtr s = (StringPtr)src;
    FREE(s->_s);
    FREE(s);
}

VectorPtr string_split(const StringPtr s, char sep)
{
    VectorPtr vs = vector_new(NULL, free_string);
    StringPtr each;
    size_t beg, end;
    beg = 0;
    while(beg < s->_size) {
        while(beg<s->_size && s->_s[beg]==sep)
            ++beg;
        if(beg == s->_size)
            break;
        end = beg + 1;
        while(end<s->_size && s->_s[end]!=sep)
            ++end;
        each = string_new();
        each->_size = end - beg;
        NEWC(each->_s, each->_size);
        memcpy(each->_s, s->_s+beg, each->_size);
        vector_push_back(vs, each);
        beg = end + 1;
    }
    return vs;
}

void string_reverse(StringPtr s)
{
    char *q, *p;

    if(s->_size == 0)
        return;

    q = s->_s + (s->_size-1);
    p = s->_s;
    while(p < q) {
        char a = *p;
        *p++ = *q;
        *q-- = a;
    }
}

VectorPtr cstr_split(const char *s, char sep)
{
    VectorPtr vs = vector_new(NULL, adt_free);
    char *each;
    size_t len = strlen(s);
    size_t beg, end;
    beg = 0;
    while(beg < len) {
        while(beg<len && s[beg]==sep)
            ++beg;
        if(beg == len)
            break;
        end = beg + 1;
        while(end<len && s[end]!=sep)
            ++end;
        NEWC(each, end-beg+1);
        memcpy(each, s+beg, end-beg);
        each[end-beg] = '\0';
        vector_push_back(vs, each);
        beg = end + 1;
    }
    return vs;
}

char *cstr_strip(char *s)
{
    size_t beg, end;
    size_t len = strlen(s);
    char *new_s;
    
    beg = 0;
    while(beg<len && isspace(s[beg]))
        ++beg;
    if(beg == len)
        return NULL;
    
    end = len-1;
    while(end>0 && isspace(s[end]))
        --end;
    
    NEWC(new_s, end-beg+2);
    memcpy(new_s, s+beg, end-beg+1);
    new_s[end-beg+1] = '\0';
    
    return new_s;
}

void cstr_rstrip(char *s)
{
    size_t len = strlen(s);
    size_t end = len-1;
    
    while(end>0 && isspace(s[end]))
        --end;
    if(end==0 && isspace(s[end]))
        s[end] = '\0';
    else
        s[end+1] = '\0';
}
