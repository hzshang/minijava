%{
#include "stand.h"
#include "util.h"
#include "sym.h"
#include "absyn.h"
#include "error.h"
int yylex(void);
A_goal root;
void yyerror(string s){
    state_error(token_pos,"%s",s);
    exit(1);
}
%}

%union {
    int pos;
    int ival;
    string sval;
    
    A_goal goal;
    A_main main;
    A_class class;
    A_class_list class_list;

    A_method_dec method_dec;
    A_method_dec_list method_dec_list;

    A_arg_dec arg_dec;
    A_arg_dec_list arg_dec_list;

    A_var_dec var_dec;
    A_var_dec_list var_dec_list;

    A_type type;

    A_stm stm;
    A_stm_list stm_list;

    A_exp exp;
    A_exp_list exp_list;
}
%token <sval> ID STRING
%token <ival> INT

%token 
    CLASS PUBLIC STATIC VOID LPAREN RPAREN LBRACK RBRACK 
    LBRACE RBRACE EXTENDS SEMICOLON BOOLEAN IF 
    WHILE PRINT LENGTH DOT THIS NEW ASSIGN PLUS MINUS TIMES 
    DIVIDE EQ LE LT GE GT AND NOT TRUE FALSE ELSE COMMA MAIN 
    RETURN INT_ID BOOLEAN_ID STRING_ID
%type <goal> goal
%type <main> main
%type <class> class
%type <class_list> classes
%type <method_dec> method
%type <method_dec_list> methods
%type <arg_dec> arg
%type <arg_dec_list> args
%type <var_dec> var
%type <var_dec_list> vars
%type <type> type type_int type_array type_boolean type_sym
%type <stm> stm stms_stm cond_stm loop_stm print_stm assgin_stm sub_stm
%type <stm_list> stms

%type <exp> exp op_exp sub_exp length_exp method_exp intval_exp boolval_exp id_exp this_exp array_exp new_id_exp reverse_exp exps_exp
%type <exp_list> exps

%left AND
%nonassoc EQ LT GT
%left PLUS MINUS
%left TIMES

%nonassoc IF
%nonassoc ELSE
%start program
%%

program: goal {root = $1;}

goal: main classes { $$ = A_goal_init($1,$2);}

main: CLASS ID LBRACE PUBLIC STATIC VOID MAIN LPAREN STRING_ID LBRACK RBRACK ID RPAREN LBRACE stm RBRACE RBRACE {$$ = A_main_init(S_symbol($2),S_symbol($12),$15);}

classes: class { $$ = A_class_list_init_class($1);}
        | class classes { $$ = A_class_list_init_classes($1,$2);}
/*TODO only support no extend */
class: CLASS ID LBRACE vars methods RBRACE { $$ = A_class_init(S_symbol($2),NULL,vars,methods);}

methods: method { $$ = A_method_list_init_method($1);}
    | method methods {$$ = A_method_list_init_methods($1,$2);}
method: PUBLIC type ID LPAREN arg_dec_list RPAREN LBRACE vars stms RETURN exp SEMICOLON RBRACE {$$ = A_method_dec_init($2,S_symbol($3),$5,$8,$9,$11);}

arg_dec_list:  arg_dec {$$ = A_arg_dec_list_init_arg($1);}
    | arg_dec arg_dec_list {$$ = A_arg_dec_list_init_args($1,$2);}
/* TODO: only support one arg*/
arg_dec: type ID {$$ = A_arg_dec_init($1,S_symbol($2));}

vars:  var { $$ = A_var_dec_list_init_var($1);}
    | var vars {$$ = A_var_dec_list_init_vars($1,$2);}
var: type ID SEMICOLON { $$ = A_var_dec_init($1,S_symbol($2));}

type: INT_ID {$$ = A_type_init_int();}
    | STRING_ID {$$ = A_type_init_string();}
    | BOOLEAN_ID {$$ = A_type_init_boolean();}
    | ID {$$ = A_type_init_sym(S_symbol($1));}

stms: stm {$$ = A_stm_list_init_stm($1);}
    | stm stms {$$ = A_stm_list_init_stms($1,$2);}

stm: LBRACE stms RBRACE {$$ = A_stm_init_stmlist($2);}
    | IF LPAREN exp RPAREN stm ELSE stm {$$ = A_stm_init_cond($3,$5,$7);}
    | WHILE LPAREN exp RPAREN stm { $$ = A_stm_init_loop($3,$5);}
    | PRINT LPAREN exp RPAREN SEMICOLON {$$ = A_stm_init_print($3);}
    | ID ASSIGN exp SEMICOLON {$$ = A_stm_init_assign(S_symbol($1),$3);}
    | ID LBRACK exp RBRACK ASSIGN exp SEMICOLON { $$ = A_stm_init_sub(S_symbol($1),$3,$6);}

exps: exp {$$ = A_exp_list_exp($1);}
    | exp exps {$$ = A_exp_list_init_exps($1,$2);}

exp: exp LPAREN PLUS RPAREN exp { $$ = A_exp_init_op($1,A_plus,$3);}
    | exp LPAREN MINUS RPAREN exp { $$ = A_exp_init_op($1,A_minus,$3);}
    | exp LPAREN TIMES RPAREN exp { $$ = A_exp_init_op($1,A_times,$3);}
    | exp LPAREN AND RPAREN exp { $$ = A_exp_init_op($1,A_and,$3);}
    | exp LPAREN LT RPAREN exp { $$ = A_exp_init_op($1,A_lt,$3);}

