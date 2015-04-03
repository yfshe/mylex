#include <stdio.h>
#include <stdlib.h>
#include "mylex.h"

void initNfa()
{
    int i, size = 0;
    for (i = 0; i < reg_num; i++)
        size += strlen(reg_exp_list[i]);

    size *= 2;
    nfa_size = size * 2;

    nfa_table = myalloc_matrix(nfa_size, nfa_size);
    int j;
    /* -1 means no edge connectted */
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            nfa_table[i][j] = -1;
    
    nfa_start = -1;
    nfa_state_num = 0;
    nfa_ac = (int *)myalloc(nfa_size * sizeof(int));
    init_array(nfa_ac, 0, nfa_size);

    for (i = 0; i < reg_num; i++){
        nfa(reg_exp_list[i], i+1);
    }
}
void nfa(char *regexp, int reg_No)
{
    int size = length(regexp);
    nfa_stack = (int*)myalloc(size * sizeof(int)); 
    int i;
    for (i = 0; i < size; i++)
        nfa_stack[i] = 0;
    nfa_stack[0] = -1;
    int nfa_stack_p = 0;
    int start1, start2, end1, end2;
    int reg_p = 0;
    int state_p = nfa_state_num;
    while (regexp[reg_p] != '\0'){
        char ch = regexp[reg_p];
        switch(ch){
            case '+':
                end2 = nfa_stack[nfa_stack_p--];
                start2 = nfa_stack[nfa_stack_p--];
                end1 = nfa_stack[nfa_stack_p--];
                start1 = nfa_stack[nfa_stack_p--];
                int i;
                for (i = 0; i < state_p; i++){
                    nfa_table[end1][i] = nfa_table[start2][i];
                    nfa_table[start2][i] = -1;
                }
                nfa_stack[++nfa_stack_p] = start1;
                nfa_stack[++nfa_stack_p] = end2;
                break;
            case '|':
                end2 = nfa_stack[nfa_stack_p--];
                start2 = nfa_stack[nfa_stack_p--];
                end1 = nfa_stack[nfa_stack_p--];
                start1 = nfa_stack[nfa_stack_p--];
                nfa_stack[++nfa_stack_p] = state_p;
                nfa_table[state_p][start1] = EPSILON_EDGE;
                nfa_table[state_p++][start2] = EPSILON_EDGE;
                nfa_stack[++nfa_stack_p] = state_p;
                nfa_table[end1][state_p] = EPSILON_EDGE;
                nfa_table[end2][state_p++] = EPSILON_EDGE;
                break;
            case '*':
                end1 = nfa_stack[nfa_stack_p--];
                start1 = nfa_stack[nfa_stack_p--];
                nfa_table[end1][start1] = EPSILON_EDGE;
                nfa_stack[++nfa_stack_p] = state_p;
                nfa_table[state_p++][start1] = EPSILON_EDGE;
                nfa_table[state_p-1][state_p] = EPSILON_EDGE;
                nfa_stack[++nfa_stack_p] = state_p;
                nfa_table[end1][state_p++] = EPSILON_EDGE;
                break;
            case '?':
                end1 = nfa_stack[nfa_stack_p--];
                start1 = nfa_stack[nfa_stack_p--];
                nfa_stack[++nfa_stack_p] = state_p;
                nfa_table[state_p++][start1] = EPSILON_EDGE;
                nfa_table[state_p-1][state_p] = EPSILON_EDGE;
                nfa_stack[++nfa_stack_p] = state_p;
                nfa_table[end1][state_p++] = EPSILON_EDGE;
                break;
            default:
                nfa_stack[++nfa_stack_p] = state_p;
                nfa_stack[++nfa_stack_p] = state_p+1;
                nfa_table[state_p][state_p+1] = ALP_to_INT(ch);
                state_p++;
                state_p++;
        }
        reg_p++;    
    }

    int end = nfa_stack[nfa_stack_p--];
    int start = nfa_stack[nfa_stack_p--];

    if (nfa_start == -1){
        /* the first exp */
        nfa_start = start;
    }else{
        /* epsilon connect to the privious nfa */
        nfa_table[start][nfa_start] = EPSILON_EDGE;
        nfa_start = start;
    }

    nfa_ac[end] = reg_No;

    nfa_state_num = state_p;

    myfree(nfa_stack);    
}

void printNfa()
{
    int i, j;
    printf("\t");
    for (i = 0; i < nfa_state_num; i++)
        printf("S%d\t", i);
    printf("\n");
    for (i = 0; i < nfa_state_num; i++){
        printf("S%d\t", i);
        for (j = 0; j < nfa_state_num; j++)
            printf("%d\t", nfa_table[i][j]);
        printf("\n");
    }
}
