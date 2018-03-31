#include "tpshell.h"

/*
 *List of implemented shell commands
 */

 char *shell_commands_list[CUSTOM_COMMAND_COUNT] = {
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
 int (*shell_commands_pointer[CUSTOM_COMMAND_COUNT])(char **) = {
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
    printf("____________tp_cd____________\n");
  #endif
  if (args[1] == NULL) {
    printf("ERROR: no argument given to cd\n");
  }
  else {
    if (chdir(args[1]) != 0) {
      printf("ERROR: changedir error\n");
    }
  }
  return 1;
}

int tp_exit(char **args){

  #ifdef DEBUG
    printf("____________tp_exit____________\n");
  #endif
  printf("Exiting shell :|\n");
  pid_t sigh = getpid();
  kill(sigh,SIGKILL);

}

int tp_help(char **args){

    #ifdef DEBUG
      printf("____________tp_help____________\n");
    #endif
    int i;
    printf("************************************************************************\n");
    printf("\tTo use tpshell simply type an executable name to run a program\n");
    printf("\tThe shell will search through the custom commands first and if present\n it'll return that function else search through the unix commands\n");
    printf("************************************************************************\n");
    printf("\tCustom commands listed below:\n");

    for (i = 0; i < CUSTOM_COMMAND_COUNT; i++) {
      printf("%d. %s\n",i,shell_commands_list[i]);
    }
    printf("************************************************************************\n");
    printf("man command for more info on default commands/programs\n");
    return 1;
}

int tp_pwd(char **args){

    #ifdef DEBUG
      printf("____________tp_pwd____________\n");
    #endif
    char *pwd_buf=(char *)malloc(MAX_BUF_SIZE*sizeof(char));
  	if(getcwd(pwd_buf,MAX_BUF_SIZE)==NULL)
  		printf("ms: getcwd error\n");
  	printf("%s\n",pwd_buf);
  	return 1;
}

int tp_ls(char **args){

    #ifdef DEBUG
      printf("____________tp_ls____________\n");
    #endif
    DIR *mydir;
    struct dirent *fp;
    char *pwd_buf=(char *)malloc(MAX_BUF_SIZE*sizeof(char));
    if(getcwd(pwd_buf,MAX_BUF_SIZE)==NULL)
    printf("ms: getcwd error\n");

    mydir = opendir(pwd_buf);
    int i = 0;
    while((fp = readdir(mydir)) != NULL){
    i++;
    printf("  %d.\t%s\n",i, fp->d_name);
    }
    closedir(mydir);

    return 1;
}

int tp_history(char **args){

    #ifdef DEBUG
      printf("____________tp_history____________\n");
    #endif
    printf("\t\t\t\t\tHISTORY\n");
    printf("-----------------------------------------------------------------------------------------\n");
    for(int i = 0; (i < HISTORY_MAX && i < total_executed); i++){

      printf("CMD: %s\n",command_hist[i]._command);
      printf("\t-----pid: %d\n",command_hist[i]._process_info);
      printf("\t-----uid: %d\n",command_hist[i]._user_info);
      printf("\t-----time: %s\n",asctime(command_hist[i]._timeinfo));
      printf("______________________________________\n");
    }
}

int tp_alias(char **args){

    #ifdef DEBUG
      printf("____________tp_alias____________\n");
    #endif
}

int tp_unalias(char **args){

    #ifdef DEBUG
      printf("____________tp_unalias____________\n");
    #endif
}

int tp_set_environment(char **args){

    #ifdef DEBUG
      printf("____________tp_set_environment ____________\n");
    #endif
}

int tp_rm_environment(char **args){

    #ifdef DEBUG
      printf("____________tp_rm_environment____________\n");
    #endif
}
