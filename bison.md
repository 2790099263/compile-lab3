# 电子科技大学信息与软件工程学院编译技术实验三

工程目前尚未完工.....

## bison 移进规约特点

bison的两种语法分析方法，一种是LALR(1)也就是从左向右看一个符号;另一种是GLR也就是通用的从左向右读取，我们使用LALR(1).

虽然LALR分析强大，但它对于语法规则有比较多的限制。它不能处理有歧义的语法，比如相同的输入可以匹配多棵语法分析树的二义性文法的情况（但是bison有一个很奇妙的技巧来解决常见的二义性文法）。它也不能处理需要向前看多个记号才能确定匹配规则的语法，下面有个实例：

```c
phrase: cart_animal AND CART
	  | work_animal AND PLOW

cart_animal: HORSE | GOAT

work_animal: HORSE | OX
```

这个语法并没有歧义，但是我们在匹配的时候需要向前提前看两个符号，这在bison是不可以的

同时，bison知道哪些是可以的，哪些是不可以的，遇到不能处理的时候会报错。

## bison的具体规则

bison程序包括四个成分

```c
/* 第一部分为定义部分，此部分主要包括选项、文字块、注释、声明符号、语义值数据类型的集合、指定开始符号及其它声明等等。
   文字块存在与%{和%}之间，它们将被原样拷贝到生成文件中。*/
%start calclist /* 指定起始符号（start symbol）有时也称为目标符号（goal symbol） */
%token NUMBER /* 声明tokens记号，以便于告诉bison在语法分析程序中记号的名称。通常这些记号总是使用大写字母，虽然bison本身并没有这个要求。 */
    
%{
  /* 文字块，该部分的内容将直接复制到生成的代码文件的开头，以便它们在使用yyparse定义之前使用。 */
  #define _GNU_SOURCE
  #include <stdio.h>
  #include "ptypes.h"
%}

%%
/* 第二部分，主要是语法规则 */
calclist: /* 空规则 -- 起始符号（start symbol）有时也称为目标符号（goal symbol） */
/* 如果没有指定语义动作，bison将使用默认的动作： { $$ = $1; }*/
  | calclist exp EOL { printf("- %d\n", $2); } // EOL 代表一个表达式的结束。像flex一样，大括号中的表示规则的动作
  ;

exp: factor // default $$ = $1
  | exp ADD factor { $$ = $1 + $3; }
  | exp SUB factor { $$ = $1 - $3; }
  ; // represent the termination of this rule.

factor: term // default $$ = $1
  | factor MUL term { $$ = $1 * $3; }
  | factor DIV term { $$ = $1 / $3; }
  ;

term: NUMBER // default $$ = $1
  | ABS term { $$ = $2 >= 0? $2 : - $2; }
  ;
%%
/*第三部分，此部分的内容将直接逐字复制到生成的代码文件末尾。该部分主要用于对之前一些声明了的函数进行实现。 */
```

### %start声明

​	%start声明起始规则，也就是语法分析器首先开始分析的规则，默认是第一个规则。大多数情况下，最清楚的表达语法的方式是自上而下，起始规则放在第一个，这样%start就不需要了。起始符号必须具备一个空规则，旨在让开始输入的记号能够从起始符号开始匹配。

### union声明

%union声明标识出了符号值可能拥有的所有C类型，格式如下：

```c
%union{
... 域声明 ...
}
```

域声明将被原封不动地拷贝到输出文件中类型为YYSTYPE的C的union声明里。

关于YYSTYPE，bison里的YYSTYPE默认是int类型的，可以用%union将YYSTYPE定义为联合体。bison生成代码时，将会在name.tab.c文件中定义YYSTYPE的yylval变量，如下所示：

```c
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;
```

并且在name.tab.h后文件中将yylval声明为extern的，如下：

```c
union YYSTYPE
{
#line 10 "/home/cmp/work_dir/source_code/yacc_bison_practice/ch3/3.02/3.02_create_AST_with_bison.y" /* yacc.c:1909  */

  struct ASTNode *a;
  double d;

#line 64 "3.02_create_AST_with_bison.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;
```

进一步还可以将YYSTYPE定义为我自己定义的一个struct的指针，然后作为一个全局变量，让lex在扫描的时候，可以直接把扫描的东西放到yylval的数据结构中去。

### %type声明

使用%type声明非终结符的类型。格式如下：

