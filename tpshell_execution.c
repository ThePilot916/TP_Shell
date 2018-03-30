#include "tpshell.h"

void initiate_globals(){

  #ifdef DEBUG
    printf("In initiate_globals\n");
  #endif

  command_stack_current_size = 0;
  args_current_push_location = 0;
  head = NULL;
  current_node = NULL;
  io_redirect_info._type[INPUT] = NULL;
  io_redirect_info._type[OUTPUT] = NULL;
  io_redirect_info._type[ERROR] = NULL;
}

void prompt(){

    #ifdef DEBUG
      printf("In promt\n");
    #endif

    char *cwd = (char *)malloc(sizeof(char)*MAX_BUF_SIZE);
		if(getcwd(cwd,MAX_BUF_SIZE) == NULL){
			printf("Error getting cwd...\n");
		}
		printf("autoPilot_pid:%d_@root:%s$ ",getpid(),cwd);

}
