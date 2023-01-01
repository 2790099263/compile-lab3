#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "node_type.h"

enum yytokentype {
    num_INT = 258,
    num_FLOAT = 259,

    Y_ID = 260,

    Y_INT = 261,
    Y_VOID = 262,
    Y_CONST = 263,
    Y_IF = 264,
    Y_ELSE = 265,
    Y_WHILE = 266,
    Y_BREAK = 267,
    Y_CONTINUE = 268,
    Y_RETURN = 269,

    Y_ADD = 270,
    Y_SUB = 271,
    Y_MUL = 272,
    Y_DIV = 273,
    Y_MODULO = 274,
    Y_LESS = 275,
    Y_LESSEQ = 276,
    Y_GREAT = 277,
    Y_GREATEQ = 278,
    Y_NOTEQ = 279,
    Y_EQ = 280,
    Y_NOT = 281,
    Y_AND = 282,
    Y_OR = 283,
    Y_ASSIGN = 284,

    Y_LPAR = 285,
    Y_RPAR = 286,
    Y_LBRACKET = 287,
    Y_RBRACKET = 288,
    Y_LSQUARE = 289,
    Y_RSQUARE = 290,
    Y_COMMA = 291,
    Y_SEMICOLON = 292,

    Y_FLOAT = 293
};

void print(past cur) {
	enum yytokentype type = cur->nodeType;
	switch (type) {
		case COMPOUND_STMT: printf("COMPOUND_STMT"); break;
		case RETURN_STMT: printf("RETURN_STMT"); break;
		case DECL_REF_EXPR: printf("DECL_REF_EXPR %s", cur->svalue); break;
		case CALL_EXPR: printf("CALL_EXPR"); break;
		case INTEGER_LITERAL: printf("INTEGER_LITERAL %d", cur->ivalue); break;
		case FLOATING_LITERAL: printf("FLOATING_LITERAL %f", cur->fvalue); break;
		case UNARY_OPERATOR: printf("UNARY_OPERATOR %s", cur->svalue); break;
		case ARRAY_SUBSCRIPT_EXPR: printf("ARRAY_SUBSCRIPT_EXPR"); break;
		case BINARY_OPERATOR: printf("BINARY_OPERATOR %s", cur->svalue); break;
		case IF_STMT: printf("IF_STMT"); break;
		case WHILE_STMT: printf("WHILE_STMT"); break;
		case CONTINUE_STMT: printf("CONTINUE_STMT"); break;
		case BREAK_STMT: printf("BREAK_STMT"); break;
		case FUNCTION_DECL: printf("FUNCTION_DECL %s", cur->svalue); break;
		case VAR_DECL: printf("VAR_DECL %s", cur->svalue); break;
		case PARM_DECL: printf("PARM_DECL %s", cur->svalue); break;
		case INIT_LIST_EXPR: printf("INIT_LIST_EXPR"); break;
		case DECL_STMT: printf("DECL_STMT"); break;
		case TRANSLATION_UNIT: printf("TRANSLATION_UNIT"); break;
		case NULL_STMT: printf("NULL_STMT"); break;
		default: printf("Unknown node type!"); break;
	}
}
void showAst(past node, int nest) {
	if(node == NULL) return;
	int i = 0;
	for(i = 0; i < nest; i ++)
		printf("  ");
	//printf("%d\n", node->nodeType);
	print(node);
	puts("");
	showAst(node->if_cond, nest + 1);
	showAst(node->left, nest + 1);
	showAst(node->right, nest + 1);
	showAst(node->next, nest);
}
past newAstNode(node_type nodetype,past left,past right){
    past node = malloc(sizeof(ast));
    if(node == NULL){
        printf("Run out of Memory!\n");
        exit(0);
    }
    memset(node,0,sizeof(ast));
    node->nodeType  =   nodetype;
    node->left      =   left    ;
    node->right     =   right   ;
    return node;
}
void Free(past x){
    if(x==NULL)return ;
    Free(x->left);
    Free(x->right);
    Free(x->next);
    free(x);
    return ;
}