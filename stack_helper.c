#include "tpshell.h"


void push_init(){

  #ifdef DEBUG
    printf("____________in push_init____________\n");
  #endif

  if(command_stack_current_size > PIPE_MAX){
    printf("Error: exceeded maximum number of piped commands\n");
    exit(0);
  }

  else{
    args_current_push_location = 0;
    struct command_stack_node *temp = malloc(sizeof(command_stack_node));

    temp->number = command_stack_current_size+1;
    temp->args = NULL;
    temp->args_count = 0;
    temp->next = NULL;

    if(command_stack_current_size == 0){
      head = temp;
      current_node = head;
    }

    else{
      current_node->next = temp;
      current_node = current_node->next;
    }
  }
}


void arg_push(char *arg){

      #ifdef DEBUG
        printf("____________in arg_push____________\n");
      #endif

      char *temp = malloc(strlen(arg)*sizeof(char));
      strcpy(temp,arg);

      if(args_current_push_location == 0){
        current_node->args = malloc(sizeof(char *));
      }
      else{
        current_node->args = realloc(current_node->args,sizeof(char *)*(args_current_push_location+1));
      }
      current_node->args[args_current_push_location] = temp;
      args_current_push_location++;
}

void current_command_args_rev(){

    #ifdef DEBUG
      printf("____________in current_command_args_rev____________\n");
    #endif

    char **temp = malloc(sizeof(char *)*args_current_push_location);

    for(int i = 0; i < args_current_push_location; i++){
      temp[i] = current_node->args[args_current_push_location-i-1];
    }
    temp[args_current_push_location] = NULL;

    //realloc to +1 size to accomodate the NULL at the end.
    current_node->args = (char **)realloc(current_node->args,sizeof(char *)*args_current_push_location);
    //copying the correctly reversed arg_list to the original arg_list
    for(int i = 0; i <= args_current_push_location; i++){
      current_node->args[i] = temp[i];
    }
    current_node->args_count = args_current_push_location;
    //printf("%s\t%s\n",current_node->args[0],current_node->args[1]);
    command_stack_current_size++;
    free(temp);
}

void current_command_display(){

    #ifdef DEBUG
      printf("____________in current_command_display____________\n");
    #endif

    if(current_node == NULL){
        printf("Issue with current_node update\n");
    }
    else{
      char **temp = current_node->args;
      int i = 0;
      while(i < args_current_push_location){
        printf("%s ",*(temp+i));
        i++;
      }
      printf("\n");
    }
}

void command_io_stack_display(){

    #ifdef DEBUG
      printf("____________in command_stack_display____________\n");
    #endif

    command_stack_node *temp;
    temp = head;
    int i = 0;
    while(temp != NULL && i < command_stack_current_size){
      printf("\n%d: ",temp->number);
      char **arg_temp = temp->args;
      int i = 0;
      while(i < temp->args_count){
        printf("%s ",*(arg_temp+i));
        i++;
      }
      printf("\n");
      temp = temp->next;
      i++;
    }
}


void i_o_push(char *val, int type){

    #ifdef DEBUG
      printf("____________in i_o_push____________\n");
    #endif

    char *temp = malloc(sizeof(char)*strlen(val));
    strcpy(temp, val);
    io_redirect_info._type[type] = temp;
}
