%{

	#include "tpshell.h"
	void yyerror(const char *);
	int yylex();
	extern char yytext[];
%}

%union{
	char *str;
}

%token GREAT,GREATx2,NEWLINE,WORD,GREATAMP,AMP,PIPE,LESS,NOTOKEN
%token <str> WORD

%%

 cmd_args: WORD arg_list	{
 														#ifdef DEBUG
															printf("In cmd_args\n");
														#endif
														if(command_stack_current_size == 0){
															push_init();
														}
 														arg_push($1);
 													}
 				;

arg_list: arg_list WORD  {
														#ifdef DEBUG
															printf("In arg_list\n");
														#endif
														if(command_stack_current_size == 0){
															push_init();
														}
															arg_push($2);
															printf("%s\n",current_node->args[0]);
													}
				|
				;

pipe_list: pipe_list PIPE cmd_args {
																				#ifdef DEBUG
																					printf("In pipe_list, about to push node\n");
																				#endif
																				current_command_args_rev();
																				command_stack_current_size++;
																				current_command_display();
																	  }
		 		 | cmd_args									{
						 														#ifdef DEBUG
																					 printf("In pipe_list, about to push node\n");
																				 #endif
																				 current_command_args_rev();
																				 command_stack_current_size++;
																				 current_command_display();
																		}
		 	 	 ;

io_modifier: GREATx2 WORD	{i_o_push($2, ERROR);}
		   		 | GREAT WORD		{i_o_push($2, OUTPUT);}
		   		 | GREATx2 AMP WORD {i_o_push($3,ERROR);}
		   		 | GREATAMP WORD	{i_o_push($2, OUTPUT);}
		   		 | LESS WORD	{i_o_push($2,INPUT);}
		   		 ;

io_modifier_list: io_modifier_list io_modifier
								|
								;

background_opt: AMP
			  			|
			  			;

command_line: pipe_list io_modifier_list background_opt NEWLINE	{
																																	#ifdef DEBUG
																																		printf("In command_line\n");
																																	#endif
																																	prompt();
																																	command_io_stack_display();
																																	prompt();

																																}
						| NEWLINE																						{
																																	#ifdef DEBUG
																																		printf("In command_line\n");
																																	#endif
																																	prompt();
																																}
						;


%%

void yyerror(const char *error){
	printf("ERROR: %s\n",error);
}
