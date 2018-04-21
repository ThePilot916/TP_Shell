#include "tp_shell.h"

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
 	"setenv",
 	"unsetenv",
  "getenv",
  "editor",
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
 	&tp_rm_environment,
  &tp_get_environment,
  &tp_editor,
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
  return 1;
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
    return 1;
}


int tp_alias(char **args){

    #ifdef DEBUG
      printf("____________tp_alias____________\n");
    #endif

    char **arg_temp = args;
    if(command_alias_head == NULL){
      command_aliases *temp = alias_new(arg_temp);
      command_alias_head = temp;
      printf("Alias entry added successfully!\n");
    }
    else{
      command_aliases *current = command_alias_head;
      command_aliases *previous = NULL;
      while(current != NULL){

        if(strcmp(current->command,*(arg_temp+1)) == 0){
          break;
        }
        previous = current;
        previous = current->next;
      }

      if(current == NULL){
        previous->next = alias_new(arg_temp);
        printf("Alias entry added successfully!\n");
      }
      else{
        if(current->count >= ALIAS_MAX){
          printf("ERROR: already at max number of aliases\n");
        }
        else{
          char *str_temp = malloc(sizeof(char)*strlen(*(arg_temp+2)));
          strcpy(str_temp,*(arg_temp+2));
          current->alias[current->count] = str_temp;
          printf("Alias entry added successfully!\n");
        }
      }
    }
    #ifdef DEBUG
      alias_display();
    #endif
    return 1;
}


int tp_unalias(char **args){

    #ifdef DEBUG
      printf("____________tp_unalias____________\n");
    #endif

    command_aliases *temp = command_alias_head;
    command_aliases *previous = NULL;
    while(temp != NULL){
      if(strcmp(temp->command,*(args+1)) == 0){
        break;
      }
      previous = temp;
      temp = temp->next;
    }
    if(temp == NULL){
      printf("ERROR: no alias entry available\n");
    }
    else{
      if(previous != NULL){
        previous->next = temp->next;
      }
      if(command_alias_head->next == NULL){
        command_alias_head = NULL;
      }
      free(temp);
      printf("Alias entry removed successfully!\n");
    }

    return 1;
}


int tp_set_environment(char **args){

    #ifdef DEBUG
      printf("____________tp_set_environment ____________\n");
    #endif
    int res = setenv(*(args+1),*(args+2),1);
    if(res != 0){
      printf("ERROR: %s\n",strerror(errno));
      return -1;
    }
    return 1;
}


int tp_rm_environment(char **args){

    #ifdef DEBUG
      printf("____________tp_rm_environment____________\n");
    #endif
    int res = unsetenv(*(args+1));
    if(res != 0){
      printf("ERROR: %s\n",strerror(errno));
      return -1;
    }
    return 1;
}


int tp_get_environment(char **args){

  #ifdef DEBUG
    printf("____________tp_get_environment____________\n");
  #endif
  char *res = NULL;
  res = getenv(*(args+1));
  printf("%s: %s\n",*(args+1),res);
  return 1;
}

int tp_editor(char **args){

    editor(*(args+1));

    return 1;
}

int tp_debug(char **args){

}
