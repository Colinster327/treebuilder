%start start_var
%token TKID TKINT TKNODE TKFOR TKSTR TKPRINT TKIN TKWEIGHT TKNAME TKCHILDOF
%left '+'

%{
  /*
  * tree_builder.y
  * Authors: Colin Murphy, Brock Klamfoth
  * Date: 2024-04-17
  * Description: This file contains the parser for a tree builder language.
  */

  #include "parse_tree.h"

  using namespace std;
%}

%union {
  char* s_val;
  expression *e_ptr;
  statement *s_ptr;
  compound_statement *c_ptr;
  vector<expression *> *e_list_ptr;
}

%{
  extern int yylex();
  extern void yyerror(const char *String);  
%}

%type <s_val> TKID TKINT TKSTR
%type <e_ptr> expression
%type <s_ptr> statement print_statement node_statement for_statement
%type <c_ptr> prog start_var
%type <e_list_ptr> expression_list

%%

start_var : prog
  { 
    map<string,shared_ptr<TreeNode>> tree_tab;
    map<string, variant<int, string>> var_tab;
    $$= $1;
    $1->evaluate_statement(tree_tab, var_tab);
  }
  ;

prog:
  statement  prog { $$ = new compound_statement($1,$2); }
  | { $$ = NULL; }
  ;

statement: 
  print_statement { $$ = $1; }
  | node_statement { $$ = $1; }
  | for_statement { $$ = $1; }
  ;

expression:
  TKINT { $$ = new constant(atoi($1)); }
  | TKSTR { $$ = new constant($1); }
  | TKID { $$ = new variable($1); }
  | expression '+' expression { $$ = new plus_expr($1, $3); }
  | expression '-' expression { $$ = new minus_expr($1, $3); }
  | expression '*' expression { $$ = new times_expr($1, $3); }
  | expression '/' expression { $$ = new divide_expr($1, $3); }
  | expression '%' expression { $$ = new mod_expr($1, $3); }
  ;

print_statement: 
  TKPRINT '(' expression ')' ';' { $$ = new print_statement($3); }
  ;

node_statement: 
  TKNODE '{' TKNAME '=' expression ';' TKWEIGHT '=' expression ';' TKCHILDOF '=' expression ';' '}' ';'
    { $$ = new node_statement($5, $9, $13); }
  | TKNODE '{' TKNAME '=' expression ';' TKWEIGHT '=' expression ';' '}' ';'
    { $$ = new node_statement($5, $9); }
  ;

for_statement:
  TKFOR TKID TKIN '[' expression ':' expression ']' '{' prog '}' ';'
    { $$ = new for_statement($2, $5, $7, $10); }
  | TKFOR TKID TKIN '[' expression_list ']' '{' prog '}' ';'
    { $$ = new for_statement($2, $5, $8); }
  ;

expression_list:
  expression { $$ = new vector<expression *>(); $$->push_back($1); }
  | expression_list ',' expression { $$ = $1; $$->push_back($3); }
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
