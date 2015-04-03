#include <string.h>
#include "mylex.h"

#define BUFFER_SIZE 4096

char *transRegExp(char *raw_reg)
{
    char temp_reg[BUFFER_SIZE];
    char temp_reg_2[BUFFER_SIZE];
    char *raw_p = raw_reg, *temp_p = temp_reg;

    /* translate '\n' and '\t' */
    while(*raw_p != '\0'){
        if(*raw_p == '\\'){
            raw_p++;
            switch(*raw_p++){
                case 'n':
                    *temp_p++ = '\n';
                    break;
                case 't':
                    *temp_p++ = '\t';
                    break;
                case 's':
                    *temp_p++ = '[';
                    *temp_p++ = '\f';
                    *temp_p++ = '\n';
                    *temp_p++ = '\r';
                    *temp_p++ = '\t';
                    *temp_p++ = '\v';
                    *temp_p++ = ' ';
                    *temp_p++ = ']';
                    break;
            }
        }else{
            *temp_p++ = *raw_p++;
        }
    }
    *temp_p = '\0';

    raw_p = temp_reg;
    temp_p = temp_reg_2;

    char *last_left_ch;
    char *last_right_ch;
    int comp_pair = 0;
    /* translate [ ] and + */
    while (*raw_p != '\0'){
        switch(*raw_p){
            case '[':
                last_left_ch = temp_p;
                *temp_p++ = '(';

                char *pre_ch;
                raw_p++;
                *temp_p++ = *raw_p;
                pre_ch = raw_p++;

                while (*raw_p != ']'){
                    if(*raw_p == '-'){
                        raw_p++;
                        int i;
                        for (i = 1; i < *raw_p - *pre_ch +1; i++){
                            *temp_p++ = '|';
                            *temp_p++ = *pre_ch + i;
                        }
                        raw_p++;
                    }else{
                        *temp_p++ = '|';
                        *temp_p = *raw_p++;
                        pre_ch = temp_p++;
                    }
                }
                *temp_p++ = ')';
                raw_p++;
                if (*raw_p == '+'){
                    comp_pair = 1;
                    last_right_ch = temp_p;
                }
                break;
            case '+':
                if (comp_pair){
                    *temp_p++ = '*';
                    while(last_left_ch != last_right_ch){
                        *temp_p++ = *last_left_ch++;
                    }
                    comp_pair = 0;
                }else{
                    *temp_p++ = '*';
                    *temp_p++ = *(raw_p - 1);
                }
                raw_p++;
                break;
            case '(':
                last_left_ch = temp_p;
                *temp_p++ = *raw_p++;
                break;
            case ')':
                *temp_p++ = *raw_p++;
                if (*raw_p == '+'){
                    comp_pair = 1;
                    last_right_ch = temp_p;
                }
                break;
            default:
                *temp_p++ = *raw_p++;
        }
    }

    *temp_p = '\0';

    return strdup(temp_reg_2);
}

char *post_regexp(char *infix_reg)
{
    int len = length(infix_reg);
    char *postfix_reg = (char *)myalloc((len+1) * sizeof(char));
    char *stack = (char *)myalloc(len* sizeof(char));
    stack[0] = '\0';
    int i = 0, j = 0, stack_p = 0;
    while (infix_reg[i] != '\0'){
        switch(infix_reg[i]){
            case '|':
            case '+':
                while (rank(stack[stack_p]) >= rank(infix_reg[i])){
                    postfix_reg[j++] = stack[stack_p--];
                }
                stack[++stack_p] = infix_reg[i];
                break;
            case '(':
                stack[++stack_p] = infix_reg[i];
                break;
            case ')':
                while(stack[stack_p] != '('){
                    postfix_reg[j++] = stack[stack_p--];
                }
                stack_p--;
                break;
            case '*':
            case '?':
            default:
                postfix_reg[j++] = infix_reg[i];
        }
        i++;
    }
    while(stack[stack_p] != '\0')
        postfix_reg[j++] = stack[stack_p--];

    postfix_reg[j] = '\0';
    return postfix_reg;
}

char *fill_regexp(char *reg)
{
    int len = length(reg);
    char *full_reg = (char *)myalloc((len* 2 + 1) * sizeof(char));
    int i, j;
    for (i = 0, j = 0; i < len; i++){
        char c = reg[i];
        full_reg[j++] = c;
        if(i != len-1)
            if((ISALPHA(c) && (ISALPHA(reg[i+1]) || reg[i+1] == '('))
            || (c == '*' && (ISALPHA(reg[i+1]) || reg[i+1] == '('))
            || (c == '?' && (ISALPHA(reg[i+1]) || reg[i+1] == '('))
            || (c == ')' && (ISALPHA(reg[i+1]) || reg[i+1] == '(')))
                full_reg[j++] = '+';
    }
    full_reg[j] = '\0';

    return full_reg;
}

char *regular_exp(char *exp)
{
    char *reg_exp = transRegExp(exp);
    reg_exp = fill_regexp(reg_exp);
    reg_exp = post_regexp(reg_exp);
    
    return reg_exp;
}

int rank(char c)
{
    switch(c){
        case '?':
            return 5;
        case '*':
            return 4;
        case '+':
            return 3;
        case '|':
            return 2;
        case '(':
            return 1;
        default:
            return 0;
    }
}


