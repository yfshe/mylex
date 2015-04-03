#include <stdio.h>
#include "mylex.h"

void gen_init()
{
    genDecl();
    genDFA();
    genAccept();
    genCreateBuffer();
    genSimulation();
    genUserProg();
}

void genDecl()
{
    printf("#include <stdlib.h>\n");
    printf("#include <stdio.h>\n");
    printf("\n");

    printf("%s\n", decl_part);

    printf("#define YY_NULL 0\n");
    printf("#define YY_ERROR -1\n");
    printf("#define ASCII_SIZE %d\n", ASCII_SIZE);
    printf("#define CHAR_TO_INT(ch) (int)(ch)\n");
    printf("\n");
    
    printf("char *yy_buffer;\n");
    printf("char *yy_cp;\n");
    printf("FILE *yy_in = NULL;\n");
    printf("char yy_eof = EOF;\n");
    printf("\n");
}
void genDFA()
{
    printf("int dfa_table[%d][%d] = {\n", dfa_state_num, ASCII_SIZE);
    int i, j;
    for (i = 0; i < dfa_state_num; i++){
        printf("{\t");
        int count = 0;
        for (j = 0; j < ASCII_SIZE; j++){
            printf("%d", dfa_table[i][j]);
            count++;
            if (j !=  ASCII_SIZE-1)
                printf(",\t");
            if ((count = count % 10) == 0)
                printf("\n\t");
        }
        if (i ==  dfa_state_num-1)
            printf("}");
        else
            printf("},\n");
    }
    printf("};\n");
    printf("\n");

    printf("int dfa_ac[%d] = {", dfa_state_num);
    for (i = 0; i < dfa_state_num; i++){
        printf("%d", dfa_ac[i]);
        if (i !=  dfa_state_num-1)
            printf(",\t");
    }
    printf("};\n");
    printf("\n");

    printf("int dfa_start = %d;\n", dfa_start);

    printf("\n");
}

void genAccept()
{
    printf("int accept(int state){\n");
    printf("\treturn dfa_ac[state];\n");
    printf("}\n");
    printf("\n");
}

void genCreateBuffer()
{
    printf("void yy_create_buffer(){\n");
    printf("\tif (yy_in == NULL){\n");
    printf("\t\tyy_in = stdin;\n");
    printf("\t\tyy_eof = '\\n';\n");
    printf("\t}\n");
    printf("\tint size = 10;\n");
    printf("\tyy_buffer = malloc(size * sizeof(char));\n");
    printf("\tint i = 0;\n");
    printf("\tchar c;\n");
    printf("\twhile((c = fgetc(yy_in)) != yy_eof){\n");
    printf("\t\tyy_buffer[i++] = c;\n");
    printf("\t\tif (i == size-1){\n");
    printf("\t\t\tchar *new_buffer = (char *)malloc(size * 2 * sizeof(char));\n");
    printf("\t\t\tint j;\n");
    printf("\t\t\tfor(j = 0; j < size; j++)\n");
    printf("\t\t\t\tnew_buffer[j] = yy_buffer[j];\n");
    printf("\t\t\tfree(yy_buffer);\n");
    printf("\t\t\tyy_buffer = new_buffer;\n");
    printf("\t\t\tsize *= 2;\n");
    printf("\t\t}\n");
    printf("\t}\n");
    printf("\tyy_buffer[i] = '\\0';\n");
    printf("\tyy_cp = yy_buffer;\n");
    printf("}\n");
    printf("\n");
}
        
    
    
    
void genSimulation()
{
    printf("int yy_lex(){\n");
    printf("\tif (yy_buffer == NULL){\n");
    printf("\t\tyy_create_buffer();\n");
    printf("\t}\n");
    printf("\tint current_state = dfa_start, next_state, last_ac_state, act;\n");
    printf("\tchar *last_ac_cp;\n");
    printf("\twhile (1){\n");
    printf("\t\tlast_ac_state = -1;\n");
    printf("\t\tlast_ac_cp = NULL;\n");
    printf("\t\tif(*yy_cp == '\\0'){\n");
    printf("\t\t\treturn YY_NULL;\n");
    printf("\t\t}\n");
    printf("\t\tnext_state = current_state;\n");
    printf("\t\tdo{\n");
    printf("\t\t\tif(accept(next_state)){\n");
    printf("\t\t\t\tlast_ac_cp = yy_cp;\n");
    printf("\t\t\t\tlast_ac_state = next_state;\n");
    printf("\t\t\t}\n");
    printf("\t\t\tcurrent_state = next_state;\n");
    printf("\t\t\tnext_state = dfa_table[current_state][CHAR_TO_INT(*yy_cp)];\n");
    printf("\t\t\tyy_cp++;\n");
    printf("\t\t}while(next_state != -1);\n");
    printf("\t\tif(last_ac_state != -1){\n");
    printf("\t\t\tact = accept(last_ac_state);\n");
    printf("\t\t\tyy_cp = last_ac_cp;\n");
    printf("\t\t\tcurrent_state = dfa_start;\n");
    printf("\t\t}else{\n");
    printf("\t\treturn YY_ERROR;\n");
    printf("\t\t}\n");
    printf("\t\tswitch(act){\n");

    int i;
    for (i = 0; i < reg_num; i++){
        printf("case %d:\n", i+1);
        printf("%s\n", action_list[i]);
        printf("break;\n");
    }
    printf("default:\n");
    printf("return YY_ERROR;\n");
    printf("\t\t}\n");
    printf("\t}\n");
    printf("}\n");
    printf("\n");
}

void genUserProg()
{
    printf("%s", user_prog);
}
