%{

  #include "tp_shell.h"
  void yyerror(const char *);
  int yylex();
  extern char yytext[];
%}

%union{
  char *str;
}

%token GREAT GREATx2 NEWLINE WORD GREATAMP AMP PIPE LESS QUOTE MULTLINE ARROW_DW QUOTE_STR
%token <str> WORD
%token <str> QUOTE_STR




%%

command_line:pipe_list multiline io_modifier_list multiline background_opt NEWLINE  {
                                                                  #ifdef DEBUG
                                                                    printf("**In command_line*********\n");
                                                                    printf("\t|\n");
                                                                    command_io_stack_display();
                                                                  #endif

                                                                  execute_stack();
                                                                  //shell_reset();
                                                                 // prompt();
								exit(0);
                                                                }
            ;

io_modifier_list: io_modifier_list multiline io_modifier multiline
                |
                ;


io_modifier: GREATx2 multiline WORD multiline  {i_o_push($3, ERROR);}
            | GREAT multiline WORD multiline    {i_o_push($3, OUTPUT);}
            | GREATx2 multiline AMP multiline WORD multiline {i_o_push($5,ERROR);}
            | GREATAMP multiline WORD multiline  {i_o_push($3, OUTPUT);}
            | LESS multiline WORD multiline  {i_o_push($3,INPUT);}
            ;

pipe_list: pipe_list multiline PIPE multiline cmd_args multiline {
                                        #ifdef DEBUG
                                          printf("**In pipe_list\n");
                                          printf("\t|\n");
                                        #endif
                                    }
          | cmd_args multiline               {
                                         #ifdef DEBUG
                                           printf("**In pipe_list\n");
                                           printf("\t|\n");
                                         #endif
                                    }
           ;

 cmd_args: WORD multiline string multiline arg_list multiline  {
                            #ifdef DEBUG
                              printf("**In cmd_args*********\n");
                              printf("\t|\n");
                            #endif


                              if(command_stack_current_size == 0 && args_current_push_location == 0){
                                  push_init();
                              }
                              arg_push($1);
                              current_command_args_rev();
                              push_init();

                            #ifdef DEBUG
                               current_command_display();
                            #endif
                            }
         ;

arg_list: arg_list multiline WORD multiline {
                            #ifdef DEBUG
                              printf("**In arg_list*********\n");
                              printf("\t|\n");
                            #endif
                            if(command_stack_current_size == 0 && args_current_push_location == 0){
                              push_init();
                            }
                            arg_push($3);
                          }
        |
        ;

background_opt: AMP multiline        {
                            if(current_node != NULL)
                              background = true;
                          }
              |
              ;
multiline: MULTLINE
         |
         ;
string: QUOTE_STR { if(command_stack_current_size == 0 && args_current_push_location == 0){
                    push_init();
                  }
                    arg_push($1);}
      |
      ;

%%

void yyerror(const char *error){
  printf("ERROR: %s\n",error);
}
