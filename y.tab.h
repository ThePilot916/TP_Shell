#define GREAT 257
#define GREATx2 258
#define NEWLINE 259
#define WORD 260
#define GREATAMP 261
#define AMP 262
#define PIPE 263
#define LESS 264
#define NOTOKEN 265
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union{
	char *str;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;
