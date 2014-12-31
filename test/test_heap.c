/*=============================================================================
#     FileName: test_heap.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-08 22:55:54
#   LastChange: 2014-11-10 16:37:17
#      History: 
#=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mheap.h"
#include "mtools.h"
#include "memory.h"

static void print_int(FILE *outf, const void *src)
{
    int *data = (int*)src;
    fprintf(outf, "%d", *data);
}

static void print_data(const void *src)
{
    printf("%d", *(int*)src);
}

static int cmp_int(const void *src1, const void *src2)
{
    const int *v1 = (const int*)src1;
    const int *v2 = (const int*)src2;
    if(*v1 == *v2)
        return 0;
    else if(*v1 < *v2)
        return -1;
    else
        return +1;
}

int main(int argc, char *argv[])
{
    int n,i,j,temp;
    int *data;
    char *basename, *outname;
    FILE *outf;
    size_t len;
    BOOL verbose, valid;
    HeapPtr hp;

    if(argc!=3 && argc!=4) {
        fprintf(stderr, "\n  Usage: %s [--verbose] out_basename n\n"
                "  --verbose: if given, .dot file will be generated after inserting each element\n"
                "\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    
    verbose = FALSE;
    if(argc == 4) {
        verbose = TRUE;
        n = atoi(argv[3]);
        basename = argv[2];
    }
    else {
        verbose = FALSE;
        n = atoi(argv[2]);
        basename = argv[1];
    }
    len = strlen(basename) + 30;
    NEWC(outname, len);
    
    hp = ADT_NEW_2(int, heap_new, cmp_int);
    
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
    for(i=0; i<n; ++i) {
        ADT_FUNC_1(int, heap_insert, hp, data[i]);
        if(verbose) {
            sprintf(outname, "%s_%d.dot", basename, i+1);
            outf = fopen(outname,"w");
            if(outf) {
                heap_to_dot(hp, outf, print_int);
                fclose(outf);
            }
            else
                fprintf(stderr, "Error: failed to open %s to output .dot\n", outname);
        }
    }

    valid = heap_valid(hp, print_data);
    printf("\nis heap valid? %s\n", valid?"TRUE":"FALSE");

    printf("\nthe top element: %d\n", *(int*)heap_top(hp));

    heap_display(hp, stdout, print_int);

    sprintf(outname, "%s.dot", basename);
    outf = fopen(outname, "w");
    if(outf) {
        heap_to_dot(hp, outf, print_int);
        fclose(outf);
    }
    else
        fprintf(stderr, "Error: failed to open %s\n", outname);

    printf("\ndelete the 1st element\n");
    heap_pop(hp);
    valid = heap_valid(hp, print_data);
    printf("\nis heap valid? %s\n", valid?"TRUE":"FALSE");
    printf("now, the top element: %d\n", *(int*)heap_top(hp));
    heap_display(hp, stdout, print_int);
    printf("\n");

    heap_free(&hp);
    FREE(data);
    FREE(outname);

    return 0;
}
