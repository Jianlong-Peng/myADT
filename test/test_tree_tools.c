/*=============================================================================
#     FileName: test_tree_tools.c
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-08-08 17:40:18
#   LastChange: 2014-11-10 16:04:30
#      History: 
#=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "mavl_tree.h"
#include "m23_tree.h"
#include "mrb_tree.h"
#include "tree_tools.h"
#include "mtools.h"
#include "memory.h"

int cmp_int(const void *v1, const void *v2)
{
    const int *d1 = (const int*)v1;
    const int *d2 = (const int*)v2;

    if(*d1 == *d2)
        return 0;
    else if(*d1 < *d2)
        return -1;
    else
        return +1;
}

void test_avl_tree(int *data, int n, BOOL detail, const char *basename);
void test_23_tree(int *data, int n, BOOL detail, const char *basename);
void test_234_tree(int *data, int n, BOOL detail, const char *basename);
void test_rb_tree(int *data, int n, BOOL detail, const char *basename);

//#define AVL_DEBUG
//#define TT_DEBUG
//#define RB_DEBUG

int main(int argc, char *argv[])
{
    int i, j, n, type, temp;
    int *data;
    char *basename;
    BOOL detail;
    typedef void (*func_type)(int *, int, BOOL, const char*);
    //func_type test_functions[] = {test_avl_tree, test_23_tree, test_234_tree, test_rb_tree};

    if(argc!=6 && argc!=7) {
        fprintf(stderr, "\n    OBJ: to test tree structures by printing the tree\n"
            "\n  Usage: %s [options] n\n"
            "\n  [options]\n"
            "  --type t\n"
            "    to specify the tree type, possible values\n"
            "    1 - AVL-tree\n"
            "    2 - 23-tree\n"
            "    3 - 234-tree\n"
            "    4 - red-black-tree\n"
            "  --out name\n"
            "    to save tree in file named {name}.dot\n"
            "  --detail <optional>\n"
            "    if given, each time an element is inserted, the tree will be saved\n"
            "    additional files named {name}_i.dot will be saved\n"
            "\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    detail = FALSE;
    type = -1;
    basename = NULL;
    for(i=1; i<argc; ++i) {
        if(argv[i][0] != '-')
            break;
        if(strcmp(argv[i], "--type") == 0)
            type = atoi(argv[++i]);
        else if(strcmp(argv[i], "--detail") == 0)
            detail = TRUE;
        else if(strcmp(argv[i], "--out") == 0)
            basename = argv[++i];
        else {
            fprintf(stderr, "Error: invalid option %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }
    }
    if(argc-i != 1) {
        fprintf(stderr, "Error: invalid number of arguments\n");
        exit(EXIT_FAILURE);
    }
    n = atoi(argv[i]);
    if(type == -1) {
        fprintf(stderr, "Error: '--type t' is needed!\n");
        exit(EXIT_FAILURE);
    }
    if(type<1 || type>4) {
        fprintf(stderr, "Error: '--type %d' is invalid\n", type);
        exit(EXIT_FAILURE);
    }
    if(basename == NULL) {
        fprintf(stderr, "Error: '--out' is needed\n");
        exit(EXIT_FAILURE);
    }
    
    srand(time(NULL));

#if defined(AVL_DEBUG)
    NEWC(data, 27);
    data[0]=1; data[1]=9; data[2]=10; data[3]=26; data[4]=22; data[5]=2;
    data[6]=8; data[7]=3; data[8]=27; data[9]=23; data[10]=25; data[11]=21;
    data[12]=7; data[13]=14; data[14]=17; data[15]=12; data[16]=5; data[17]=19;
    data[18]=11; data[19]=24; /*data[20]=15; data[21]=4; data[22]=6; data[23]=13;
    data[24]=16; data[25]=18; data[26]=20;*/
    n = 20;
#elif defined(TT_DEBUG)
    NEWC(data, 10);
    data[0]=6; data[1]=2; data[2]=9; data[3]=1; data[4]=5; data[5]=3;
    data[6]=8; data[7]=4; data[8]=7; data[9]=10;
    n = 10;
#elif defined(RB_DEBUG)
    NEWC(data, 10);
    data[0]=9; data[1]=3; data[2]=4; data[3]=8; data[4]=7;
    data[5]=10; data[6]=1; data[7]=6; data[8]=5; data[9]=2;
    n = 10;
#else
    NEWC(data, n);
    for(i=0; i<n; ++i) data[i] = i+1;
    for(i=0; i<n; ++i) {
        j = rand()%n;
        temp = data[i];
        data[i] = data[j];
        data[j] = temp;
    }
