#include "stand.h"
#include "util.h"
#include "absyn.h"
#include "minijava.tab.h"
#include "error.h"
#include "state.h"
#include "parse_tree.h"

YYSTYPE yylval;
extern A_goal root;
extern int line_num;
extern int yylex();
extern int yydebug;
extern int yyparse(void);
extern E_error err;
int main(int argc,char* argv[]){
    if(argc == 1){
        ERR("Usage: %s filename [enable_out] [enable_debug]\n",argv[0]);
        exit(1);
    }
    yydebug = argc == 4 ?1:0;
    string fname = argv[1];
    state_reset(fname);
    yyparse();
    if(err->kind != E_none){
        fprintf(stderr,"parse terminated\n");
        exit(1);
    }
    if(argc > 2 && err->kind == E_none){
        parse_goal(root);
        printf(";\n");
    }
    return 0;
}
