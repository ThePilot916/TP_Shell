all: lex yacc execute stack_helper shell_commands editor main

lex: tp_lex.l tp_shell.h
			lex tp_lex.l

yacc: tp_yacc.y tp_shell.h
			yacc -d tp_yacc.y

execute: tp_shell_execution.c	tp_shell.h
					gcc -g -c tp_shell_execution.c -w -o .tp_exec.o

stack_helper:	tp_stack_helper.c tp_shell.h
							gcc -g -c tp_stack_helper.c -w -o .stk_hlp.o

shell_commands: tp_shell_commands.c tp_shell.h
								gcc -g -c tp_shell_commands.c -w -o .shell_cmd.o

editor: tp_shell_editor.c tp_shell.h
				gcc -g -c tp_shell_editor.c -o .editor.o

main: tp_shell_main.c tp_shell.h
			gcc	-g -O0 lex.yy.c y.tab.c tp_shell_main.c .tp_exec.o .stk_hlp.o .shell_cmd.o .editor.o -ll -lncurses -w -o pilotshell.exe

clean:
	rm *.exe .*.o y.tab.h lex.yy.c y.tab.c
