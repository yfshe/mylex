#include <stdlib.h>
#include <stdio.h>
#include "mylex.h"
#include "simulate.h"

#define YY_NULL 0
#define YY_ERROR -1
#define CHAR_TO_INT(ch) (int)(ch)

FILE *yyin = (FILE *) 0, *yyout = (FILE *) 0;
char *yy_buffer;
char *yy_cp;

int accept(int state)
{
    return dfa_ac[state];
}

char *enlarge_buffer(char *list, int size, int new_size)
{
    char *new_list = (char *)malloc(new_size * sizeof(char));
    int i;
    for(i = 0; i < size; i++)
        new_list[i] = list[i];
    free(list);
    return new_list;
}

void create_buffer(FILE *in)
{
    int size = 10;
    yy_buffer = malloc(size * sizeof(char));

    int i = 0;
    char c;
    while((c = fgetc(in)) != '\n'){
        yy_buffer[i++] = c;
        if (i == size-1){
            char *new_buffer = (char *)malloc(size * 2 * sizeof(char));
            int j;
            for(j = 0; j < size; j++)
                new_buffer[j] = yy_buffer[j];
            free(yy_buffer);
            yy_buffer = new_buffer;
            size *= 2;
        }
    }
    yy_buffer[i] = '\0';
    yy_cp = yy_buffer;
}

int yy_lex()
{

    int current_state = dfa_start, next_state, last_ac_state, act;
    char *last_ac_cp;
    while (1){
        last_ac_state = -1;
        last_ac_cp = NULL;

        if(*yy_cp == '\0'){
            return YY_NULL;
        }

        next_state = current_state;
        do{
            if(accept(next_state)){
                last_ac_cp = yy_cp;
                last_ac_state = next_state;
            }
            current_state = next_state;
            next_state = dfa_table[current_state][CHAR_TO_INT(*yy_cp)];
            yy_cp++;
        }while(next_state != -1);

        if(last_ac_state != -1){
            act = accept(last_ac_state); 
            yy_cp = last_ac_cp;
            current_state = dfa_start;
        }else{
            return YY_ERROR;
        }

        switch(act){
            case 1:
                {
                    return 1;
                }
                break;
            case 2:
                {
                    return 2;
                }
                break;
            default:
                return YY_ERROR;
        }
    }
}

int test_main()
{
    create_buffer(stdin);
    
    int next_token;
    while ((next_token = yy_lex()) != YY_NULL){
        switch(next_token){
            case 1:
                printf("find ab*a\n");
                break;
            case 2:
                printf("find ba*b\n");
                break;
            case YY_ERROR:
                printf("case error\n");
        }
    }

}
