# part of the makefile

CFLAGS = -O0 -g

all: lrparser.tab.c lex.yy.c ast.c main.c
	gcc -o lrparser $(CFLAGS) lrparser.tab.c lex.yy.c ast.c main.c


lrparser.tab.c : lrparser.y
	bison -d lrparser.y

lex.yy.c : lrlex.l
	flex lrlex.l
	


clean:
	$(RM) *.o *.exe lrparser.tab.c lrparser.tab.h lex.yy.c
