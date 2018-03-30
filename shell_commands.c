#include "tpshell.h"


/*
 *List of implemented shell commands
 */
char *shell_commands_list[CUSTOM_SHELL_COUNT] = {
 "cd",
 "exit",
 "help",
 "pwd",
 "ls",
 "history",
 "alias",
 "unalias",
 "set_environ",
 "rm_environ"
};

/*
 *Pointers to all the implemented shell commands
 *Matches index with the char list
 */
int (*shell_commands_pointer[CUSTOM_SHELL_COUNT])(char **) = {
 &tp_cd,
 &tp_exit,
 &tp_help,
 &tp_pwd,
 &tp_ls,
 &tp_history,
 &tp_alias,
 &tp_unalias,
 &tp_set_environment,
 &tp_rm_environment
};

int tp_cd(char **args){

  #ifdef DEBUG
    printf("in tp_cd\n");
  #endif

  if(args[1] == NULL){
      printf("ERROR: need to pass argument to \'cd\'\n");
      return 0;
  }
  else{
    if(chdir(args[1]) != 0){
      printf("ERROR: issue with changeDir\n");
    }
  }
  return 1;
}

int tp_exit(char **args){

  #ifdef DEBUG
    printf("in tp_exit\n");
  #endif

  printf("Exiting shell :|\n");
  pid_t sigh = getpid();
  kill(sigh,SIGKILL);
}

int tp_help(char **args){

    #ifdef DEBUG
      printf("in tp_help\n");
    #endif
}

int tp_pwd(char **args){

    #ifdef DEBUG
      printf("in tp_pwd\n");
    #endif
}

int tp_ls(char **args){

    #ifdef DEBUG
      printf("in tp_ls\n");
    #endif
}

int tp_history(char **args){

    #ifdef DEBUG
      printf("in tp_history\n");
    #endif
}

int tp_alias(char **args){

    #ifdef DEBUG
      printf("in tp_alias\n");
    #endif
}

int tp_unalias(char **args){

    #ifdef DEBUG
      printf("in tp_unalias\n");
    #endif
}

int tp_set_environment(char **args){

    #ifdef DEBUG
      printf("in tp_set_environment\n");
    #endif
}

int tp_rm_environment(char **args){

    #ifdef DEBUG
      printf("in tp_rm_environment\n");
    #endif
}
