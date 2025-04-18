%{
    /*
    * tree_builder.l
    * Authors: Colin Murphy, Brock Klamfoth
    * Date: 2024-03-28
    * Description: This file contains the lexical analyzer for a tree builder language.
    */

    int linecount=1;
    int line_nmb()
    {
      return linecount;
    }
%}

LETTER [a-zA-Z]
DIGIT [0-9]
STR \"[^\"]*\"
INT -?{DIGIT}+
ID ({LETTER}|{DIGIT})({LETTER}|{DIGIT}|_)*

%%

[ \t\n\r\f\v]   { linecount ++; }
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
                  yytext[strlen(yytext) - 1] = '\0';
                  yylval.s_val = strdup(yytext + 1);
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

int yywrap() {
    return 1;  // Indicate end of input
}
