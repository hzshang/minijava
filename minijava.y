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

    A_method method;
    A_method_list method_list;

    A_arg_dec arg_dec;
    A_arg_dec_list arg_dec_list;

    A_var_dec var_dec;
    A_var_dec_list var_dec_list;

    A_type type;

    A_stm stm;
    A_stm_list stm_list;

    A_exp exp;
}
%token <sval> ID STRING
%token <ival> INT

%token 
    CLASS PUBLIC STATIC VOID LPAREN RPAREN LBRACK RBRACK 
    LBRACE RBRACE EXTENDS SEMICOLON BOOLEAN IF 
    WHILE PRINT LENGTH DOT THIS NEW ASSIGN PLUS MINUS TIMES 
    DIVIDE EQ LE LT GE GT AND REVERSE TRUE FALSE ELSE COMMA MAIN 
    RETURN INT_ID BOOLEAN_ID STRING_ID

%type <goal> goal
%type <main> main
%type <class> class
%type <class_list> classes
%type <method> method
%type <method_list> methods
%type <arg_dec> arg arg_next
%type <arg_dec_list> args arg_nexts
%type <var_dec> var
%type <var_dec_list> vars
%type <type> type
%type <stm> stm
%type <stm_list> stms
%type <exp> exp


/* 先执行的排在下面*/
%nonassoc LT AND
%left PLUS MINUS
%left TIMES

%right REVERSE
%left LBRACK
%left DOT
%start program

%%

program: goal {root = $1;}
    ;

goal: main classes { $$ = A_goal_init($1,$2);}
    ;

main: CLASS ID LBRACE PUBLIC STATIC VOID MAIN LPAREN STRING_ID LBRACK RBRACK ID RPAREN LBRACE stm RBRACE RBRACE {$$ = A_main_init(S_symbol($2),S_symbol($12),$15);}
    ;

classes: {$$ = A_class_list_init_null();}
    | class classes { $$ = A_class_list_init_classes($1,$2);}
    ;

/*TODO: only support no extend */
/*TODO: only support no args */
class: CLASS ID LBRACE vars methods RBRACE { $$ = A_class_init(S_symbol($2),NULL,$4,$5);}
    ;
methods: { $$ = A_method_list_init_null();}
    | method methods {$$ = A_method_list_init_methods($1,$2);}
    ;
method: PUBLIC type ID LPAREN args RPAREN LBRACE stms RETURN exp SEMICOLON RBRACE {$$ = A_method_init($2,S_symbol($3),$5,$8,$10);}
    ;

args: arg args {$$ = A_arg_dec_list_init_args($1,$2);}
    | { $$ = A_arg_dec_list_init_null();}
    ;

/* TODO: only support one arg*/
arg_next: COMMA arg {$$ = $2;}

arg_nexts:  arg_next arg_nexts {$$ = A_arg_dec_list_init_args($1,$2);}
    | {$$ = A_arg_dec_list_init_null();}
    ;

arg: type ID { $$ = A_arg_dec_init($1,S_symbol($2));}
    ;

args: arg arg_nexts {$$ = A_arg_dec_list_init_args($1,$2);} 
    ;

vars: {$$ = A_var_dec_list_init_null();}
    | var vars {$$ = A_var_dec_list_init_vars($1,$2);}
    ;

var: type ID SEMICOLON { $$ = A_var_dec_init($1,S_symbol($2));}
    ;

type: INT_ID {$$ = A_type_init_int();}
    | STRING_ID {$$ = A_type_init_string();}
    | BOOLEAN_ID {$$ = A_type_init_boolean();}
    | ID {$$ = A_type_init_sym(S_symbol($1));}
    ;

stms: {$$ = A_stm_list_init_null();}
    | stm stms {$$ = A_stm_list_init_stms($1,$2);}
    ;

stm: LBRACE stms RBRACE {$$ = A_stm_init_stmlist($2);}
    | IF LPAREN exp RPAREN stm ELSE stm {$$ = A_stm_init_cond($3,$5,$7);}
    | WHILE LPAREN exp RPAREN stm { $$ = A_stm_init_loop($3,$5);}
    | PRINT LPAREN exp RPAREN SEMICOLON {$$ = A_stm_init_print($3);}
    | ID ASSIGN exp SEMICOLON {$$ = A_stm_init_assign(S_symbol($1),$3);}
    | ID LBRACK exp RBRACK ASSIGN exp SEMICOLON { $$ = A_stm_init_sub(S_symbol($1),$3,$6);}
    | var {$$ = A_stm_init_var($1);}
    ;

exp:  ID { $$ = A_exp_init_id(S_symbol($1));}
    | INT { $$ = A_exp_init_intval($1);}
    | TRUE { $$ = A_exp_init_boolval(true);}
    | FALSE { $$ = A_exp_init_boolval(false);}
    | THIS { $$ = A_exp_init_this();}
    | NEW ID LPAREN RPAREN {$$ = A_exp_init_newid(S_symbol($2));}
    | NEW INT_ID LBRACK exp RBRACK { $$ = A_exp_init_array($4);}
    | REVERSE exp { $$ = A_exp_init_reverse($2);}
    | LBRACK exp RBRACK {$$ = A_exp_init_exp($2);}
    | exp PLUS exp %prec PLUS { $$ = A_exp_init_op($1,A_plus,$3);}
    /* TODO: not support -132 */
    | exp MINUS exp %prec MINUS { $$ = A_exp_init_op($1,A_minus,$3);}
    | exp TIMES exp %prec TIMES { $$ = A_exp_init_op($1,A_times,$3);}
    | exp AND exp %prec AND { $$ = A_exp_init_op($1,A_and,$3);}
    | exp LT exp %prec LT { $$ = A_exp_init_op($1,A_lt,$3);}
    | exp LBRACK exp RBRACK { $$ = A_exp_init_sub($1,$3);}
    | exp DOT LENGTH { $$ = A_exp_init_length($1);}
    /* TODO: only suport no args */
    | exp DOT ID LPAREN RPAREN { $$ = A_exp_init_method($1,S_symbol($3));}
    ;


