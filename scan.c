#include <stdio.h>
#include <string.h>
#include "mylex.h"

#define REG_SIZE 1000

void scan()
{
    char *temp_reg_list[REG_SIZE];
    char *temp_act_list[REG_SIZE];

    reg_num = 0;

    char buffer[BUFFER_SIZE];
    int i;
    char c;
    for (i = 0; i < BUFFER_SIZE; buffer[i++] = '\0');

    scanf(" ${ %[^}]}$", buffer);
    decl_part = strdup(buffer);

    scanf(" $$");

    while(!scanf(" $$%c", &c)){
        scanf("%s", buffer);
        temp_reg_list[reg_num] = regular_exp(buffer);

        scanf(" { %[^}]}", buffer);
        temp_act_list[reg_num] = strdup(buffer);

        reg_num++;
    }

    scanf(" %[^EOF]", buffer);
    user_prog = strdup(buffer);

    reg_exp_list = (char **)malloc(reg_num * sizeof(char *));
    action_list = (char **)malloc(reg_num * sizeof(char *));
    for (i = 0; i < reg_num; i++){
        reg_exp_list[i] = temp_reg_list[i];
        action_list[i] = temp_act_list[i];
    }
}
