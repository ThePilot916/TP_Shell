#include "tpshell.h"

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
  history_current_pointer = 0;
}

void initiate_shell(){

      #ifdef DEBUG
        printf("____________initiating shell____________\n");
      #endif

      printf("\n\t------------------------------------------------------------ThePilot Shell 2.0-----------------------------------------------------------------\n");
    	for(int i = 0; i <= 100; i++){
    		printf("\rInitialising shell: %d",i);
        fflush(stdout);
        sleep(0.01);
    	}
    	printf("\nInitialisation complete!!!");
    	printf("\nEnjoy the flight captain!!!\n");
      initiate_globals();
      prompt();
      if(!yyparse()){
        printf("ERROR: parser issue\n");
      }
}

void prompt(){
  char *cwd = malloc(sizeof(char)*MAX_BUF_SIZE);
  if(getcwd(cwd,MAX_BUF_SIZE) == NULL){
    printf("Error getting cwd...\n");
  }
  printf("autoPilot_pid:%d_@root:%s$ ",getpid(),cwd);
}

/*
 *To execute the parsed command stack
 */

void execute_stack(){

  #ifdef DEBUG
    printf("___________executing stack____________ \n");
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
    printf("%s\n",current_node->args[0]);
    int custom = execute_custom(current_node->args);
    if(custom == 0){
      execute_inbuilt(current_node->args);
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

  pid_t fork_val;
  fork_val = fork();
  if(fork_val == -1){
    printf("ERROR: issue forking process\n");
  }
  else if(fork_val == 0){
    execvp(args[0],args);
  }
  if(!background){
    waitpid(fork_val,NULL,0);
  }
}


void shell_reset(){
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
