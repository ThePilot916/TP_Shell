#include "tpshell.h"


void push_init(){

  #ifdef DEBUG
    printf("In push_init\n");
  #endif

  if(command_stack_current_size > PIPE_MAX){
    printf("Error: exceeded maximum number of piped commands\n");
    exit(0);
  }

  else{
    args_current_push_location = 0;
    command_stack_node *temp = malloc(sizeof(command_stack_node));

    temp->number = command_stack_current_size+1;
    temp->args = NULL;
    temp->next = NULL;

    if(command_stack_current_size == 0){
      head = temp;
      current_node = head;
    }
    else{
      current_node->next = temp;
      current_node = current_node->next;
    }
    command_stack_current_size++;
  }
}


void arg_push(char *arg){

      #ifdef DEBUG
        printf("In arg_push\n");
      #endif

      char *temp = malloc(strlen(arg)*sizeof(char));
      strcpy(temp,arg);
      if(current_node == NULL){
        printf("Its null you retard\n");
      }
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
      printf("In current_command_args_rev\n");
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

    free(temp);
}

void current_command_display(){

    #ifdef DEBUG
      printf("In current_command_display\n");
    #endif

    if(current_node == NULL){
        printf("Issue with current_node update\n");
    }
    else{
      char **temp = current_node->args;
      printf("CMD: ");
      while(temp != NULL){
        printf("%s",*temp);
      }
      temp++;
      printf("\n");
    }
}

void command_io_stack_display(){

    #ifdef DEBUG
      printf("In command_stack_display\n");
    #endif

    command_stack_node *temp;
    temp = head;
    while(temp != NULL){
      printf("\n%d: ",temp->number);
      char **arg_temp = temp->args;
      while(arg_temp != NULL){
        printf("%s",*arg_temp);
        arg_temp++;
      }
      printf("\n");
      temp = temp->next;
    }
}


void i_o_push(char *val, int type){

    #ifdef DEBUG
      printf("in i_o_push\n");
    #endif

    char *temp = malloc(sizeof(char)*strlen(val));
    strcpy(temp, val);
    io_redirect_info._type[type] = temp;
}
