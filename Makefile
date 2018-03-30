
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
			gcc	-g lex.yy.c tpshell.c shell_main.c tp_exec.o stk_hlp.o -ll -w -o pilotshell.exe
clean:
	rm *.exe *.o y.tab.h lex.yy.c tpshell.c
