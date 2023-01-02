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

past astLVal(char* s,past array);
past astArray(past a,past b);
past astUnaryExp(char* s,past unary);
past funcc(char* s,past call);
past astCallParams(past add,past call);
past astMulExp(char* s,past unary,past mul);
past astAddExp(char* s,past mul,past add);
past astRelExp(char* s,past add,past rel);
past astEqExp(char* s,past rel,past eq);
past astLAndExp(past eq,past land);
past astLOrExp(past land,past lor);
past newS(char* value);
past newF(float value);
past newI(int value);
past newAstNode();
void showAst(past node, int nest);
past astCompUnit(past a,past b);
past astConstDecl(char* s,past def);
past astConstDefs(past a,past b);
past astConstDef(char* s,past init);
past astConstInitVal(past a,past b);
past astConstInitVals(past a,past b);
past astVarDecl(char* s,past a,past b);
past astVarDecls(past a,past b);
past astVarDef(char* s,past a);
past astInitVal(past a,past b);
past astInitVals(past a,past b);
past astFuncDef(char* s,char* s2,past a,past b);
past astFuncParams(past a,past b);
past astFuncParam(char* s,char* s2);
past astBlock(past a);
past astBlockItems(past a,past b);
past astStmt1(past a,past b);
past astStmt2(void);
past astwhile(past a,past b);
past astif(past a,past b,past c);
past astbreak(void);
past astcontinue(void);
past astreturn(past a);
void init(void);
#endif