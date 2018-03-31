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
#include <pwd.h>
#include <errno.h>


#define CMD_MAX_LEN 256
#define PIPE_MAX 10
#define HISTORY_MAX 25
#define ALIAS_MAX 10
#define MAX_BUF_SIZE 255
#define CUSTOM_COMMAND_COUNT 11

#define INPUT	0
#define OUTPUT 1
#define ERROR	2


extern char *shell_commands_list[CUSTOM_COMMAND_COUNT];
extern int (*shell_commands_pointer[CUSTOM_COMMAND_COUNT])(char **);

/*
 *Shell functions
 */

int tp_cd(char **);
int tp_exit(char **);
int tp_help(char **);
int tp_pwd(char **);
int tp_ls(char **);
int tp_history(char **);
int tp_alias(char **);
int tp_unalias(char **);
int tp_set_environment(char **);
int tp_rm_environment(char **);
int tp_get_environment(char **);

/*
 *Execution realated functions
 */
void initiate_globals();
void initiate_shell();
void prompt();
void execute_stack();
void replace_if_alias(char **);
int execute_custom(char **);
int execute_inbuilt(char **);
void shell_reset();

/*
 *Command and IO_redirect stack operations
 */
void push_init();
void arg_push(char *);
void current_command_args_rev();
void command_io_stack_display();
void current_command_display();
void i_o_push(char *, int);
void history_push(char **,pid_t,uid_t);
char *command_to_string(char **args);
void alias_display();

/*
 *Command linked list
 */
typedef struct command_stack_node{
	int	   number;
	char **args;
	int args_count;
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
	char *command;
	char *alias[ALIAS_MAX];
	int count;
	struct command_alias *next;
}command_aliases;

command_aliases *alias_new(char **args);
/*
 *command history
 */
typedef struct command_history{
	char *_command;
	struct tm *_timeinfo;
	pid_t _process_info;
	uid_t _user_info;
}command_history;



/*
 *Globals
 */
command_stack_node *head;
command_stack_node *current_node;
io_redirect io_redirect_info;
command_aliases *command_alias_head;
command_history command_hist[HISTORY_MAX];
int command_stack_current_size;
int args_current_push_location;
bool background;
int history_current_push_pointer;
int history_current_display_pointer;
time_t current_time;
int total_executed;


#endif
