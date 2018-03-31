%{
	#include "y.tab.h"
	#include "tp_shell.h"
	void yyerror(const char *);

%}

%%

\n		return NEWLINE;
[ \t]	;
">"		return GREAT;
"<"		return LESS;
">>"	return GREATx2;
">&"	return GREATAMP;
"|"		return PIPE;
"&"		return AMP;
[^ \t\n][^ \t\n]*	{yylval.str = strdup(yytext);
					return WORD;}

%%
