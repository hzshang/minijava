#include "stand.h"
#include "util.h"
#include "absyn.h"
#include "minijava.tab.h"
#include "error.h"
#include "parse_tree.h"

extern A_goal root;
YYSTYPE yylval;
extern int line_num;
extern int yylex();
extern int yydebug;
extern int yyparse(void);

int main(int argc,char* argv[]){
    if(argc == 1){
        printf("%s filename [debug]\n",argv[0]);
        exit(0);
    }
    yydebug = argv[2]?1:0;
    string fname = argv[1];
    state_reset(fname);
    yyparse();
    parse_goal(root);    
    return 0;
}
