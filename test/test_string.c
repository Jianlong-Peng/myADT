/*=============================================================================
#     FileName: test_string.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Created: 2015-06-07 21:17:15
#   LastChange: 2015-06-07 21:43:43
#      History:
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include "mstring.h"

#define MAX_S 127

int main(int argc, char *argv[])
{
    char s[MAX_S+1];
    char pattern[MAX_S+1];
    StringPtr sptr;
    size_t i;

    while(1) {
        do {
            printf("string (q to quit): ");
            scanf("%s", s);
        } while(strlen(s)==0);
        if(strlen(s)==1 && (s[0]=='q' || s[0]=='Q'))
            break;
        do {
            printf("pattern: ");
            scanf("%s", pattern);
        } while(strlen(pattern)==0);
        sptr = string_from_cstr(s);
        i = string_find_cstr(sptr, pattern);
        if(i == string_size(sptr))
            printf("pattern not found\n");
        else
            printf("pattern being found at %d\n", i);
        string_free(&sptr);
    }

    return 0;
}