`%type <type> name, name, ...`

每个type的名字必须是用%union定义过。而每个name就是非终结符的名字。对于记号而言，你需要使用%token, %left, %right, %nonassoc，这些声明不仅可以用来指明记号的类型，还可以定义优先级和结合性

## 文字记号

bison把单引号引起的字符也作为一个记号看待。例如：

```c
expr: '(' expr ')';
```

左圆括号和右圆括号都是文字记号（literal token）。文字记号的编号也就是它们在本地字符集（通常是ASCII）对应的数值，与C语言用的字符的数值一致。

词法分析器通常从输入中对应的单个字符来产生这些记号，但是如同其他记号一样，输入字符和产生的记号之间的对应关系是完全有词法分析器决定的。一种常见的技术是让词法分析器把所有不能识别的字符作为文字记号看待。例如，在flex词法分析器中：

```c
return yytext[0];
```

这包括了语言中的所有单字符操作符，而让Bison来捕获哪些输入中存在不能识别的字符，然后报告错误。

bison也允许你为字符串定义一个别名来方便识别记号，例如：

```c
%token NE "!="
%%
...
exp: exp "!=" exp;
```

它定义了记号NE，使得你可以在语法分析器中任意地使用NE或者!=，词法分析器读到这个单词时，必须依然返回NE的内部记号编号，而不是一个字符串。

## bison中所有特殊的符号汇总

由于bison处理符号记号而不是字面文本，它的输入字符集比词法分析器要来得简单。下面是Bison所使用的特殊字符列表：

%，具有两个百分号的行用来分割bison语法的各个部分

$ 在语义动作中，美元符号引入一个值引用 例如  $$3代表规则右部第三个符号的值。

@，在语义动作中，@符号引入一个位置引用，比如@2代表规则右部第二个符号的位置。

'，文字记号用单引号

"，bison允许你把双引号引起的字符串定义为记号的别名

<>，在语义动作中的值引用里，你可以通过扩在尖括号里的类型名来覆盖值的默认类型，例如$3

{}，语义动作的C代码使用花括号括起

;，规则部分的每个规则都必须使用分号结尾，后面又紧跟以竖线开始的另一个规则的规则可以出该。

/，当两个连续的规则具有相同的左部时，第二个规则可以把左部的符号和冒号替换为竖线

:，在每条规则中，冒号出现在规则左部的非终结符之后

## 递归的语法规则

为了分析不定长的项目列表，你需要使用递归规则，也就是用自身来定义自己。例如，下面这个例子分析一个可能为空的数字列表：

```c
numberlist:  /* 空规则 */
          | numberlist NUMBER
          ;
```

递归规则的实现完全依赖于具体需要分析的语法。下面这个例子分析一个通过逗号分隔的不为空的表达式列表，其中的expr在语法的其他地方已经被定义：

```c
exprlist: expr
        : exprlist ',' expr
        ;
```

也可能存在交互的递归规则，它们彼此引用对方：

```c
exp: term
   | term '+' term
   ;

term: '(' exp ')'
    | VARIABLE
    ;
```

**任何递归规则或者交互递归规则组里的每个规则都必须至少有一条非递归的分支（不指向自身）**；否则，将没有任何途径来终止它所比匹配的字符串，这是一个错误。

## 左递归和右递归

当你编写一个递归规则时，你可以把递归的引用放在规则右部的左端或者右端，例如：

```c
exprlist: exprlist ',' expr; /* 左递归 */
exprlist: expr ',' exprlist; /* 右递归 */	
```

大多数情况下，你可以选择任意一种方式来编写语法。bison处理左递归要比处理右递归更有效率。这是因为它的内部堆栈需要追踪到目前位置所有还处在分析中规则的全部符号。

如果使用右递归，而且有个表达式包含了10个子表达式，当读取第10个表达式的时候，堆栈中会有20个元素：10个表达式各自有expr和逗号。当表达式结束时，所有嵌套的exprlist都需要按照从右向左的顺序来规约。另一个方面，如果你使用左递归的版本，exprlist将在每个expr之后进行规约，这样内部堆栈中列表将永远不会超过三个元素。

具有10个元素的表达式列表不会对语法分析器造成什么问题。但是我们的语法经常需要分析拥有成千上万的元素的列表，尤其是当程序被定义为语句的列表时：



## 参考文章

https://blog.csdn.net/weixin_46222091/article/details/105990745