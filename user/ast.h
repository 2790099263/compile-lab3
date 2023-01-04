#define _AST_H
#ifdef	_AST_H
#include "node_type.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define funum 114514
typedef struct _ast ast;
typedef struct _ast *past;

struct _ast{
	int 		ivalue;
	float 		fvalue;
	char* 		svalue;
	node_type 	nodeType;
	past 		left;
	past 		right;
	past 		next;
};

past doLVal(char* s,past array);
past doArray(past a,past b);
past doUnaryExp(char* s,past unary);
past funcc(char* s,past call);
past doCallParams(past add,past call);
past doMulExp(char* s,past unary,past mul);
past doAddExp(char* s,past mul,past add);
past doRelExp(char* s,past add,past rel);
past doEqExp(char* s,past rel,past eq);
past doLAndExp(past eq,past land);
past doLOrExp(past land,past lor);
past newString(char* value);
past newFloat(float value);
past newInteger(int value);
past newAstNode();
void showAst(past node, int nest);
past doCompUnit(past a,past b);
past doConstDecl(char* s,past def);
past doConstDefs(past a,past b);
past doConstDef(char* s,past init);
past doConstInitVal(past a,past b);
past doConstInitVals(past a,past b);
past doVarDecl(char* s,past a,past b);
past doVarDecls(past a,past b);
past doVarDef(char* s,past a);
past doInitVal(past a,past b);
past doInitVals(past a,past b);
past doFuncDef(char* s,char* s2,past a,past b);
past doFuncParams(past a,past b);
past doFuncParam(char* s,char* s2);
past doBlock(past a);
past doBlockItems(past a,past b);
past doStmt1(past a,past b);
past doStmt2(void);
past dowhile(past a,past b);
past doif(past a,past b,past c);
past dobreak(void);
past docontinue(void);
past doreturn(past a);
#endif