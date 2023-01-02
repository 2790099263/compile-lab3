#include<stdio.h>
#include <stdlib.h>
extern FILE* yyin;
extern int yyparse(void);
extern void init(void);
void yyerror(char *s)
{
	printf("%s\n", s);
}
int main(int argc,char* argv[])
{
	if(argc<2){
		printf("input file is needed.\n");
		exit(0);
	}
	setbuf(stdout,NULL);
	yyin = fopen(argv[1], "r");
	if(yyin==NULL){
		printf("can not open file.\n");
		exit(0);
	}
	yyparse();
	return 0;
}
