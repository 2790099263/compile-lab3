#include "ast.h"

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
extern int yylex();
extern int yylval;
extern char *yytext;
char s_now[10];
int tok;

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
	showAst(node->left, nest + 1);
	showAst(node->right, nest + 1);
	showAst(node->next, nest);
}
past newAstNode()
{
	past node = malloc(sizeof(ast));
	if(node == NULL)
	{
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));
	return node;
}

past newI(int value)
{
	past var = newAstNode();
	var->nodeType = INTEGER_LITERAL;
	var->ivalue = value;
	var->svalue=malloc(10);
	sprintf(var->svalue,"%d",value);
	return var;
}
past newF(float value)
{
	past var = newAstNode();
	var->nodeType = FLOATING_LITERAL;
	var->fvalue = value;
	var->svalue=malloc(10);
	sprintf(var->svalue,"%f",value);
	return var;
}
past newS(char* value){
	past var = newAstNode();
	var->nodeType = DECL_REF_EXPR;
	var->svalue = malloc(strlen(value)+1);
	strcpy(var->svalue,value);
	return var;
}
past astLOrExp(past land,past lor){
	past top = newAstNode();
	top->svalue = malloc(3);
	top->svalue = "||";
	top->nodeType = BINARY_OPERATOR;
	if(strcmp(lor->svalue,"||")){
		top->left = land;
		top->right = lor;
		return top;
	}
	else{
		past p = lor;
		while(p->left!=NULL&&!strcmp(p->left->svalue,"||")){
			p=p->left;
		}
		top->right = p->left;
		p->left = top;
		top->left = land;
		return lor;
	}
}
past astLAndExp(past eq,past land){
	past top = newAstNode();
	top->svalue = malloc(3);
	top->svalue = "&&";
	top->nodeType = BINARY_OPERATOR;
	if(strcmp(land->svalue,"&&")){
		top->left = eq;
		top->right = land;
		return top;
	}
	else{
		past p = land;
		while(p->left!=NULL&&!strcmp(p->left->svalue,"&&")){
			p=p->left;
		}
		top->right = p->left;
		p->left = top;
		top->left = eq;
		return land;
	}
}
past astEqExp(char* s,past rel,past eq){
	past top = newAstNode();
	top->svalue = malloc(3);
	strcpy(top->svalue,s);
	top->nodeType = BINARY_OPERATOR;
	if(strcmp(eq->svalue,"==")||strcmp(eq->svalue,"!=")){
		top->left = rel;
		top->right = eq;
		return top;
	}
	else{
		past p = eq;
		while(p->left!=NULL&&(!strcmp(p->left->svalue,"==")||!strcmp(p->left->svalue,"!="))){
			p=p->left;
		}
		top->right = p->left;
		p->left = top;
		top->left = rel;
		return eq;
	}
}
past astRelExp(char* s,past add,past rel){
	past top = newAstNode();
	top->svalue = malloc(3);
	strcpy(top->svalue,s);
	top->nodeType = BINARY_OPERATOR;
	if(strcmp(rel->svalue,"<")||strcmp(rel->svalue,"<=")||strcmp(rel->svalue,">")||strcmp(rel->svalue,">=")){
		top->left = add;
		top->right = rel;
		return top;
	}
	else{
		past p = rel;
		while(p->left!=NULL&&(!strcmp(p->left->svalue,"<")||!strcmp(p->left->svalue,"<=")||!strcmp(p->left->svalue,">")||!strcmp(p->left->svalue,">="))){
			p=p->left;
		}
		top->right = p->left;
		p->left = top;
		top->left = add;
		return rel;
	}
}
past astAddExp(char* s,past mul,past add){
	past top = newAstNode();
	top->svalue = malloc(3);
	strcpy(top->svalue,s);
	top->nodeType = BINARY_OPERATOR;
	if(strcmp(add->svalue,"+")||strcmp(add->svalue,"-")){
		top->left = mul;
		top->right = add;
		return top;
	}
	else{
		past p = add;
		while(p->left!=NULL&&p->left->nodeType!=UNARY_OPERATOR&&(!strcmp(p->left->svalue,"+")||!strcmp(p->left->svalue,"-"))){
			p=p->left;
		}
		top->right = p->left;
		p->left = top;
		top->left = mul;
		return add;
	}
}
past astMulExp(char* s,past unary,past mul){
	past top = newAstNode();
	top->svalue = malloc(3);
	strcpy(top->svalue,s);
	top->nodeType = BINARY_OPERATOR;
	if(strcmp(mul->svalue,"*")||strcmp(mul->svalue,"/")||strcmp(mul->svalue,"%")){
		top->left = unary;
		top->right = mul;
		return top;
	}
	else{
		past p = mul;
		while(p->left!=NULL&&(!strcmp(p->left->svalue,"*")||!strcmp(p->left->svalue,"/")||!strcmp(p->left->svalue,"%"))){
			p=p->left;
		}
		top->right = p->left;
		p->left = top;
		top->left = unary;
		return mul;
	}
}
past astCallParams(past add,past call){
	add->next = call;
	return add;
}
past funcc(char* s,past call){
	past funcc = newAstNode();
	funcc->nodeType = CALL_EXPR;
	funcc->svalue=malloc(2);
	strcpy(funcc->svalue," ");
	funcc->left = newS(s);
	funcc->right = call;
	return funcc;
}
past astUnaryExp(char* s,past unary){
	past tmp = newAstNode();
	tmp->nodeType = UNARY_OPERATOR;
	tmp->svalue = malloc(3);
	strcpy(tmp->svalue,s);
	tmp->left = unary;
	return tmp;
}
past astArray(past a,past b){
	past tmp = newAstNode();
	tmp->nodeType = ARRAY_SUBSCRIPT_EXPR;
	tmp->svalue=malloc(2);
	strcpy(tmp->svalue," ");
	if(b==NULL){
		tmp->right = a;
		return tmp;
	}
	else{
		past p = b;
		while(p->left!=NULL){
			p=p->left;
		}
		p->left = tmp;
		tmp->right = a;
		return b;
	}
}
past astLVal(char* s,past array){
	past id = newS(s);
	past p = array;
	while(p->left!=NULL){
		p=p->left;
	}
	p->left = id;
	return array;
}
past astCompUnit(past a,past b){
	a->next = b;
	return a;
}
past astConstDecl(char* s,past def){
	past tmp = newAstNode();
	tmp->nodeType = DECL_STMT;
	tmp->svalue=malloc(2);
	strcpy(tmp->svalue," ");
	past p = def;
	do{
		p->svalue = malloc(strlen(s)+6);
		strcat(p->svalue," const ");
		strcat(p->svalue,s);
		p=p->next;
	}while(p!=NULL);
	tmp->left = def;
	return tmp;
}
past astConstDefs(past a,past b){
	a->next = b;
	return a;
}
past astConstDef(char* s,past init){
	past tmp = newAstNode();
	tmp->nodeType = VAR_DECL;
	tmp->svalue = malloc(strlen(s)+15);
	strcpy(tmp->svalue,s);
	tmp->left = init;
	return tmp;
}
past astConstInitVal(past a,past b){
	past tmp = newAstNode();
	tmp->nodeType = INIT_LIST_EXPR;
	tmp->svalue=malloc(3);
	strcpy(tmp->svalue," ");
	if(a==NULL&&b==NULL){
		return tmp;
	}
	else if(b==NULL){
		tmp->left = a;
		return tmp;
	}
	else{
		a->next = b;
		tmp->left = a;
		return tmp;
	}
}
past astConstInitVals(past a,past b){
	a->next = b;
	return a;
}
past astVarDecl(char* s,past a,past b){
	past tmp = newAstNode();
	tmp->nodeType = DECL_STMT;
	tmp->svalue=malloc(2);
	strcpy(tmp->svalue," ");
	if(b==NULL){
		strcat(a->svalue," ");
		strcat(a->svalue,s);
		tmp->left = a;
		return tmp;
	}
	else{
		a->next = b;
		past p = a;
		while(p!=NULL){
			strcat(p->svalue," ");
			strcat(p->svalue,s);
			p=p->next;
		}
		tmp->left = a;
		return tmp;
	}
}
past astVarDecls(past a,past b){
	a->next = b;
	return a;
}
past astVarDef(char* s,past a){
	past tmp = newAstNode();
	tmp->nodeType = VAR_DECL;
	tmp->svalue = malloc(strlen(s)+10);
	strcpy(tmp->svalue,s);
	tmp->left = a;
	return tmp;
}
past astInitVal(past a,past b){
	past tmp = newAstNode();
	tmp->nodeType = INIT_LIST_EXPR;
	tmp->svalue=malloc(2);
	strcpy(tmp->svalue," ");
	if(a==NULL&&b==NULL){
		return tmp;
	}
	else if(b==NULL){
		tmp->left = a;
		return tmp;
	}
	else{
		a->next = b;
		tmp->left = a;
		return tmp;
	}
}
past astInitVals(past a,past b){
	a->next = b;
	return a;
}
past astFuncDef(char* s,char* s2,past a,past b){
	past tmp = newAstNode();
	tmp->nodeType=FUNCTION_DECL;
	tmp->svalue=malloc(strlen(s)+strlen(s2)+2);
	strcpy(tmp->svalue,s);
	strcat(tmp->svalue," ");
	strcat(tmp->svalue,s2);
	tmp->left=a;
	tmp->right=b;
	return tmp;
}
past astFuncParams(past a,past b){
	a->next = b;
	return a;
}
past astFuncParam(char* s,char* s2){
	past tmp = newAstNode();
	tmp->nodeType=PARM_DECL;
	tmp->svalue=malloc(strlen(s)+strlen(s2)+2);
	strcpy(tmp->svalue,s);
	strcat(tmp->svalue," ");
	strcat(tmp->svalue,s2);
	return tmp;
}
past astBlock(past a){
	past tmp = newAstNode();
	tmp->nodeType=COMPOUND_STMT;
	tmp->svalue=malloc(2);
	strcpy(tmp->svalue," ");
	tmp->left=a;
	return tmp;
}
past astBlockItems(past a,past b){
	a->next = b;
	return a;
}
past astStmt1(past a,past b){
	past tmp=newAstNode();
	tmp->nodeType=BINARY_OPERATOR;
	tmp->svalue=malloc(3);
	strcpy(tmp->svalue,"=");
	tmp->left=a;
	tmp->right=b;
	return tmp;
}
past astStmt2(void){
	past tmp=newAstNode();
	tmp->nodeType=NULL_STMT;
	tmp->svalue=malloc(2);
	strcpy(tmp->svalue," ");
	return tmp;
}
past astwhile(past a,past b){
	past tmp=newAstNode();
	tmp->nodeType=WHILE_STMT;
	tmp->svalue=malloc(2);
	strcpy(tmp->svalue," ");
	tmp->left=a;
	tmp->right=b;
	return tmp;
}
past astif(past a,past b,past c){
	past tmp=newAstNode();
	tmp->nodeType=IF_STMT;
	tmp->svalue=malloc(2);
	strcpy(tmp->svalue," ");
	tmp->left=a;
	tmp->right=b;
	if(c!=NULL){
		tmp->svalue=malloc(10);
		strcpy(tmp->svalue,"has_else");
		tmp->right->next=c;
	}
	return tmp;
}
past astbreak(void){
	past tmp=newAstNode();
	tmp->nodeType=BREAK_STMT;
	tmp->svalue=malloc(2);
	strcpy(tmp->svalue," ");
	return tmp;
}
past astcontinue(void){
	past tmp=newAstNode();
	tmp->nodeType=CONTINUE_STMT;
	tmp->svalue=malloc(2);
	strcpy(tmp->svalue," ");
	return tmp;
}
past astreturn(past a){
	past tmp=newAstNode();
	tmp->nodeType=RETURN_STMT;
	tmp->svalue=malloc(2);
	strcpy(tmp->svalue," ");
	tmp->left=a;
	return tmp;
}
