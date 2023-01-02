%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#define funum 114514

int yylex(void);
void yyerror(char *);

%}

%union{
	int		ivalue;
	float		fvalue;
	char*		svalue;
	past		pAst;
};


%token 	<ivalue>	num_INT 
%token 	<fvalue> 	num_FLOAT
%token 	<svalue> 	Y_ID Y_INT Y_FLOAT Y_VOID Y_CONST 
%token 	<pAst> 		Y_IF Y_ELSE Y_WHILE Y_BREAK Y_CONTINUE 
%token 	<pAst>		Y_RETURN Y_ADD Y_SUB Y_MUL Y_DIV Y_MODULO 
%token	<pAst>		Y_LESS Y_LESSEQ Y_GREAT Y_GREATEQ Y_NOTEQ Y_EQ 
%token	<pAst>		Y_NOT Y_AND Y_OR Y_ASSIGN Y_LPAR Y_RPAR
%token	<pAst>		Y_LBRACKET Y_RBRACKET Y_LSQUARE Y_RSQUARE Y_COMMA Y_SEMICOLON

%type  	<pAst>		PrimaryExp LOrExp LAndExp RelExp AddExp MulExp EqExp CallParams 
%type	<pAst>		UnaryExp LVal ArraySubscripts CompUnit Decl ConstDecl ConstDefs 
%type	<pAst>		ConstDef ConstExps ConstInitVal ConstInitVals VarDecl VarDecls 
%type	<pAst>		VarDef InitVal InitVals FuncDef FuncParams FuncParam Block BlockItems BlockItem Stmt
%type <svalue> Type
%start Start

%%
Start:		
		|CompUnit	{showAst($1,0);}
		;

CompUnit:	Decl CompUnit		{$$=astCompUnit($1,$2);}
		|	FuncDef CompUnit	{$$=astCompUnit($1,$2);}
		|	Decl				{$$=$1;}
		|	FuncDef				{$$=$1;}
		;
		
PrimaryExp:	Y_LPAR AddExp Y_RPAR	{$$ = $2;}
		|	LVal					{$$ = $1;}
		|	num_INT					{$$ = newI($1);}
		|	num_FLOAT				{$$ = newF($1);}
		;

Type:	Y_INT		{$$ = "int";}
		|Y_FLOAT	{$$ = "float";}
		|Y_VOID		{$$ = "void";}
		;
		
LOrExp:	 LAndExp				{$$ = $1;}
		|LAndExp Y_OR LOrExp	{$$ = astLOrExp($1,$3);}
		;
		
LAndExp: EqExp					{$$ = $1;}
		|EqExp Y_AND LAndExp	{$$ = astLAndExp($1,$3);}
		;
		
EqExp:	 RelExp					{$$ = $1;}
		|RelExp Y_EQ EqExp		{$$ = astEqExp("==",$1,$3);}
		|RelExp Y_NOTEQ EqExp	{$$ = astEqExp("!=",$1,$3);}
		;
		
RelExp:	 AddExp						{$$ = $1;}
		|AddExp Y_LESS RelExp		{$$ = astRelExp("<",$1,$3);}
		|AddExp Y_GREAT RelExp		{$$ = astRelExp(">",$1,$3);}
		|AddExp Y_LESSEQ RelExp		{$$ = astRelExp("<=",$1,$3);}
		|AddExp Y_GREATEQ RelExp	{$$ = astRelExp(">=",$1,$3);}
		;
		
AddExp:	 MulExp					{$$ = $1;}
		|MulExp Y_ADD AddExp	{$$ = astAddExp("+",$1,$3);}
		|MulExp Y_SUB AddExp	{$$ = astAddExp("-",$1,$3);}
		;
		
MulExp:	 UnaryExp				{$$ = $1;}
		|UnaryExp Y_MUL MulExp	{$$ = astMulExp("*",$1,$3);}
		|UnaryExp Y_DIV MulExp	{$$ = astMulExp("/",$1,$3);}
		|UnaryExp Y_MODULO MulExp	{$$ = astMulExp("%",$1,$3);}
		;
		
CallParams:	AddExp						{$$ = $1;}
			|AddExp Y_COMMA CallParams	{$$ = astCallParams($1,$3);}
			;
		
UnaryExp:	PrimaryExp						{$$ = $1;}
			|Y_ID Y_LPAR Y_RPAR				{$$ = funcc($1,NULL);}
			|Y_ID Y_LPAR CallParams Y_RPAR	{$$ = funcc($1,$3);}
			|Y_ADD UnaryExp					{$$ = astUnaryExp("+",$2);}
			|Y_SUB UnaryExp					{$$ = astUnaryExp("-",$2);}
			|Y_NOT UnaryExp					{$$ = astUnaryExp("!",$2);}
			;
		
ArraySubscripts:	Y_LSQUARE AddExp Y_RSQUARE					{$$ = astArray($2,NULL);}
					|Y_LSQUARE AddExp Y_RSQUARE ArraySubscripts	{$$ = astArray($2,$4);}
					;
			
LVal:	Y_ID					{$$=newS($1);}
		|Y_ID ArraySubscripts	{$$=astLVal($1,$2);}
		;
		
		
Decl:	ConstDecl	{$$=$1;}
		|VarDecl	{$$=$1;}
		;
		
