%{
    /*
    * lexer.l
    * Authors: Colin Murphy, Brock Klamfoth
    * Date: 2024-03-28
    * Description: This file contains the lexical analyzer for a tree builder language.
    */

    #include <iostream>    

    using namespace std;
%}

LETTER [a-zA-Z]
DIGIT [0-9]
STR \"[^\"]*\"
INT -?{DIGIT}+
ID ({LETTER}|{DIGIT})({LETTER}|{DIGIT}|_)*

%%

[ \t\n\r\f\v]   { /* ignore whitespace */ }
"isachildof"    { return TKCHILDOF; }
"buildnode"     { return TKNODE; }
"weight"        { return TKWEIGHT; }
"print"         { return TKPRINT; }
"name"          { return TKNAME; }
"for"           { return TKFOR; }
"in"            { return TKIN; }
"{"             { return '{'; }
"}"             { return '}'; }
"("             { return '('; }
")"             { return ')'; }
"["             { return '['; }
"]"             { return ']'; }
"+"             { return '+'; }
"="             { return '='; }
";"             { return ';'; }
":"             { return ':'; }
","             { return ','; }
{STR}           {
                  yylval.s_val = strdup(yytext);
                  return TKSTR;
                }
{INT}           {
                  yylval.s_val = strdup(yytext);
                  return TKINT;
                }

{ID}            {
                  yylval.s_val = strdup(yytext);
                  return TKID; 
                }
.               { /* ignore other characters */ }

%%
