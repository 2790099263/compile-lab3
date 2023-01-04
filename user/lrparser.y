%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
extern past root;
int yylex(void);
void yyerror(char *);

%}

%union{
	int		ivalue;
	float		fvalue;
	char*		svalue;
	past		pAst;
};


%token <ivalue> num_INT 
%token <fvalue> num_FLOAT
%token <svalue> Y_ID Y_INT Y_FLOAT Y_VOID Y_CONST 
%token <pAst> Y_IF Y_ELSE Y_WHILE Y_BREAK Y_CONTINUE Y_RETURN Y_ADD Y_SUB Y_MUL Y_DIV Y_MODULO Y_LESS Y_LESSEQ Y_GREAT Y_GREATEQ Y_NOTEQ Y_EQ Y_NOT Y_AND Y_OR Y_ASSIGN Y_LPAR Y_RPAR Y_LBRACKET Y_RBRACKET Y_LSQUARE Y_RSQUARE Y_COMMA Y_SEMICOLON

%type  <pAst>	PrimaryExp LOrExp LAndExp RelExp AddExp MulExp EqExp CallParams UnaryExp LVal ArraySubscripts CompUnit Decl ConstDecl ConstDefs ConstDef ConstExps ConstInitVal ConstInitVals VarDecl VarDecls VarDef InitVal InitVals FuncDef FuncParams FuncParam Block BlockItems BlockItem Stmt
%type <svalue> Type
%start Start

%%
Start:		
		|CompUnit	{showAst($1,0);}
		;
		
PrimaryExp:	Y_LPAR AddExp Y_RPAR	{$$ = $2;}
		|LVal		{$$ = $1;}
		|num_INT	{$$ = newInteger($1);}
		|num_FLOAT	{$$ = newFloat($1);}
		;

Type:		Y_INT	{$$ = "int";}
		|Y_FLOAT	{$$ = "float";}
		|Y_VOID		{$$ = "void";}
		;
		
LOrExp:		LAndExp		{$$ = $1;}
		|LOrExp Y_OR LAndExp	{$$ = doLOrExp($1,$3);}
		;
		
LAndExp:	EqExp		{$$ = $1;}
		|LAndExp Y_AND EqExp	{$$ = doLAndExp($1,$3);}
		;
		
EqExp:		RelExp		{$$ = $1;}
		|EqExp Y_EQ RelExp	{$$ = doEqExp("==",$1,$3);}
		|EqExp Y_NOTEQ RelExp	{$$ = doEqExp("!=",$1,$3);}
		;
		
RelExp:		AddExp		{$$ = $1;}
		|RelExp Y_LESS AddExp		{$$ = doRelExp("<",$1,$3);}
		|RelExp Y_GREAT AddExp		{$$ = doRelExp(">",$1,$3);}
		|RelExp Y_LESSEQ AddExp		{$$ = doRelExp("<=",$1,$3);}
		|RelExp Y_GREATEQ AddExp	{$$ = doRelExp(">=",$1,$3);}
		;
		
AddExp:		MulExp		{$$ = $1;}
		|AddExp Y_ADD MulExp	{$$ = doAddExp("+",$1,$3);}
		|AddExp Y_SUB MulExp	{$$ = doAddExp("-",$1,$3);}
		;
		
MulExp:		UnaryExp	{$$ = $1;}
		|MulExp Y_MUL UnaryExp	{$$ = doMulExp("*",$1,$3);}
		|MulExp Y_DIV UnaryExp	{$$ = doMulExp("/",$1,$3);}
		|MulExp Y_MODULO UnaryExp	{$$ = doMulExp("%",$1,$3);}
		;
		
CallParams:	AddExp	{$$ = $1;}
		|AddExp Y_COMMA CallParams	{$$ = doCallParams($1,$3);}
		;
		
UnaryExp:	PrimaryExp	{$$ = $1;}
		|Y_ID Y_LPAR Y_RPAR	{$$ = funcc($1,NULL);}
		|Y_ID Y_LPAR CallParams Y_RPAR	{$$ = funcc($1,$3);}
		|Y_ADD UnaryExp		{$$ = doUnaryExp("+",$2);}
		|Y_SUB UnaryExp		{$$ = doUnaryExp("-",$2);}
		|Y_NOT UnaryExp		{$$ = doUnaryExp("!",$2);}
		;
		
ArraySubscripts:	Y_LSQUARE AddExp Y_RSQUARE	{$$ = doArray($2,NULL);}
			|Y_LSQUARE AddExp Y_RSQUARE ArraySubscripts	{$$ = doArray($2,$4);}
			;
			
LVal:		Y_ID		{$$=newString($1);}
		|Y_ID ArraySubscripts	{$$=doLVal($1,$2);}
		;
		


CompUnit:	Decl CompUnit		{$$=doCompUnit($1,$2);}
		|FuncDef CompUnit	{$$=doCompUnit($1,$2);}
		|Decl			{$$=$1;}
		|FuncDef		{$$=$1;}
		;
		
Decl:		ConstDecl	{$$=$1;}
		|VarDecl	{$$=$1;}
		;
		
