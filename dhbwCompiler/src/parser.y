/* 
 * parser.y - Parser utility for the DHBW compiler
 * bison -d -b parser parser.y -t
 * gcc parser.tab.c lex.yy.c -lfl -lm -o bin32
 */

%{
	#include "include/utlist.h" 
	#include "symboltable.h"
	#include "ir_code_generation.h"
	#include <stdio.h>
	
	void yyerror (char const *);
%}
 
%union{
	int num;
	char *id;
	struct symbol *Sym;
}

%debug
%locations
%start program

/*
 * One shift/reduce conflict is expected for the "dangling-else" problem. This
 * conflict however is solved by the default behavior of bison for shift/reduce 
 * conflicts (shift action). The default behavior of bison corresponds to what
 * we want bison to do: SHIFT if the lookahead is 'ELSE' in order to bind the 'ELSE' to
 * the last open if-clause. 
 */
%expect 1

%token DO WHILE
%token IF ELSE
%token <id>INT <id>VOID
%token RETURN
%token COLON COMMA SEMICOLON
%token BRACE_OPEN BRACE_CLOSE

%token <id>ID
%token <num>NUM

%right ASSIGN 
%left  LOGICAL_OR
%left  LOGICAL_AND
%left  EQ NE     
%left  LS LSEQ GTEQ GT 
%left  SHIFT_LEFT SHIFT_RIGHT
%left  PLUS MINUS     
%left  MUL
%right LOGICAL_NOT UNARY_MINUS UNARY_PLUS
%left  BRACKET_OPEN BRACKET_CLOSE PARA_OPEN PARA_CLOSE


%type <Sym> function_call_parameters
%type <Sym> function_definition
%type <Sym> function_parameter_list
%type <Sym> function_declaration
%type <Sym> function_call
%type <num> type
%type <Sym> function_parameter
%type <Sym> identifier_declaration
%type <Sym> expression
%type <Sym> primary

%%

program
     : program_element_list			/*Nothing to be done here*/
     ;

program_element_list
     : program_element_list program_element 
     | program_element 				
     ;

program_element
     : variable_declaration SEMICOLON		/*Nothing to be done here*/
     | function_declaration SEMICOLON		/*Nothing to be done here*/
     | function_definition					/*Nothing to be done here*/
     | SEMICOLON							/*Nothing to be done here*/
     ;
     
type
     : INT  {$$=1}
     | VOID {$$=0}
     ;

variable_declaration
     : variable_declaration COMMA identifier_declaration	/*Nothing to be done here*/
     | type identifier_declaration { if($1==0) { printf("ERROR -- your variable needs a type\n"); } } 
     ;
	
identifier_declaration
     : identifier_declaration BRACKET_OPEN NUM BRACKET_CLOSE {$1->is.var.isArray = 1;$1->is.var.arr_size = $3;}
     | ID {$$ = pushVar($1);}
     ;

function_definition
     : type ID PARA_OPEN PARA_CLOSE BRACE_OPEN {pushFunc($1,$2,NULL);} stmt_list BRACE_CLOSE {resetScope();}
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE BRACE_OPEN {resetScope(); $<Sym>$=pushFunc($1,$2,$4->is.func.local_table);} stmt_list BRACE_CLOSE {resetScope();} 
     ;

function_declaration
     : type ID PARA_OPEN PARA_CLOSE	{$$=pushFunc($1,$2,NULL);$$->is.func.isProto=1;resetScope();}					
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE {resetScope(); $$=pushFunc($1,$2,$4->is.func.local_table); $$->is.func.isProto=1;resetScope();}		
     ;

function_parameter_list
     : {$<Sym>$=pushFunc(0,"-pseudo-",NULL);}function_parameter
     | function_parameter_list COMMA function_parameter	{$$=$1;}	
     ;
	
function_parameter
     : type identifier_declaration {$$=$2;}
     ;

stmt_list
     : // empty: epsilon
     | stmt_list stmt					/*Nothing to be done here*/
     ;

stmt
     : stmt_block
     | variable_declaration SEMICOLON
     | expression SEMICOLON
     | stmt_conditional
     | stmt_loop
     | RETURN expression SEMICOLON
     | RETURN SEMICOLON
     | SEMICOLON
     ;

stmt_block
     : BRACE_OPEN stmt_list BRACE_CLOSE
     ;
//the shift/reduce error which occurs here is expected. nothing to see here move along
//changed the grammar slightly to handle the goto statements easier.

stmt_conditional
     : IF PARA_OPEN expression PARA_CLOSE stmt_conditional_r 
     //| IF PARA_OPEN expression 
     ;
     
stmt_conditional_r
     : stmt 
     | stmt ELSE stmt
     ;
     
stmt_loop
     : WHILE PARA_OPEN expression PARA_CLOSE stmt
     | DO stmt WHILE PARA_OPEN expression PARA_CLOSE SEMICOLON
     ;
     
/*
 * The non-terminal 'expression' is one of the core statements containing all arithmetic, logical, comparison and
 * assignment operators.expression
 */									
expression								// 0 = "false", nonzero = "true"
     : expression ASSIGN expression			
     | expression LOGICAL_OR expression	
     | expression LOGICAL_AND expression
     | LOGICAL_NOT expression		
     | expression EQ expression		
     | expression NE expression		
     | expression LS expression 	
     | expression LSEQ expression 	
     | expression GTEQ expression 
     | expression GT expression	
     | expression PLUS expression				
     | expression MINUS expression				
     | expression MUL expression				
     | MINUS expression %prec UNARY_MINUS		
     | ID BRACKET_OPEN primary BRACKET_CLOSE	
     | PARA_OPEN expression PARA_CLOSE			
     | function_call							
     | primary									
     ;

primary
     : NUM 										
     | ID  										
     ;

function_call
      : ID PARA_OPEN PARA_CLOSE						
      | ID PARA_OPEN function_call_parameters PARA_CLOSE	
      ;

function_call_parameters
     : function_call_parameters COMMA expression		
     | expression						
     ;

%%

void yyerror (const char *msg)
{
	printf("ERROR: %s\n", msg);
	//return 0;
}