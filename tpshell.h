#ifndef DECLARED
#define DECLARED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <dirent.h>
#include <termios.h>
#include <time.h>


#define CMD_MAX_LEN 256
#define PIPE_MAX 10
#define HISTORY_MAX 25
#define ALIAS_MAX 10
#define MAX_BUF_SIZE 255


#define INPUT	0
#define OUTPUT	1
#define ERROR	2

/*
 *Command linked list
 */
typedef struct command_stack_node{
	int	   number;
	char **args;
	struct command_stack_node *next;
}command_stack_node;

/*
 * IO redir for the associated cmd,
 * also a linked list, TODO: refine
 */
typedef struct io_redirect{
	//int	  _number;
	char *_type[3];
	struct io_redirect *next;
}io_redirect;


/*
 *Command Aliases, value and function pointers
 */
typedef struct command_alias{
	char *_command;
	char *_alias[ALIAS_MAX];
	int count;
}command_aliases;

/*
 *command history
 */
typedef struct command_history{
	char *_history[HISTORY_MAX];
}command_history;



/*
 *Globals
 */
command_stack_node *head;
command_stack_node *current_node;
io_redirect io_redirect_info;
command_aliases command_alias;
command_history command_hist;
int command_stack_current_size;
int args_current_push_location;


/*
 *Shell functions
 */
int tp_cd(char **);
int tp_exit();
int tp_help();
int tp_pwd(char **);
int tp_ls(char **);
int tp_history();
int tp_alias(char *,char *);
int tp_unalias(char *,char *);
int tp_set_environment(char *);
int tp_rm_environment(char *);

/*
 *Execution realated functions
 */
void initiate_globals();
void execute();
void prompt();

/*
 *Command and IO_redirect stack operations
 */
void push_init();
void arg_push(char *);
void current_command_args_rev();
void command_io_stack_display();
void current_command_display();
void i_o_push(char *, int);



#endif
