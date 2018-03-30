#include "tpshell.h"

void initiate_globals(){

  #ifdef DEBUG
    printf("initiating globals\n");
  #endif

  command_stack_current_size = 0;
  args_current_push_location = 0;
  head = NULL;
  current_node = NULL;
  io_redirect_info._type[INPUT] = NULL;
  io_redirect_info._type[OUTPUT] = NULL;
  io_redirect_info._type[ERROR] = NULL;
}

void initiate_shell(){
    while(1){
      initiate_globals();
      #ifdef DEBUG
        printf("initiating shell\n");
      #endif
      prompt();
      if(!yyparse()){
        printf("ERROR: parser issue\n");
      }
    }
}

void prompt(){
  char *cwd = malloc(sizeof(char)*MAX_BUF_SIZE);
  if(getcwd(cwd,MAX_BUF_SIZE) == NULL){
    printf("Error getting cwd...\n");
  }
  printf("autoPilot_pid:%d_@root:%s$ ",getpid(),cwd);
}
