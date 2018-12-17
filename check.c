#include "stand.h"
#include "util.h"
#include "absyn.h"
#include "minijava.tab.h"
#include "error.h"
extern A_goal root;
YYSTYPE yylval;
extern int line_num;
extern int yylex();
extern int yydebug;
extern int yyparse(void);
string names[] = {"ID","CLASS","PUBLIC","STATIC","VOID","LPAREN","RPAREN","LBRACK","RBRACK","LBRACE","RBRACE","STRING ","EXTENDS","SEMICOLON","BOOLEAN","INT","IF","WHILE","PRINT","LENGTH","DOT","THIS","NEW","ASSIGN","PLUS ","MINUS","TIMES","DIVIDE","EQ","LE","LT","GE","GT","AND","REVERSE","TRUE","FALSE","ELSE","COMMA","MAIN","RETURN","INT_ID","BOOLEAN_ID","STRING_ID"
};
string tokname(int tok) {
  return tok<257 || tok>301 ? "BAD_TOKEN" : names[tok-257];
}

int main(int argc,char* argv[]){
    if(argc == 1){
        printf("%s filename [debug level]\n",argv[0]);
        exit(0);
    }
    yydebug = argv[2]?atoi(argv[2]):0;
    string fname = argv[1];
    state_reset(fname);
    yyparse();

    return 0;
}