#endif

    //for testing 5 7 3 9 8 6 4 10 2 1
    //data[0]=5; data[1]=7; data[2]=3; data[3]=9; data[4]=8; data[5]=6;
    //data[6]=4; data[7]=10; data[8]=2; data[9]=1;
    
    //test_functions[n-1](data, n, detail);
    switch(type) {
    case 1: test_avl_tree(data, n, detail, basename); break;
    case 2: test_23_tree(data, n, detail, basename); break;
    case 3: test_234_tree(data, n, detail, basename); break;
    case 4: test_rb_tree(data, n, detail, basename); break;
    default: fprintf(stderr, "Error: invalid type %d\n", type);
    }

    FREE(data);

    return 0;
}

static void display_int_elem(const void *data)
{
    printf("%d", *(int*)data);
}
/*
static void print_int_avl(const void *data, FILE *outf)
{
    AVLNodePtr node = (AVLNodePtr)data;
    void *_data = avl_data(node);
    fprintf(outf, "%d", *(int*)_data);
}
static void display_avl_node(void **elem, void *args)
{
    int **v = (int**)elem;
    printf("%d\n", **v);
}
*/
static void show_avl_int(FILE *outf, const void *data)
{
    AVLNodePtr node = (AVLNodePtr)data;
    //int _data = *(int*)avl_data(node);
    if(node)
        fprintf(outf, "%d", *(int*)avl_data(node));
    else
        fprintf(outf, "NULL");
}
static void show_rb_int(FILE *outf, const void *data)
{
    RBNodePtr node = (RBNodePtr)data;
    //int _data = *(int*)rb_data(node);
    if(node)
        fprintf(outf, "%d", *(int*)rb_data(node));
    else
        fprintf(outf, "NULL");
}
static void show_23_int(FILE* outf, const void *data)
{
    Two3NodePtr node = (Two3NodePtr)data;
    void *dl = tt_left_elem(node);
    void *dr = tt_right_elem(node);
    if(dl)
        fprintf(outf, "%d", *(int*)dl);
    else
        fprintf(outf, "NULL");
    if(dr)
        fprintf(outf, "|%d", *(int*)dr);
    else
        fprintf(outf, "|NULL");
}

void test_avl_tree(int *data, int n, BOOL detail, const char *basename)
{
    AVLTreePtr t;
    int i, height;
    char *outname;
    FILE *outf;
    size_t len;
    BOOL valid;
    int query;
    AVLNodePtr retval;

    len = strlen(basename) + 30;
    NEWC(outname, len);

    fprintf(stdout, "\ntesting AVL-tree...\n");
    t = ADT_NEW_2(int, avl_new, cmp_int);

    fprintf(stdout, "\nelements to be inserted: ");

    for(i=0; i<n; ++i) {
        fprintf(stdout, " %d", data[i]);
        ADT_FUNC_1(int, avl_insert, t, data[i]);
        if(detail) {
            sprintf(outname, "%s_%d.dot", basename, i+1);
            outf = fopen(outname,"w");
            if(outf) {
                avl_tree_to_dot(t, outf, show_avl_int);
                fclose(outf);
            } else
                fprintf(stderr, "Error: failed to open %s\n", outname);
        }
#ifdef AVL_DEBUG
        avl_valid(t, display_int_elem);
#endif
    }
    fprintf(stdout, "\n\n");


    height = avl_tree_height(t);
    fprintf(stdout, "tree height: %d\n", height);

    valid = avl_valid(t, display_int_elem);
    fprintf(stdout, "if tree is valid? %s\n\n", (valid==TRUE)?"TRUE":"FALSE");

    //avl_tree_display(t, stderr, print_int_avl);
    sprintf(outname, "%s.dot", basename);
    outf = fopen(outname, "w");
    if(outf) {
        avl_tree_to_dot(t, outf, show_avl_int);
        fclose(outf);
    } else
        fprintf(stderr, "Error: failed to open %s\n", outname);

    while(1) {
        printf("please input a query number (-1 to quit): ");
        scanf("%d", &query);
        if(query == -1)
            break;
        ADT_FUNC_RET(int, avl_has, t, query, retval);
        //retval = avl_has(t, &query);
        if(retval == NULL)
            printf("%d not in the tree!\n", query);
        else
            printf("%d is in the tree!\n", *(int*)avl_data(retval));
    }

    fprintf(stdout, "\nto free AVL-tree\n\n");
    avl_free(&t);
}

