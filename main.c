#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mylex.h"

int main(int argc, char *argv[])
{
    if (argc != 2){
        printf("Usage: mylex [lex_file]\n");
        exit(0);
    }
    char *file_in = argv[1];
    char *file_out = strdup(file_in);

    char *ptr = strrchr(file_out, '.');
    if(ptr && *(++ptr) == 'l'){
        *ptr++ = 'c';
        *ptr = '\0';
    }else{
        printf("unlegal lex file!");
        exit(0);
    }


    freopen(file_in, "r", stdin);
    scan();    

    initNfa();
    
    dfa();

    myfree_matrix(nfa_table, nfa_size);

    min_dfa();

    myfree_matrix(dfa_table, nfa_state_num * 2);
    myfree(dfa_ac);

    dfa_table = min_dfa_table;
    dfa_ac = min_dfa_ac;
    dfa_state_num = min_dfa_state_num;

    freopen(file_out, "w", stdout);

    gen_init();

    fclose(stdout);
    freopen("/dev/tty", "w", stdout);

    return 0;
}
