/*=============================================================================
#     FileName: test_stack.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Created: 2014-11-10 14:43:30
#   LastChange: 2014-11-10 16:37:01
#      History:
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mtools.h"
#include "mstack.h"
#include "memory.h"

static void print_int(FILE *outf, const void *src)
{
    int *data = (int*)src;
    fprintf(outf, "%d", *data);
}


int main(int argc, char *argv[])
{
    int n,i,j,temp;
    int *data;
    StackPtr sp;

    if(argc !=2 ) {
        fprintf(stderr, "\n  Usage: %s n\n"
                "\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);

    srand(time(NULL));
    
    sp = ADT_NEW_1(int, stack_new);
    
    NEWC(data, n);
    for(i=0; i<n; ++i) data[i] = i+1;
    for(i=0; i<n; ++i) {
        j = rand()%n;
        temp = data[i];
        data[i] = data[j];
        data[j] = temp;
    }
    printf("\nelements to be inserted: ");
    for(i=0; i<n; ++i)
        printf(" %d", data[i]);
    printf("\n");
    for(i=0; i<n; ++i)
        ADT_FUNC_1(int, stack_push, sp, data[i]);

    printf("\ntotally %d elements in the stack\n", stack_size(sp));
    printf("the top element: %d\n", *(int*)stack_top(sp));

    stack_display(sp, stdout, print_int);

    printf("\ndelete the 1st element\n");
    stack_pop(sp);
    printf("now, the top element: %d\n", *(int*)stack_top(sp));
    stack_display(sp, stdout, print_int);

    printf("\n");

    stack_free(&sp);
    FREE(data);

    return 0;
    
}