ConstDecl:	Y_CONST Type ConstDef Y_SEMICOLON	{$$=astConstDecl($2,$3);}
			|Y_CONST Type ConstDefs Y_SEMICOLON	{$$=astConstDecl($2,$3);}
			;
		
ConstDefs:	ConstDef Y_COMMA ConstDef	{$$=astConstDefs($1,$3);}
			|ConstDef Y_COMMA ConstDefs	{$$=astConstDefs($1,$3);}
			;
		
ConstDef:	Y_ID Y_ASSIGN ConstInitVal				{$$=astConstDef($1,$3);}
			|Y_ID ConstExps Y_ASSIGN ConstInitVal	{$$=astConstDef($1,$4);}
			;
		
ConstExps:	Y_LSQUARE AddExp Y_RSQUARE				{$$=$2;}
			|Y_LSQUARE AddExp Y_RSQUARE ConstExps	{$$=$2;}
			;
		
ConstInitVal:	AddExp												{$$=$1;}
				|Y_LBRACKET Y_RBRACKET								{$$=astConstInitVal(NULL,NULL);}
				|Y_LBRACKET ConstInitVal Y_RBRACKET					{$$=astConstInitVal($2,NULL);}
				|Y_LBRACKET ConstInitVal ConstInitVals Y_RBRACKET	{$$=astConstInitVal($2,$3);}
				;
		
ConstInitVals:	Y_COMMA ConstInitVal				{$$=$2;}
				|Y_COMMA ConstInitVal ConstInitVals	{$$=astConstInitVals($2,$3);}
				;
		
VarDecl:	Type VarDef Y_SEMICOLON				{$$=astVarDecl($1,$2,NULL);}
			|Type VarDef VarDecls Y_SEMICOLON	{$$=astVarDecl($1,$2,$3);}
			;
		
VarDecls:	Y_COMMA VarDef				{$$=$2;}
			|Y_COMMA VarDef VarDecls	{$$=astVarDecls($2,$3);}
			;
		
VarDef:		Y_ID								{$$=newS($1);}
			|Y_ID Y_ASSIGN InitVal				{$$=astVarDef($1,$3);}
			|Y_ID ConstExps						{$$=newS($1);}
			|Y_ID ConstExps Y_ASSIGN InitVal	{$$=astVarDef($1,$4);}
			;
		
InitVal:	AddExp									{$$=$1;}
			|Y_LBRACKET Y_RBRACKET					{$$=astInitVal(NULL,NULL);}
			|Y_LBRACKET InitVal Y_RBRACKET			{$$=astInitVal($2,NULL);}
			|Y_LBRACKET InitVal InitVals Y_RBRACKET	{$$=astInitVal($2,$3);}
			;
		
InitVals:	Y_COMMA InitVal				{$$=$2;}
			|Y_COMMA InitVal InitVals	{$$=astInitVals($2,$3);}
			;
		
FuncDef:	Type Y_ID Y_LPAR Y_RPAR Block				{$$=astFuncDef($1,$2,NULL,$5);}
			|Type Y_ID Y_LPAR FuncParams Y_RPAR Block	{$$=astFuncDef($1,$2,$4,$6);}
			;
		
FuncParams:	FuncParam						{$$=$1;}
			|FuncParam Y_COMMA FuncParams	{$$=astFuncParams($1,$3);}
			;
		
FuncParam:	Type Y_ID						{$$=astFuncParam($1,$2);}
			|Type Y_ID Y_LSQUARE Y_RSQUARE	{$$=astFuncParam($1,$2);}
			|Type Y_ID ArraySubscripts						{$$=astFuncParam($1,$2);}
			|Type Y_ID Y_LSQUARE Y_RSQUARE ArraySubscripts	{$$=astFuncParam($1,$2);}
			;
		
Block:	Y_LBRACKET BlockItems Y_RBRACKET	{$$=astBlock($2);}
		|Y_LBRACKET Y_RBRACKET				{$$=astBlock(NULL);}
		;
		
BlockItems:	BlockItem				{$$=$1;}
			|BlockItem BlockItems	{$$=astBlockItems($1,$2);}
			;
		
BlockItem:	Decl	{$$=$1;}
			|Stmt	{$$=$1;}
			;
		
Stmt:	LVal Y_ASSIGN AddExp Y_SEMICOLON	{$$=astStmt1($1,$3);}
		|Y_SEMICOLON						{$$=astStmt2();}
		|AddExp Y_SEMICOLON					{$$=$1;}
		|Block								{$$=$1;}
		|Y_WHILE Y_LPAR LOrExp Y_RPAR Stmt	{$$=astwhile($3,$5);}
		|Y_IF Y_LPAR LOrExp Y_RPAR Stmt		{$$=astif($3,$5,NULL);}
		|Y_IF Y_LPAR LOrExp Y_RPAR Stmt Y_ELSE Stmt	{$$=astif($3,$5,$7);}
		|Y_BREAK Y_SEMICOLON			{$$=astbreak();}
		|Y_CONTINUE Y_SEMICOLON			{$$=astcontinue();}
		|Y_RETURN AddExp Y_SEMICOLON	{$$=astreturn($2);}
		|Y_RETURN Y_SEMICOLON			{$$=astreturn(NULL);}
		;

%%

