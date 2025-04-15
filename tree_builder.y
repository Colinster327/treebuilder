%start start_var
%token TKID TKINT TKNODE TKFOR TKSTR TKPRINT TKIN TKWEIGHT TKNAME TKCHILDOF
%left '+'

%{
  #include <map>
  #include <string>
  #include "parse_tree.h"

  using namespace std;
%}

%union {
  char* s_val;
  integer_expression *int_ptr;
  string_expression *s_ptr;
  string_list vector<string> s_list;
  statement *s_ptr;
  compound_statement *c_ptr;
}

%{
  extern int yylex();
  extern void yyerror(char *String);  

  #include <iostream>
  using namespace std;
%}

%type <s_val> TKID TKINT TKSTR
%type <int_ptr> integer_expression
%type <s_ptr> string_expression
%type <s_list> string_list
%type <s_ptr> statement print_statement for_statement node_statement
%type <c_ptr> prog start_var

%%

start_var : prog
  { 
    map<string,int> my_sym_tab;
    $$= $1;
    $1->evaluate_statement(my_sym_tab);
  }
  ;

prog:
  | statement  prog { $$ = new compound_statement($1,$2); }
  | { $$ = NULL; }
  ;

statement: 
  | for_statement { $$ = $1; }
  | print_statement { $$ = $1; }
  | node_statement { $$ = $1; }
  ;

integer_expression:
  | TKINT { $$ = new int_constant($1); }
  | integer_expression '+' integer_expression
    { $$ = new int_plus_expr($1, $3); }
  ;

string_expression:
  | TKSTR { $$ = new string_constant($1); }
  | string_expression '+' string_expression
    { $$ = new string_plus_expr($1, $3); }
  | string_expression '+' integer_expression
    { $$ = new string_int_plus_expr($1, $3); }
  | integer_expression '+' string_expression 
    { $$ = new string_int_plus_expr($1, $3); }
  ;

print_statement: 
  | TKPRINT '(' string_expression ')' ';' { $$ = new print_statement($3); }
  ;

node_statement: 
  | TKNODE '{' TKNAME '=' string_expression ';' TKWEIGHT '=' integer_expression ';' TKCHILDOF '=' string_expression '}' ';'
    { $$ = new node_statement($5, $9, $13); }
  | TKNODE '{' TKNAME '=' string_expression ';' TKWEIGHT '=' integer_expression ';' '}' ';'
    { $$ = new node_statement($5, $9); }
  ;

for_statement:
  | TKFOR TKID TKIN '[' integer_expression ':' integer_expression ']' '{' prog '}' ';'
    { $$ = new for_statement($2, $5, $7, $10); }
  | TKFOR TKID TKIN '[' string_list ']' '{' prog '}' ';'
    { $$ = new for_statement($2, $5, $8); }
  ;

string_list: 
  | string_expression { $$ = vector<string>(); $$.push_back($1); }
  | string_expression ',' string_list { $$ = $3; $$.insert($$.begin(), $1); }
  ;

%%

#include "lex.yy.c"

void yyerror(char *error_string)
{
  cout << "Error : " << error_string << "  on line " << line_nmb() << endl;
  exit(-1);
}


main() {
  yyparse();
}
