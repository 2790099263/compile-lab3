%{
#include <stdlib.h>
#include "ast.h"
#include "lrparser.tab.h"


void yyerror(char *);
%}

%%
"int"	{return Y_INT;}
"float"	{return Y_FLOAT;}
"void"	{return Y_VOID;}
"const"	{return Y_CONST;}
"if"	{return Y_IF;}
"else"	{return Y_ELSE;}
"while"	{return Y_WHILE;}
"break"	{return Y_BREAK;}
"continue"	{return Y_CONTINUE;}
"return"	{return Y_RETURN;}
[0-9]+	{ yylval.ivalue = atoi(yytext);return num_INT; }
[0-9]+[eE][+-]?[0-9]+	{yylval.fvalue=strtof(yytext,NULL);return num_FLOAT;}
[0-9]*"."[0-9]*([eE][+-]?[0-9]+)?	{ yylval.fvalue = strtof(yytext,NULL);return num_FLOAT;}
[_A-Za-z]+[_a-zA-Z0-9]*	{yylval.svalue=malloc(strlen(yytext)+1);strcpy(yylval.svalue,yytext);return Y_ID;}
"0"+[xX0-9A-Fa-f]+ {yylval.ivalue = atoi(yytext);return num_INT;}
"0"[xX](([0-9A-Fa-f]*"."[0-9A-Fa-f]*)|[0-9a-fA-F]*)[Pp][+-]?[0-9]+ {yylval.fvalue=strtof(yytext,NULL);return num_FLOAT;}
"+"	{ return Y_ADD; }
"-"	{ return Y_SUB; }
"*"	{ return Y_MUL; }
"/"	{ return Y_DIV; }
"%"	{return Y_MODULO;}
"<"	{return Y_LESS;}
"<="	{return Y_LESSEQ;}
">"	{return Y_GREAT;}
">="	{return Y_GREATEQ;}
"!="	{return Y_NOTEQ;}
"=="	{return Y_EQ;}
"!"	{return Y_NOT;}
"&&"	{return Y_AND;}
"||"	{return Y_OR;}
"="	{return Y_ASSIGN;}

"("	{return Y_LPAR;}
")"	{return Y_RPAR;}
"{"	{return Y_LBRACKET;}
"}"	{return Y_RBRACKET;}
"["	{return Y_LSQUARE;}
"]"	{return Y_RSQUARE;}
","	{return Y_COMMA;}
";"	{return Y_SEMICOLON;}

"/*"((("*"[^/])?|[^*])*\n?)*("*/")	{}
[ \t\n]	{}
"//".*[\n] {}


%%

int yywrap(void) 
{
	return 1;
}
