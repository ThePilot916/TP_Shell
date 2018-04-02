#include "tp_shell.h"


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
    parsing_quoted_string = false;
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

      if(parsing_quoted_string){
        #ifdef DEBUG
          printf("parsing quoted string: %d\n",args_current_push_location);
        #endif
        int i = 0;
        while(*(quoted_string+i) != NULL){
          i++;
        }
        quoted_string = realloc(quoted_string,sizeof(char *)*(i+2));
        char *temp = malloc(sizeof(char)*(strlen(arg)+1));
        strcpy(temp,arg);
        *(quoted_string+i) = temp;
        *(quoted_string+i+1) = NULL;
      }

      else if(!strchr(arg,'\"')){
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

        if(strchr(arg,'\"') == NULL){
          args_current_push_location++;
        }
      }

      if(strchr(arg,'\"') && parsing_quoted_string == false){
        parsing_quoted_string = true;
        quoted_string = malloc(sizeof(char *)*2);
        char *temp = malloc(sizeof(char)*(strlen(arg)+1));
        strcpy(temp,arg);
        *(quoted_string) = temp;
        *(quoted_string+1) = NULL;
      }
      else if(strchr(arg,'\"') && parsing_quoted_string == true){
        parsing_quoted_string = false;
        quoted_str_rev();
        current_node->args[args_current_push_location] = command_to_string(quoted_string);
        args_current_push_location++;
      }
}


void current_command_args_rev(){

    #ifdef DEBUG
      printf("____________in current_command_args_rev____________\n");
    #endif

    char **temp = malloc((sizeof(char *)*args_current_push_location)+2*(sizeof(char*)));

    for(int i = 0; i < args_current_push_location; i++){
      temp[i] = current_node->args[args_current_push_location-i-1];
    }
    temp[args_current_push_location] = NULL;

    //realloc to +1 size to accomodate the NULL at the end.
    current_node->args = realloc(current_node->args,(sizeof(char *)*args_current_push_location)+sizeof(char *));
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
    //tok_temp[strlen(*(temp+i)+1)] = '\0';
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


char **string_to_command(char *args){

  #ifdef DEBUG
    printf("____________string_to_command____________\n");
  #endif
  //strip the arg of quotes if present

  char *strip_string = malloc(sizeof(char)*strlen(args)+1);
  strcpy(strip_string,args);

  int w = 0, r = 0;
  while (*(strip_string+r) != '\0'){
      if (*(strip_string+r) != '\"'){

          *(strip_string+w) = *(strip_string+r);
          w++;
      }
      r++;
  }
  *(strip_string+w)='\0';

  //tokenize and add to command array
  char **command;
  char *tok;
  tok = strtok(strip_string," ");
  int i = 0;
  while(tok != NULL){
      if(i == 0){
        command = malloc(sizeof(char *)+sizeof(char *));
      }
      else{
        command = realloc(command,(sizeof(char *)*(i+1))+sizeof(char *));
      }
      char *temp = malloc((sizeof(char)*strlen(tok))+1);
      strcpy(temp,tok);
      command[i] = temp;
      i++;
      tok = strtok(NULL," ");
  }

  command[i] = NULL;
  free(strip_string);
  return command;
}


command_aliases *alias_new(char **args){

  #ifdef DEBUG
    printf("____________alias_new____________\n");
  #endif

  char **strip_string;
  strip_string = args;
  command_aliases *temp = malloc(sizeof(command_aliases));
  temp->command = malloc((sizeof(char)*strlen(*(strip_string+1)))+1);
  temp->command[0] = '\0';
  temp->alias[0] =  malloc((sizeof(char)*strlen(*(strip_string+2)))+1);
  strcpy((temp->command),*(strip_string+1));
  strcpy((temp->alias[0]),*(strip_string+2));
  temp->count = 1;
  temp->next = NULL;
  return temp;
}


void alias_display(){

  #ifdef DEBUG
    printf("____________alias_display____________\n");
  #endif

  if(command_alias_head == NULL){
    printf("ERROR: no aliases added\n");
  }
  else{
    command_aliases *temp = command_alias_head;
    printf("Alias Entries=>\n");
    while(temp != NULL){
      printf("%s: ",temp->command);
      for(int i = 0; i < temp->count; i++){
        printf(" %s",temp->alias[i]);
      }
      printf("\n");
      temp = temp->next;
    }
  }
}


void quoted_str_rev(){
  int str_tok_count, i;
  i = 0;
  while(*(quoted_string+i) != NULL){
    i++;
  }
  str_tok_count = i;
  char **temp = malloc(sizeof(char *)*(str_tok_count+2));

  for(int j = 0; j < str_tok_count; j++){
    temp[j] = quoted_string[str_tok_count-j-1];

  }
  for(int j = 0; j < str_tok_count; j++){
    quoted_string[j] = temp[j];
  }
  free(temp);
}
