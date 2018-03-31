%{

	#include "tpshell.h"
	void yyerror(const char *);
	int yylex();
	extern char yytext[];
%}

%union{
	char *str;
}

%token GREAT GREATx2 NEWLINE WORD GREATAMP AMP PIPE LESS
%token <str> WORD



%%
command_list:
						|command_list command_line
						;

command_line: pipe_list io_modifier_list background_opt NEWLINE  {
																																	#ifdef DEBUG
																																		printf("**In command_line*********\n");
																																		printf("\t|\n");
																																		command_io_stack_display();
																																	#endif

																																	execute_stack();
																																	shell_reset();
																																	prompt();
																																}
						| NEWLINE                                            {
																																	#ifdef DEBUG
																																		printf("**In command_line*********\n");
																																		printf("\t|\n");
																																	#endif
																																	prompt();
																																}
						;

io_modifier_list: io_modifier_list io_modifier
								|
								;


io_modifier: GREATx2 WORD  {i_o_push($2, ERROR);}
						| GREAT WORD    {i_o_push($2, OUTPUT);}
						| GREATx2 AMP WORD {i_o_push($3,ERROR);}
						| GREATAMP WORD  {i_o_push($2, OUTPUT);}
						| LESS WORD  {i_o_push($2,INPUT);}
						;

pipe_list: pipe_list PIPE cmd_args {
																				#ifdef DEBUG
																					printf("**In pipe_list\n");
																					printf("\t|\n");
																				#endif

																				 //command_stack_current_size++;
																				 //printf("after1: %d\n",command_stack_current_size);
																		}
					| cmd_args                  {
																				 #ifdef DEBUG
																					 printf("**In pipe_list\n");
																					 printf("\t|\n");
																				 #endif

																				 //printf("before2: %d\n",command_stack_current_size);
																				 //command_stack_current_size++;
																				 //printf("after2: %d\n",command_stack_current_size);
																		}
					 ;

 cmd_args: WORD arg_list  {
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

arg_list: WORD arg_list {
														#ifdef DEBUG
															printf("**In arg_list*********\n");
															printf("\t|\n");
														#endif
														if(command_stack_current_size == 0 && args_current_push_location == 0){
														 push_init();
														}
														arg_push($1);
													}
				|
				;

background_opt: AMP        {
														if(current_node != NULL)
															background = true;
													}
							|
							;


%%

void yyerror(const char *error){
	printf("ERROR: %s\n",error);
}
