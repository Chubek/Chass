# Grammar for ChassL

Chass is a program used for encrypting a hashtable database. It sports a very small, domain-specific language interface.

Name of this language is ChassL.

The following document explains the grammar for ChassL.


## Basic Lexical Grammar

The lexical grammar for Chass can be described as follows. This grammar is expessed in EBNF notation.

Supported literals are: 

- Numeric (float, integer)
- String

Identifiers may have alphanumeric characters, '-' and '\_' in them.

```ebnf
# Lexical Grammar for ChassL

STRING  ::= /['"].*['"]/ ;
NUMBER 	::= /([0-9].)?[0-9]+/ ;
IDENT 	::= /[-_a-zA-Z0-9]/ ;

```

## Expressions

The language supports basic expessions:


```ebnf
# Syntactic Gramamr for ChassL's Expressions

factor		::= NUMBER | IDENT | STRING | '(' exp ')' ;

prefix-op	::= '+' | '-' | '~' | '!' | "++" | "--" ;
postfix-op	::= "++" | "--" | '(' { exp ',' } ')' | '[' exp ']' ;

prefix-exp	::= prefix-op factor ;
postfix-exp 	::= factor { postfix-op };

unary-exp 	::= prefix-exp | postfix-exp ;

infix-op	::= '+' | '-'  | '*' | '/' 
		 | '%'  | "**" | ">>"| "<<"
		 | "==" | "!=" | '<' | '>'
	         | ">=" | "=<" | '&' | '^' 
		 | '|'  | "&&" | "||"| '='
		 | "+=" | "-=" | "*="| "/="
		 | "%=" | ">>="|"<<="| "**="
		 ;
binary-exp	::= factor infix-op factor ;

ternay-exp	::= exp '?' exp ':' exp ;

exp 		::= factor | unary-exp | binary-exp | ternary-exp ;

```

## Statements

Basic control-flow operations and statements are supported in ChassL.


```ebnf
# Syntactic Grammar for ChassL's Statements

exp-stmt	::= exp ';'
for-stmt	::= "for" IDENT "in" exp "do" stmt "end" ;
do-while-stmt	::= "do" stmt "while" exp "end" ;
while-stmt	::= "while" exp "do" stmt "end" ;
until-stm	::= "until" exp "do" stmt "end" ;
if-stmt		::= "if" exp "do" stmt { "elif" exp "do" stmt "end" } [ "else" "do" stmt "end" ] ;
unless-stmt	::= "unless" exp "do" stmt { "unlessif" exp "do" stmt "end" } [ "else" "do" stmt "end" ] ;
ctrlflow-stmt	::= "return" exp | "continue" | "break" ;
stmt		::= exp-stmt
		 | for-stmt
		 | do-while-stmt
		 | until-stmt
		 | if-stmt
		 | unless-stmt
		 | ctrlflow-stmt
		 | { stmt }
		 ;
```
