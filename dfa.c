#include <stdio.h>
#include <stdlib.h>
#include "mylex.h"

void printDstates(int **Dstates)
{
    int i = 0, j;
    while (Dstates[i] != NULL){
        j = 1;
        while (Dstates[i][j] != -1)
            printf("%d ", Dstates[i][j++]);
        printf("\n");
        i++;
    }
}

void printDstate(int *Dstate)
{
    int i = 1;
    while (Dstate[i] != -1)
        printf("%d ", Dstate[i++]);
    printf("\n");
}

void printDFA()
{
    int i, j;
    for (i = 0; i < ASCII_SIZE; i++)
        printf ("\t%c", (char)(i+'a'));
    printf("\n");
    for (i = 0; i < dfa_state_num; i++){
        printf("%d", i);
        for (j = 0; j < ASCII_SIZE; j++)
            printf ("\t%d", dfa_table[i][j]);
        printf("\n");
    }
}

void printMinDFA()
{
    int i, j;
    for (i = 0; i < ASCII_SIZE; i++)
        printf ("\t%d", i);
    printf("\n");
    for (i = 0; i < min_dfa_state_num; i++){
        printf("%d", i);
        for (j = 0; j < ASCII_SIZE; j++)
            printf ("\t%d", min_dfa_table[i][j]);
        printf("\n");
    }
}

void dfa()
{
    int max_states = nfa_state_num * 2;
    dfa_table = myalloc_matrix(max_states, ASCII_SIZE);
    init_matrix(dfa_table, -1, max_states, ASCII_SIZE);
    dfa_ac = (int *)myalloc(max_states * sizeof(int));
    init_array(dfa_ac, 0, max_states);
    int dfa_ac_p = 0;
    int **Dstates = (int **)myalloc(max_states * sizeof(int *));
    int Dstates_p = 0;
    int i;
    for (i = 0; i < max_states; i++)
        Dstates[i] = NULL;
    int *state_Set = (int *)myalloc(nfa_state_num * sizeof(int));
    init_array(state_Set, -1, nfa_state_num);
    int size;
    state_Set[0] = nfa_start;
    closure(state_Set, &size);
    Dstates[Dstates_p] = Dstate(size);
    copySet(Dstates[Dstates_p], 1, state_Set, size);

    dfa_ac[Dstates_p] = find_ac(Dstates[Dstates_p]);

    Dstates_p++;
    int FINISH = 0;
    /* state reference a Dstate in Dstates */
    /* state_p is a pointer of Dstates */
    int *state, state_p, *next_state, next_state_p;
    while(!FINISH){
        state = NULL;
        int i;
        /* to find a set not marked */
        for (i = 0; i < Dstates_p; i++){
            if(Dstates[i][0] ==  -1){
                /* Dstates[][0] == 0 means marked */
                Dstates[i][0] = 0;
                state = Dstates[i];
                state_p = i;
                break;
            }
        }
        if (state == NULL){
            /* no set is not marked */
            FINISH = 1;
            continue;
        }
        for (i = 0; i < ASCII_SIZE; i++){
            init_array(state_Set, -1, nfa_state_num);
            if (move(state, state_Set, i)){
                /* effective move */
                closure(state_Set, &size);
                next_state = Dstate(size);
                copySet(next_state, 1, state_Set, size);

                if (next_state_p = containDstate(Dstates, next_state)){
                    /* next_state_p is a state already in Dstates */
                    dfa_table[state_p][i] = next_state_p;
                }else{
                    /* create a new state in Dstates */
                    dfa_ac[Dstates_p] = find_ac(next_state);
                    dfa_table[state_p][i] = Dstates_p;
                    Dstates[Dstates_p++] = next_state;
                }
            }
        }
    }
    dfa_state_num = Dstates_p;
    dfa_start = 0;
    

}

int find_ac(int *state_set)
{
    int i = 1, ac = 0;
    while (state_set[i] != -1){ 
        int n_ac = nfa_ac[state_set[i]];
        if(n_ac != 0){
            if(ac == 0)
                ac = n_ac;
            else if(n_ac < ac)
                ac = n_ac;
        }
        i++; 
    }
    return ac;
}

void printArray(int *array, int size)
{
    int i; 
    for (i = 0; i < size; i++)
        printf("%d\t", i);
    printf("\n");
    for (i = 0; i < size; i++)
        printf("%d\t", array[i]);
    printf("\n");
}

