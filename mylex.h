#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ASCII_SIZE 128
#define EPSILON_EDGE -2
#define NO_EDGE -1
#define ISALPHA(c) (c != '*' && c != '|' && c != '(' && c != ')' && c != '+' && c != '?') 
#define ALP_to_INT(c) ((int)c)
#define BUFFER_SIZE 4096

void initNfa();
void nfa(char *exp, int exp_No);
void dfa();
int find_ac(int *state_set);
void min_dfa();
int move(const int *FROM, int *TO, int ch);
void closure(int *T, int *size);
void *myalloc(size_t size);
void myfree(void *ptr);
int **myalloc_matrix(size_t first_size, size_t second_size);
void myfree_matrix(int **ptr, int size);
void init_array(int *array, int num, int size);
void init_matrix(int **matrix, int num, int x_size, int y_size);
int rank(char c);
int length(char *list);
char *enlarge(char *list, int size, int new_size);
int *Dstate(int size);
void copySet(int *to_set, int start_index, int *from_set, int size);
int equalSet(int *set1, int *set2);
int containDstate(int **Dstates, int *state);


/* nfa */
int **nfa_table;
int *nfa_stack;
int *nfa_ac;
int nfa_start;
int nfa_state_num;
int nfa_size;
/* nfa */


/* dfa */
int **dfa_table;
int *dfa_ac;
int dfa_start;
int dfa_state_num;

int **min_dfa_table;
int *min_dfa_ac;
int min_dfa_state_num;
int min_dfa_start;
/* dfa */


/* reg_exp */
char *post_regexp(char *infix_reg);
char *fill_regexp(char *reg);
char *transRegExp(char *raw_reg);
char *regular_exp(char *exp);
/* reg_exp */


/* scan */
char *decl_part;
char **reg_exp_list;
char **action_list;
char *user_prog;
int reg_num;

void scan();
/* scan */

/* gen */
void gen_init();
void genDecl();
void genDFA();
void genAccept();
void genCreateBuffer();
void genSimulation();
void genUserProg();
/* gen */
