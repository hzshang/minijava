#include "stand.h"
#include "util.h"
#include "absyn.h"
#include "minijava.tab.h"
#include "error.h"
#include "state.h"
#include "parse_tree.h"
#include "sym.h"
YYSTYPE yylval;
extern A_goal root;
extern int line_num;
extern int yydebug;
extern int yyparse(void);
extern E_error err;
int main(int argc,char* argv[]){
    if(argc == 1){
        ERR("Usage: %s filename [-D] [-O]\n",argv[0]);
        exit(1);
    }
    int enable_out = 0;
    for(int i =1;i< argc;i++){
        if (argv[i][0] == '-'){
            switch(argv[i][1]){
                case 'D':
                    yydebug = 1;
                    break;
                case 'O':
                    enable_out = 1;
                    break;
            }
        }
    }
    string fname = argv[1];
    state_reset(fname);
    yyparse();
    if(!err_count)
        S_check_goal(root);
    if(err_count){
        ERR("%d errors found\n",err_count);
        ERR("parse terminated\n");
        exit(1);
    }
    if(enable_out){
        parse_goal(root);
        printf(";\n");
    }
    return 0;
}

