#include "tp_shell.h"


void initiate_globals(){

  #ifdef DEBUG
    printf("____________initiating globals____________\n");
  #endif

  command_stack_current_size = 0;
  args_current_push_location = 0;
  head = NULL;
  current_node = NULL;
  io_redirect_info._type[INPUT] = NULL;
  io_redirect_info._type[OUTPUT] = NULL;
  io_redirect_info._type[ERROR] = NULL;
  background = false;
  history_current_display_pointer = 0;
  history_current_push_pointer = 0;
  command_alias_head = NULL;
  themeval = 1;
}


void initiate_shell(){

      #ifdef DEBUG
        printf("____________initiating shell____________\n");
      #endif
      printf("\x1b[41;37m");
      printf("\n\t------------------------------------------------------------ThePilot Shell 3.0-----------------------------------------------------------------\n");
      printf(" \x1B[49m\x1B[K ");
      printf("\n");
      for(int i = 0; i <= 100; i++){
    		printf("\rInitialising shell: %d'\%",i);
        fflush(stdout);
        usleep(15000);
    	}

      initiate_globals();
      char *cwd = malloc(sizeof(char)*MAX_BUF_SIZE);
      if(getcwd(cwd,MAX_BUF_SIZE) == NULL){
        printf("ERROR: getting cwd...\n");
      }
      strcat(cwd,"/pilotshell.exe");
      int res = setenv("SHELL",cwd,1);
      if(res != 0){
        printf("ERROR: %s\n",strerror(errno));
        return -1;
      }

    	printf("\nInitialisation complete!!!");
    	printf("\nEnjoy the flight captain!!!\n");
      printf("\n");
      prompt();
      if(!yyparse()){
        printf("ERROR: parser issue\n");
      }
}


void prompt(){
  char *cwd = malloc(sizeof(char)*MAX_BUF_SIZE);
  if(getcwd(cwd,MAX_BUF_SIZE) == NULL){
    printf("ERROR: getting cwd...\n");
  }
  printf(ANSI_COLOR_RED"autoPilot"ANSI_COLOR_RESET"_pid:"ANSI_COLOR_GREEN"%d"ANSI_COLOR_RESET"_@root:%s"ANSI_COLOR_RED"$ "ANSI_COLOR_RESET,getpid(),cwd);
}

/*
 *To execute the parsed command stack
 */
void execute_stack(){

  #ifdef DEBUG
    printf("____________executing stack____________ \n");
  #endif

  int previous_in = dup(INPUT);
  int previous_out = dup(OUTPUT);

  int fd_in,fd_out;

  if(io_redirect_info._type[INPUT] != NULL){
    //using redirected input
    fd_in = open(io_redirect_info._type[INPUT],O_RDONLY);
  }
  else{
    //use original input
    fd_in = dup(previous_in);
  }

  current_node = head;

  for(int i = 0; i < command_stack_current_size && current_node != NULL; i++){
    //redirecting stdin
    dup2(fd_in,INPUT);
    close(fd_in);
    //setting output redirection
    if(i == command_stack_current_size-1){
      if(io_redirect_info._type[OUTPUT] != NULL){
        //use redirected output
        fd_out = open(io_redirect_info._type[OUTPUT],O_RDWR|O_CREAT,0666);
      }
      else{
        //use original output
        fd_out = dup(previous_out);
      }
    }
    else{
      //not the last command being executed, create a pipe and redirect
      int fd_pipe[2];
      pipe(fd_pipe);
      fd_out = fd_pipe[1];
      fd_in = fd_pipe[0];
    }
    //redirecting stdout
    dup2(fd_out,OUTPUT);
    close(fd_out);
    replace_if_alias(current_node->args);
    int custom = execute_custom(current_node->args);
    if(custom == 0){
      int inbuilt = execute_inbuilt(current_node->args);
      if(inbuilt == -1){
        printf("ERROR: no such command found\n");
        break;
      }
    }
    current_node = current_node->next;
  }
  //reseting any IO_redirection done
  dup2(previous_in,INPUT);
  dup2(previous_out,OUTPUT);
  close(previous_in);
  close(previous_out);
}


int execute_custom(char **args){

  #ifdef DEBUG
    printf("____________in execute_custom____________\n");
  #endif

  int present = 0;
  for(int i = 0; i < CUSTOM_COMMAND_COUNT; i++){
    if(strcmp(shell_commands_list[i],*(args)) == 0){
      present = 1;
      history_push(args,getpid(),geteuid());
      (*shell_commands_pointer[i])(args);
      break;
    }
  }
  return present;
}


int execute_inbuilt(char **args){

  #ifdef DEBUG
    printf("____________in execute_inbuilt____________\n");
  #endif

  int res = 0;

  pid_t fork_val;
  pid_t pid;
  uid_t uid;
  fork_val = fork();
  if(fork_val == -1){
    printf("ERROR: issue forking process\n");
  }
  else if(fork_val == 0){
    pid = getpid();
    uid = geteuid();
    res = execvp(args[0],args);
    if(res == -1){
      printf("ERROR: no such command found\n");
    }
    exit(0);
  }
  history_push(args,pid,uid);
  if(!background){
    waitpid(fork_val,NULL,0);
  }
  return res;
}


void shell_reset(){

  #ifdef DEBUG
    printf("____________shell_reset____________\n");
  #endif

  current_node = head;
  while(current_node->next != NULL){
    command_stack_node *temp = current_node->next;
    free(current_node->args);
    free(current_node);
    current_node = temp;
  }
  current_node = NULL;
  head = NULL;
  command_stack_current_size = 0;
  args_current_push_location = 0;
  head = NULL;
  current_node = NULL;
  io_redirect_info._type[INPUT] = NULL;
  io_redirect_info._type[OUTPUT] = NULL;
  io_redirect_info._type[ERROR] = NULL;
  background = false;
}


void replace_if_alias(char **args){

  #ifdef DEBUG
    printf("____________replace_if_alias____________\n");
  #endif

  if(command_alias_head != NULL){
    command_aliases *temp = command_alias_head;

    while(temp != NULL){
      int flag = 0;

      for(int i = 0; i < temp->count; i++){
        if(strcmp(*(args),temp->alias[i]) == 0){
          if(temp->command[0] == '\"'){
              char **command = string_to_command(temp->command);
              int i = 0;

              while(*(command+i) != NULL){
                if(*(args+i) == NULL){
                  args = realloc(args,sizeof(char *)*(i+2));
                }
                *(args+i) = *(command+i);
                i++;
              }
              *(args+i) = NULL;
          }
          else{
            *(args) = temp->command;
          }
          flag = 1;
          break;
        }
        if(flag == 1){
          break;
        }
      }
      temp = temp->next;
    }
  }
}
