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
  string_expression *str_ptr;
  statement *s_ptr;
  compound_statement *c_ptr;
}

%{
  extern int yylex();
  extern void yyerror(const char *String);  

  #include <iostream>
  using namespace std;
%}

%type <s_val> TKID TKINT TKSTR
%type <int_ptr> integer_expression
%type <str_ptr> string_expression
%type <s_ptr> statement print_statement node_statement
%type <c_ptr> prog start_var

%%

start_var : prog
  { 
    map<string,TreeNode *> my_sym_tab;
    $$= $1;
    $1->evaluate_statement(my_sym_tab);
  }
  ;

prog:
  | statement  prog { $$ = new compound_statement($1,$2); }
  | { $$ = NULL; }
  ;

statement: 
  | print_statement { $$ = $1; }
  | node_statement { $$ = $1; }
  ;

integer_expression:
  | TKINT { $$ = new int_constant(atoi($1)); }
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
  | TKNODE '{' TKNAME '=' string_expression ';' TKWEIGHT '=' integer_expression ';' TKCHILDOF '=' string_expression ';' '}' ';'
    { $$ = new node_statement($5, $9, $13); }
  | TKNODE '{' TKNAME '=' string_expression ';' TKWEIGHT '=' integer_expression ';' '}' ';'
    { $$ = new node_statement($5, $9); }
  ;

%%

#include "lex.yy.c"

void yyerror(const char* s) {
  cout << "Error : " << s << "  on line " << line_nmb() << endl;
  exit(-1);
}


int main() {
  yyparse();
}
