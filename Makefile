
all: execute stack_helper lex yacc main

execute: tpshell_execution.c	tpshell.h
					gcc -g -c tpshell_execution.c -o tp_exec.o

stack_helper:	stack_helper.c tpshell.h
							gcc -g -c stack_helper.c -o stk_hlp.o

lex: tpshell.l tpshell.h
			lex tpshell.l
yacc: tpshell.y tpshell.h
			yacc -d tpshell.y

main: tpshell.c tpshell.h
			gcc	-g lex.yy.c y.tab.c -Wall tpshell.c tp_exec.o stk_hlp.o -ll -o pilotshell
