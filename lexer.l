%{
    /*
    * lexer.l
    * Authors: Colin Murphy, Brock Klamfoth
    * Date: 2024-03-28
    * Description: This file contains the lexical analyzer for a tree builder language.
    */

    #include <iostream>    

    using namespace std;

    #define TKID 257
    #define TKINT 258
    #define TKNODE 259
    #define TKFOR 260
    #define TKSTR 261
    #define TKPRINT 262
    #define TKIN 263
%}

LETTER [a-zA-Z]
DIGIT [0-9]
STR \"[^\"]*\"
INT {DIGIT}+
ID ({LETTER}|{DIGIT})({LETTER}|{DIGIT}|_)*

%%

[ \t\n\r\f\v]   { /* ignore whitespace */ }
"buildnode"     { return TKNODE; }
"print"         { return TKPRINT; }
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
{STR}           { return TKSTR; }
{INT}           { return TKINT; }
{ID}            { return TKID; }
.               { /* ignore other characters */ }

%%

int yywrap () {
    return 1;
}

int main(){
    int token;
    while (token = yylex()){
        switch (token) {
            case TKID: cout << "TKID: " << yytext << endl; break;
            case TKINT: cout << "TKINT: " << yytext << endl; break;
            case TKNODE: cout << "TKNODE" << endl; break;
            case TKFOR: cout << "TKFOR" << endl; break;
            case TKSTR: cout << "TKSTR: " << yytext << endl; break;
            case TKPRINT: cout << "TKPRINT" << endl; break;
            case TKIN: cout << "TKIN" << endl; break;
            default: cout << (char)token << endl;
        }
    }
    cout << endl;
}