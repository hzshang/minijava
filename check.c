#include "stand.h"
#include "util.h"
#include "tokens.h"
#include "error.h"


YYSTYPE yylval;
extern int line_num;
int yylex();
string names[] = {"ID","CLASS","PUBLIC","STATIC","VOID","LPAREN","RPAREN","LBRACK","RBRACK","LBRACE","RBRACE","STRING ","EXTENDS","SEMICOLON","BOOLEAN","INT","IF","WHILE","PRINT","LENGTH","DOT","THIS","NEW","ASSIGN","PLUS ","MINUS","TIMES","DIVIDE","EQ","LE","LT","GE","GT","AND","NOT","TRUE","FALSE","ELSE","COMMA","MAIN","RETURN","INT_ID","BOOLEAN_ID","STRING_ID"
};
string tokname(int tok) {
  return tok<257 || tok>301 ? "BAD_TOKEN" : names[tok-257];
}

int main(int argc,char* argv[]){
    if(argc == 1){
        printf("%s filename\n",argv[0]);
        exit(0);
    }
    string fname = argv[1];
    int tok;
    state_reset(fname);
    while(1){
        tok = yylex();
        if(!tok)
            break;
        switch (tok){
            case ID:
            case STRING:
                printf("%2d:%10s %4d %s\n",line_num,tokname(tok),token_pos,yylval.sval);
                break;
            case INT:
                printf("%2d:%10s %4d %d\n",line_num,tokname(tok),token_pos,yylval.ival);
            default:
                printf("%2d:%10s %4d\n",line_num,tokname(tok),token_pos);
                break;
        }
    }
    return 0;
}