void test_23_tree(int *data, int n, BOOL detail, const char *basename)
{
    Two3TreePtr t;
    int i, height;
    char *outname;
    FILE *outf;
    size_t len;
    BOOL valid;
    int query;
    Two3NodePtr retval;

    len = strlen(basename) + 30;
    NEWC(outname, len);

    fprintf(stdout, "\ntesting 23-tree...\n");
    t = ADT_NEW_2(int, tt_new, cmp_int);

    fprintf(stdout, "\nelements to be inserted: ");

    for(i=0; i<n; ++i) {
        fprintf(stdout, " %d", data[i]);
        ADT_FUNC_1(int, tt_insert, t, data[i]);
        if(detail) {
            sprintf(outname, "%s_%d.dot", basename, i+1);
            outf = fopen(outname,"w");
            if(outf) {
                tt_tree_to_dot(t, outf, show_23_int);
                fclose(outf);
            } else
                fprintf(stdout, "Error: failed to open %s\n", outname);
        }
    }
    fprintf(stdout, "\n\n");


    height = tt_tree_height(t);
    fprintf(stdout, "tree height: %d\n\n", height);

    valid = tt_valid(t, display_int_elem);
    fprintf(stdout, "if tree is valid? %s\n\n", (valid==TRUE)?"TRUE":"FALSE");

    sprintf(outname, "%s.dot", basename);
    outf = fopen(outname, "w");
    if(outf) {
        tt_tree_to_dot(t, outf, show_23_int);
        fclose(outf);
    } else
        fprintf(stdout, "Error: failed to open %s\n", outname);

    while(1) {
        printf("please input a query number (-1 to quit): ");
        scanf("%d", &query);
        if(query == -1)
            break;
        ADT_FUNC_RET(int, tt_has, t, query, retval);
        //retval = avl_has(t, &query);
        if(retval == NULL)
            printf("%d not in the tree!\n", query);
        else {
            printf("%d is in the tree!\n", query);
            if(tt_left_elem(retval))
                printf("left element: %d\n", *(int*)tt_left_elem(retval));
            if(tt_right_elem(retval))
                printf("right element: %d\n", *(int*)tt_right_elem(retval));
        }
    }

    printf("\nto free tree!\n\n");
    tt_free(&t);
}

void test_234_tree(int *data, int n, BOOL detail, const char *basename)
{
    fprintf(stderr, "Sorry, but 'test_234_tree' is not yet implemented\n");
}

void test_rb_tree(int *data, int n, BOOL detail, const char *basename)
{
    RBTreePtr t;
    int i, height;
    char *outname;
    FILE *outf;
    size_t len;
    BOOL valid;
    int query;
    RBNodePtr retval;

    len = strlen(basename) + 30;
    NEWC(outname, len);

    fprintf(stdout, "\ntesting Red-Black tree...\n");
    t = ADT_NEW_2(int, rb_new, cmp_int);

    fprintf(stdout, "\nelements to be inserted: ");

    for(i=0; i<n; ++i) {
        fprintf(stdout, " %d", data[i]);
        ADT_FUNC_1(int, rb_insert, t, data[i]);
        //printf("root: %d:%s\n", *(int*)rb_data(rb_root(t)), rb_node_color(rb_root(t))==RED?"RED":"BLACK");
        if(detail) {
            sprintf(outname, "%s_%d.dot", basename, i+1);
            outf = fopen(outname,"w");
            if(outf) {
                rb_tree_to_dot(t, outf, show_rb_int);
                fclose(outf);
            } else
                fprintf(stderr, "Error: failed to open %s\n", outname);
        }
    }
    fprintf(stdout, "\n\n");


    height = rb_tree_height(t);
    fprintf(stdout, "tree height: %d\n", height);
    //printf("root: %d, color=%s\n", *(int*)rb_data(rb_root(t)), (rb_node_color(rb_root(t))==RED)?"RED":"BLACK");
    
    valid = rb_valid(t, display_int_elem);
    fprintf(stdout, "if tree is valid? %s\n\n", (valid==TRUE)?("TRUE"):("FALSE"));

    sprintf(outname, "%s.dot", basename);
    outf = fopen(outname, "w");
    if(outf) {
        rb_tree_to_dot(t, outf, show_rb_int);
        fclose(outf);
    } else
        fprintf(stderr, "Error: failed to open %s\n", outname);
    
    while(1) {
        printf("please input a query number (-1 to quit): ");
        scanf("%d", &query);
        if(query == -1)
            break;
        else
            ADT_FUNC_RET(int, rb_has, t, query, retval);
            //retval = rb_has(t, &query);
        if(retval == NULL)
            printf("%d not in the tree!\n", query);
        else
            printf("%d is in the tree!\n", *(int*)rb_data(retval));
    }

    printf("\nto free Red-Black tree\n\n");
    rb_free(&t);
}

