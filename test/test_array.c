/*=============================================================================
#     FileName: test_array.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-30 16:04:07
#   LastChange: 2014-11-10 16:02:04
#      History: 
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include "marray.h"
#include "mtools.h"

void display_array(ArrayPtr a);

int main(int argc, char *argv[])
{
    int i, n;
	//double b,c;
    ArrayPtr a;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);
    a = ADT_NEWC_1(int, n, array_new_n);
    printf("array empty: %s\n", array_empty(a)?"TRUE":"FALSE");
    printf("array size: %d\n", array_size(a));
    printf("array capacity: %d\n", array_capacity(a));

    for(i=0; i<n; ++i)
        ADT_FUNC_1(int, array_push_back, a, 2*i+1);
    printf("\nafter adding elements\n");
    printf("array size: %d\n", array_size(a));
    display_array(a);

    printf("\nadd 20 more elements\n");
    for(i=20; i<40; ++i)
        ADT_FUNC_1(int, array_push_back, a, i);
    printf("array size: %d\n", array_size(a));
    printf("array capacity: %d\n", array_capacity(a));
    display_array(a);

    printf("\nthe last element is %d\n", *(int*)array_back(a));
    printf("\ndelete the last element\n");
    array_pop_back(a);
    printf("now the last element is %d\n", *(int*)array_back(a));

    printf("\nchange the 1st element to 100\n");
    ADT_FUNC_2(int, array_set, a, 100, 1);
    //ARRAY_SET(int, a, 100, 1);
    printf("swap the 2nd and 5th element\n");
    array_swap(a, 1, 4);
    printf("\nNow the array is\n");
    display_array(a);

    printf("\nto free array\n");
    array_free(&a);

    return 0;
}

void display_array(ArrayPtr a)
{
    int i;
    for(i=0; i<array_size(a); ++i) {
        printf("%d ", *(int*)array_get(a, i));
        if((i+1)%10 == 0)
            printf("\n");
    }
    if((i+1)%10)
        printf("\n");
}

