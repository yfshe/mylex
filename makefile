mylex : main.c mylex.h gen.c scan.c reg_exp.c dfa.c nfa.c util.c
	gcc -g main.c mylex.h gen.c scan.c reg_exp.c dfa.c nfa.c util.c -o mylex