void min_dfa()
{
    int *state_group = (int *)myalloc(dfa_state_num * sizeof(int));
    int *state_link = (int *)myalloc(dfa_state_num * sizeof(int));



    /* record the represent state in each group(new state) */
    int *min_states = (int *)myalloc(dfa_state_num * sizeof(int));
    /* show the grouping view in each group */
    int *groups = (int *)myalloc((dfa_state_num+1) * sizeof(int));
    int min_states_p = 0;


    init_array(min_states, -1, dfa_state_num);
    init_array(groups, -1, dfa_state_num+1);


    int i;
    for (i = 0; i < dfa_state_num; i++){
        state_group[i] = dfa_ac[i];
    }

    /* printf("state_group:\n"); */
    /* printArray(state_group, dfa_state_num); */


    /* construct the linked list for each group */
    /* -1 means end */
    for (i = dfa_state_num-1; i >= 0; i--){
        int group = state_group[i];
        if (min_states[group] ==  -1){
            state_link[i] = -1;
        }else{
            state_link[i] = min_states[group];
        }
        min_states[group] = i;
    }
    min_states_p = reg_num + 1;

    /* printf("state_link:\n"); */
    /* printArray(state_link, dfa_state_num); */

    /* grouping */
    int p_old;
    do{
        p_old = min_states_p;
        int n = min_states_p, i;
        for (i = 0; i < n; i++){
            int rep_state = min_states[i];
            int ch;
            int state, next_state, trans_state, group_p;
            for (ch = 0; ch < ASCII_SIZE; ch++){
                state = rep_state;

                init_array(groups, -1, dfa_state_num+1);

                while(state != -1){
                    /* save next_state, because state_link may changed */
                    next_state = state_link[state];
                    trans_state = dfa_table[state][ch];
                    
                    if(trans_state == -1)
                        /* dead_state */
                        group_p = dfa_state_num;
                    else
                        group_p = state_group[trans_state];
                    
                    if (groups[group_p] == -1){
                        /* form a new group */
                        groups[group_p] = state;
                        state_link[state] = -1;
                        if(state != rep_state){
                            /* form a new state */
                            state_group[state] = min_states_p;
                            min_states[min_states_p++] = state;


                            /* printf("state_group:\n"); */
                            /* printArray(state_group, dfa_state_num); */


                        }
                    }else{
                        /* join the group */
                        int st = groups[group_p];
                        while (state_link[st] != -1)
                            st = state_link[st];
                        state_link[st] = state;
                        state_group[state] = state_group[st];
                        state_link[state] = -1;


                        /* printf("state_group:\n"); */
                        /* printArray(state_group, dfa_state_num); */


                    }
                    state = next_state;
                }
            }
        }
    }while(p_old != min_states_p);

    min_dfa_state_num = min_states_p;
    min_dfa_table = myalloc_matrix(min_dfa_state_num, ASCII_SIZE);
    init_matrix(min_dfa_table, -1, min_dfa_state_num, ASCII_SIZE);

    min_dfa_ac = myalloc(min_dfa_state_num * sizeof(int));
    init_array(min_dfa_ac, 0, min_dfa_state_num);

    for (i = 0; i < min_dfa_state_num; i++){
        /* represent state */
        int state = min_states[i];
        
        min_dfa_ac[i] = dfa_ac[state];

        while (state != -1){
            int trans_state, ch;
            for (ch = 0; ch < ASCII_SIZE; ch++){
                trans_state = dfa_table[state][ch];
                if (trans_state != -1)
                    min_dfa_table[i][ch] = state_group[trans_state];
            }
            state = state_link[state];
        }
    }


}

int move(const int *FROM, int *TO, int ch)
{
    int f_ptr = 1, t_ptr = 0;
    while(FROM[f_ptr] != -1){
        int state = FROM[f_ptr++];
        int next_state = 0;
        for (; next_state < nfa_state_num; next_state++){
            if(nfa_table[state][next_state] == ch){
                int i = 0;
                for (; i < t_ptr && TO[i] != next_state; i++){}
                if (i == t_ptr)
                    TO[t_ptr++] = next_state;
            }
        }
    }
    return t_ptr;
}

void closure(int *state_Set, int *size)
{
    int *stack = (int *)myalloc(nfa_state_num * sizeof(int));
    stack[0] = -1;
    int stack_p = 0, Set_p = 0;    
    
    while (state_Set[Set_p] != -1){
        stack[++stack_p] = state_Set[Set_p++];
    }

    while(stack[stack_p] != -1){
        int state = stack[stack_p--];
        int i, j;
        for (i = 0; i < nfa_state_num; i++){
            int edge = nfa_table[state][i];
            if(edge == EPSILON_EDGE){
                for (j = 0; j < Set_p; j++){
                    if(state_Set[j] == i)
                        break;
                }
                if(j == Set_p){
                    state_Set[Set_p++] = i;
                    stack[++stack_p] = i;
                }
            }
        }
    }
    *size = Set_p;
}

int *Dstate(int size)
{
    int *Dstate = (int *)malloc((size+2) * sizeof(int));
    /* Dstates[][0] == 1 means no marked */
    Dstate[0] = -1;
    /* -1 means end */
    Dstate[size+1] = -1;
    return Dstate;
}

void copySet(int *to_set, int start_index, int *from_set, int size)
{
    int i;
    for (i = 0; i < size; i++){
        to_set[start_index+i] = from_set[i];
    }
    
}

int equalSet(int *set1, int *set2)
{
    int *markSet = (int *)myalloc(nfa_state_num * sizeof(int));
    int i;
    for(i = 0; i < nfa_state_num; i++)
        markSet[i] = 0;
    i = 1;
    while (set1[i] != -1)
        markSet[set1[i++]] = 1;
    i = 1;
    while (set2[i] != -1){
        if(markSet[set2[i]] == 0){
            myfree(markSet);
            return 0;
        }
        else
            markSet[set2[i++]] = 0;
    }
    for (i = 0; i < nfa_state_num; i++)
        if (markSet[i] == 1){
            myfree(markSet);
            return 0;
        }
    myfree(markSet);
    return 1;
}

int containDstate(int **Dstates, int *state)
{
    int i = 0;
    while (Dstates[i] != NULL){
        if (equalSet(Dstates[i], state))
            return i;
        i++;
    }
    return 0;
}

int containState(int *state_set, int state)
{
    int i = 1;
    while (state_set[i] != -1 && state_set[i] != state){ i++; }
    if (state_set[i] == state)
        return 1;
    else
        return 0;
}


