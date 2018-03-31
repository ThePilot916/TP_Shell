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

      char *temp;
      temp = malloc((strlen(arg)*sizeof(char))+1);
      strcpy(temp,arg);
      temp[strlen(arg)] = '\0';
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

    char **temp = malloc((sizeof(char *)*args_current_push_location)+1);

    for(int i = 0; i < args_current_push_location; i++){
      temp[i] = current_node->args[args_current_push_location-i-1];
    }
    temp[args_current_push_location] = NULL;

    //realloc to +1 size to accomodate the NULL at the end.
    current_node->args = (char **)realloc(current_node->args,(sizeof(char *)*args_current_push_location)+1);
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


void history_push(char **args,pid_t pid, uid_t uid){

  #ifdef DEBUG
    printf("____________history_push____________\n");
  #endif

  command_hist[history_current_push_pointer]._command = command_to_string(args);
  command_hist[history_current_push_pointer]._timeinfo = localtime(&current_time);
  command_hist[history_current_push_pointer]._process_info = pid;
  command_hist[history_current_push_pointer]._user_info = uid;
  total_executed++;
  history_current_push_pointer = ((history_current_push_pointer+1)%HISTORY_MAX);
}

char *command_to_string(char **args){

  #ifdef DEBUG
    printf("____________command_to_string____________\n");
  #endif

  char *result;
  char **temp;
  char *tok_temp;
  temp = args;
  int i = 0;
  int string_size = 0;
  while(*(temp+i) != NULL){
    tok_temp = malloc((sizeof(char)*strlen(*(temp+i)))+2);
    tok_temp[0] = '\0';
    strcpy(tok_temp,*(temp+i));
    strcat(tok_temp," ");
    tok_temp[strlen(*(temp+i)+1)] = '\0';
    if(string_size == 0){
      string_size += strlen(tok_temp);
      result = malloc(sizeof(char)*string_size+1);
      result[0] = '\0';
    }
    else{
      string_size += strlen(tok_temp);
      result = realloc(result,(sizeof(char)*string_size)+1);
    }
    strcat(result,tok_temp);
    free(tok_temp);
    i++;
  }
  return result;
}


command_aliases *alias_new(char **args){

  #ifdef DEBUG
    printf("____________alias_new____________\n");
  #endif

  char **str_temp;
  str_temp = args;
  printf("1\n");
  command_aliases *temp = malloc(sizeof(command_aliases));
    printf("2\n");
  temp->command = malloc((sizeof(char)*strlen(*(str_temp+1)))+1);
  temp->command[0] = '\0';
  temp->alias[0] =  malloc((sizeof(char)*strlen(*(str_temp+2)))+1);
    printf("3\n");
  strcpy((temp->command),*(str_temp+1));
    printf("4\n");
  strcpy((temp->alias[0]),*(str_temp+2));
    printf("5\n");
  temp->count = 1;
  temp->next = NULL;
  return temp;
}

void alias_display(){

  if(command_alias_head == NULL){
    printf("ERROR: no aliases added\n");
  }
  else{
    command_aliases *temp = command_alias_head;
    while(temp != NULL){
      printf("CMD: %s Aliases =>",temp->command);
      for(int i = 0; i < temp->count; i++){
        printf("\t%s\n",temp->alias[i]);
      }
      temp = temp->next;
    }
  }
}
