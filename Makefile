
all: execute stack_helper lex yacc shell_commands main

execute: tpshell_execution.c	tpshell.h
					gcc -g -c tpshell_execution.c -o .tp_exec.o

stack_helper:	stack_helper.c tpshell.h
							gcc -g -c stack_helper.c -o .stk_hlp.o

lex: lex.l tpshell.h
			lex lex.l

yacc: yacc.y tpshell.h
			yacc -d yacc.y

shell_commands: shell_commands.c tpshell.h
								gcc -g -c shell_commands.c -o .shell_cmd.o

main: shell_main.c tpshell.h
			gcc	-g -O0 lex.yy.c y.tab.c shell_main.c .tp_exec.o .stk_hlp.o .shell_cmd.o -ll -w -o pilotshell.exe
clean:
	rm *.exe .*.o y.tab.h lex.yy.c y.tab.c
