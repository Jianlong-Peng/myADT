/*=============================================================================
#     FileName: test_vector.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Created: 2014-11-06 09:05:13
#   LastChange: 2014-11-10 15:21:57
#      History:
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include "mvector.h"
#include "mtools.h"

void free_vector(void *data);
void display_vector(VectorPtr v);

int main(int argc, char *argv[])
{
    int m,n,i,j;
    VectorPtr v1,v2;

    if(argc != 3) {
        fprintf(stderr, "\n  Usage: %s m n\n"
                "  to create 2D matrix of m x n\n"
                "\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    m = atoi(argv[1]);
    n = atoi(argv[2]);

    printf("\nto create vector of %d x %d\n", m, n);
    //v1 = vector_new_n(m, NULL, free_vector);
    v1 = vector_new(NULL, free_vector);
    for(i=0; i<m; ++i) {
        v2 = ADT_NEWC_1(int, n, vector_new_n);
        for(j=0; j<n; ++j) {
            //VECTOR_SET(int, v2, i*m+j, j);
            ADT_FUNC_1(int, vector_push_back, v2, i*n+j);
        }
        //vector_set(v1, v2, i);
        vector_push_back(v1, v2);
    }

    printf("\nvector_size(v1)=%d, vector_full(v1)=%s\n", vector_size(v1), vector_full(v1)?"TRUE":"FALSE");
    printf("\nvector:\n");
    display_vector(v1);

    printf("\ndelete v1[0]\n");
    vector_pop_front(v1);
    if(vector_empty(v1))
        printf("\nvector now is empty!\n");
    else {
        printf("\nvector:\n");
        display_vector(v1);

        printf("\ndelete v1[-1]\n");
        vector_pop_back(v1);
        if(vector_empty(v1))
            printf("\nnow vector is empty!\n");
        else {
            printf("\nvector:\n");
            display_vector(v1);

            printf("\nnow the first row is:");
            //v2 = (VectorPtr)vector_front(v1);
            v2 = ADT_GET_1(VectorPtr, vector_front, v1);
            for(j=0; j<vector_size(v2); ++j)
                printf(" %d", *ADT_GET_2(int*, vector_get, v2, j));
            printf("\n");
        }
    }

    printf("\nto clear vector\n");
    vector_clear(v1);
    printf("\nvector_size(v1)=%d, vector_empty(v1)=%s\n", vector_size(v1), vector_empty(v1)?"TRUE":"FALSE");
    printf("\nto free vector\n");
    vector_free(&v1);
    printf("\n");

    return 0;
}

void free_vector(void *data)
{
    VectorPtr v = (VectorPtr)data;
    vector_free(&v);
}

void display_vector(VectorPtr v)
{
    VectorPtr v2;
    int i,j;

    for(i=0; i<vector_size(v); ++i) {
        v2 = ADT_GET_2(VectorPtr, vector_get, v, i);
        //v2 = (VectorPtr)vector_get(v, i);
        printf("%-2d:",i);
        for(j=0; j<vector_size(v2); ++j)
            printf(" %d", *ADT_GET_2(int*, vector_get, v2, j));
        printf("\n");
    }
}

