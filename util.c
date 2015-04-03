#include <stdio.h>
#include <stdlib.h>
#include "mylex.h"

void *myalloc(size_t size)
{
    return malloc(size);
}

void myfree(void *ptr)
{
    free(ptr);
}

int **myalloc_matrix(size_t first_size, size_t second_size)
{
    int **ptr = (int **)myalloc(first_size * sizeof(int *));
    int i = 0;
    for(; i < first_size; i++){
        ptr[i] = (int *)myalloc(second_size * sizeof(int));
    }
    return ptr;
}

void myfree_matrix(int **ptr, int size)
{
    int i = 0;
    for (; i < size; i++){
        myfree((void *)ptr[i]);
    }
    myfree((void *)ptr);
}

void init_array(int *array, int num, int size)
{
    int i;
    for (i = 0; i < size; i++){
        array[i] = num;
    }
}

void init_matrix(int **matrix, int num, int x_size, int y_size)
{
    int i, j;
    for (i = 0; i < x_size; i++)
        for (j = 0; j < y_size; j++)
            matrix[i][j] = num;
}

int length(char *list)
{
    int i = 0;
    while(list[i++] != '\0'){}
    return i-1;
}

char *enlarge(char *list, int size, int new_size)
{
    char *new_list = (char *)myalloc(new_size * sizeof(char));
    int i;
    for(i = 0; i < size; i++)
        new_list[i] = list[i];
    myfree(list);
    return new_list;
}


