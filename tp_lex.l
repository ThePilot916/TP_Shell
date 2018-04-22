%{
	#include "y.tab.h"
	#include "tp_shell.h"
	void yyerror(const char *);

%}

%%
\"(\\.|[^"\\])*\" {yylval.str = strdup(yytext); return QUOTE_STR;}
"\\\n" return MULTLINE;
\n		return NEWLINE;
[ \t]	;
'\^\[\[B' return ARROW_DW;
">"		return GREAT;
"<"		return LESS;
">>"	return GREATx2;
">&"	return GREATAMP;
"|"		return PIPE;
"&"		return AMP;
[^ \t\n][^ \t\n]*	{yylval.str = strdup(yytext);
					return WORD;}

%%