ConstDecl:	Y_CONST Type ConstDef Y_SEMICOLON	{$$=doConstDecl($2,$3);}
		|Y_CONST Type ConstDefs Y_SEMICOLON	{$$=doConstDecl($2,$3);}
		;
		
ConstDefs:	ConstDef Y_COMMA ConstDef	{$$=doConstDefs($1,$3);}
		|ConstDef Y_COMMA ConstDefs	{$$=doConstDefs($1,$3);}
		;
		
ConstDef:	Y_ID Y_ASSIGN ConstInitVal	{$$=doConstDef($1,$3);}
		|Y_ID ConstExps Y_ASSIGN ConstInitVal	{$$=doConstDef($1,$4);}
		;
		
ConstExps:	Y_LSQUARE AddExp Y_RSQUARE		{$$=$2;}
		|Y_LSQUARE AddExp Y_RSQUARE ConstExps	{$$=$2;}
		;
		
ConstInitVal:	AddExp		{$$=$1;}
		|Y_LBRACKET Y_RBRACKET		{$$=doConstInitVal(NULL,NULL);}
		|Y_LBRACKET ConstInitVal Y_RBRACKET	{$$=doConstInitVal($2,NULL);}
		|Y_LBRACKET ConstInitVal ConstInitVals Y_RBRACKET	{$$=doConstInitVal($2,$3);}
		;
		
ConstInitVals:	Y_COMMA ConstInitVal		{$$=$2;}
		|Y_COMMA ConstInitVal ConstInitVals	{$$=doConstInitVals($2,$3);}
		;
		
VarDecl:	Type VarDef Y_SEMICOLON		{$$=doVarDecl($1,$2,NULL);}
		|Type VarDef VarDecls Y_SEMICOLON	{$$=doVarDecl($1,$2,$3);}
		;
		
VarDecls:	Y_COMMA VarDef		{$$=$2;}
		|Y_COMMA VarDef VarDecls	{$$=doVarDecls($2,$3);}
		;
		
VarDef:		Y_ID	{$$=newString($1);}
		|Y_ID Y_ASSIGN InitVal	{$$=doVarDef($1,$3);}
		|Y_ID ConstExps		{$$=newString($1);}
		|Y_ID ConstExps Y_ASSIGN InitVal	{$$=doVarDef($1,$4);}
		;
		
InitVal:	AddExp		{$$=$1;}
		|Y_LBRACKET Y_RBRACKET		{$$=doInitVal(NULL,NULL);}
		|Y_LBRACKET InitVal Y_RBRACKET		{$$=doInitVal($2,NULL);}
		|Y_LBRACKET InitVal InitVals Y_RBRACKET	{$$=doInitVal($2,$3);}
		;
		
InitVals:	Y_COMMA InitVal		{$$=$2;}
		|Y_COMMA InitVal InitVals	{$$=doInitVals($2,$3);}
		;
		
FuncDef:	Type Y_ID Y_LPAR Y_RPAR Block	{$$=doFuncDef($1,$2,NULL,$5);}
		|Type Y_ID Y_LPAR FuncParams Y_RPAR Block	{$$=doFuncDef($1,$2,$4,$6);}
		;
		
FuncParams:	FuncParam	{$$=$1;}
		|FuncParam Y_COMMA FuncParams	{$$=doFuncParams($1,$3);}
		;
		
FuncParam:	Type Y_ID	{$$=doFuncParam($1,$2);}
		|Type Y_ID Y_LSQUARE Y_RSQUARE	{$$=doFuncParam($1,$2);}
		|Type Y_ID ArraySubscripts	{$$=doFuncParam($1,$2);}
		|Type Y_ID Y_LSQUARE Y_RSQUARE ArraySubscripts	{$$=doFuncParam($1,$2);}
		;
		
Block:		Y_LBRACKET BlockItems Y_RBRACKET	{$$=doBlock($2);}
		|Y_LBRACKET Y_RBRACKET		{$$=doBlock(NULL);}
		;
		
BlockItems:	BlockItem	{$$=$1;}
		|BlockItem BlockItems	{$$=doBlockItems($1,$2);}
		;
		
BlockItem:	Decl	{$$=$1;}
		|Stmt	{$$=$1;}
		;
		
Stmt:		LVal Y_ASSIGN AddExp Y_SEMICOLON	{$$=doStmt1($1,$3);}
		|Y_SEMICOLON	{$$=doStmt2();}
		|AddExp Y_SEMICOLON	{$$=$1;}
		|Block		{$$=$1;}
		|Y_WHILE Y_LPAR LOrExp Y_RPAR Stmt	{$$=dowhile($3,$5);}
		|Y_IF Y_LPAR LOrExp Y_RPAR Stmt		{$$=doif($3,$5,NULL);}
		|Y_IF Y_LPAR LOrExp Y_RPAR Stmt Y_ELSE Stmt	{$$=doif($3,$5,$7);}
		|Y_BREAK Y_SEMICOLON		{$$=dobreak();}
		|Y_CONTINUE Y_SEMICOLON		{$$=docontinue();}
		|Y_RETURN AddExp Y_SEMICOLON	{$$=doreturn($2);}
		|Y_RETURN Y_SEMICOLON		{$$=doreturn(NULL);}
		;
		

%%

