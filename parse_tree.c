#include "stand.h"
#include "parse_tree.h"
#define STEP 3
#define RSHIFT do{\
    xlen+=STEP;\
}while(0)

#define LSHIFT do{\
    xlen-=STEP;\
}while(0)

static int xlen = 0;
static void printf_fmt(string fmt,...){
    va_list ap;
    if(fmt[0]=='\n'){
        printf("\n%*c",xlen,' ');
        va_start(ap,&fmt[1]);
        vfprintf(stdout,&fmt[1],ap);
        va_end(ap);
    }else{
        va_start(ap,fmt);
        vfprintf(stdout,fmt,ap);
        va_end(ap);
    }
}

void parse_goal(A_goal root){
    printf_fmt("Goal(");
    RSHIFT;
    parse_main(root->main);
    printf_fmt(",");
    parse_classes(root->classes);
    LSHIFT;
    printf_fmt("\n)\n");
}

void parse_main(A_main main){
    printf_fmt("\nMainClass(");
    RSHIFT;
    parse_sym(main->id);
    printf_fmt(",");
    parse_sym(main->arg_id);
    printf_fmt(",");
    parse_stm(main->stm);
    LSHIFT;
    printf_fmt("\n)");
}

void parse_class(A_class cls){
    printf_fmt("\nClass(");
    RSHIFT;
    parse_sym(cls->id);
    printf_fmt(",");
    if(cls->extend)
        parse_sym(cls->extend);
    else
        printf_fmt("\n[NULL]");
    printf_fmt(",");
    parse_var_decs(cls->vars);
    printf_fmt(",");
    parse_methods(cls->methods);
    LSHIFT;
    printf_fmt("\n)");
}


static inline void parse_exp_op(A_exp a,A_op op,A_exp b){
    printf_fmt("\nExpOp(");
    RSHIFT;
    switch(op){
        case A_and:
            printf_fmt("\n&&");
            break;
        case A_plus:
            printf_fmt("\n+");
            break;
        case A_minus:
            printf_fmt("\n-");
            break;
        case A_times:
            printf_fmt("\n*");
            break;
        case A_lt:
            printf_fmt("\n<");
            break;
        default:
            assert(false);
            break;
    }
    printf_fmt(",");
    parse_exp(a);
    printf_fmt(",");
    parse_exp(b);
    LSHIFT;
    printf_fmt(")");
}
static inline void parse_exp_uminus(A_exp a){
    printf_fmt("\nExpUminus(");
    RSHIFT;
    parse_exp(a);
    LSHIFT;
    printf_fmt("\n)");
}
static inline void parse_exp_sub(A_exp val,A_exp sub){
    printf_fmt("\nExpSub");
    RSHIFT;
    parse_exp(val);
    printf(",");
    parse_exp(sub);
    LSHIFT;
    printf_fmt("\n)");
}
static inline void parse_exp_length(A_exp exp){
    printf_fmt("\nExpLength");
    RSHIFT;
    parse_exp(exp);
    LSHIFT;
    printf_fmt("\n)");
}
static inline void parse_exp_method(A_exp exp,S_sym name,A_exp_list exps){
    printf_fmt("\nExpMethod(");
    RSHIFT;
    parse_exp(exp);
    printf_fmt(",");
    parse_sym(name);
    printf_fmt(",");
    parse_exps(exps);
    LSHIFT;
    printf_fmt("\n)");
}
static inline void parse_exp_array(A_exp size){
    printf_fmt("\nExpIntArray(");
    RSHIFT;
    parse_exp(size);
    LSHIFT;
    printf_fmt("\n)");
}
static inline void parse_exp_reverse(A_exp exp){
    printf_fmt("\nExpReverse(");
    RSHIFT;
    parse_exp(exp);
    LSHIFT;
    printf_fmt("\n)");
}
static inline void parse_exp_new_id(S_sym name){
    printf_fmt("\nExpNew(");
    RSHIFT;
    parse_sym(name);
    LSHIFT;
    printf_fmt("\n)");
}
void parse_exp(A_exp exp){
    printf_fmt("\nExp(");
    RSHIFT;
    switch(exp->kind){
        case A_exp_ops:
            parse_exp_op(exp->u.op.a,exp->u.op.op,exp->u.op.b);
            break;
        case A_exp_sub:
            parse_exp_sub(exp->u.sub.exp,exp->u.sub.sub);
            break;
        case A_exp_length:
            parse_exp_length(exp->u.length.exp);
            break;
        case A_exp_method:
            parse_exp_method(exp->u.method.exp,exp->u.method.method,exp->u.method.args);
            break;
        case A_exp_id:
            parse_sym(exp->u.id.name);
            break;
        case A_exp_array:
            parse_exp_array(exp->u.array.size);
            break;
        case A_exp_new_id:
            parse_exp_new_id(exp->u.new_id.name);
            break;
        case A_exp_reverse:
            parse_exp_reverse(exp->u.reverse.exp);
            break;
        case A_exp_exp:
            parse_exp(exp->u.exp.exp);
            break;
        case A_exp_uminus:
            parse_exp_uminus(exp->u.uminus.exp);
            break;
        /* code don't have fmt bellow*/
        case A_exp_int:
            printf_fmt("\n%d",exp->u.intval.val);
            break;
        case A_exp_bool:
            printf_fmt("\n%s",exp->u.boolval.val?"true":"false");
            break;
        case A_exp_this:
            RSHIFT;
            printf_fmt("\nthis");
            LSHIFT;
            break;
        default:
            assert(false);
            break;
    }
    LSHIFT;
    printf_fmt("\n)");
}

void parse_stm(A_stm stm){
    printf_fmt("\nStatement(");
    RSHIFT;
    switch(stm->kind){
        case A_stm_stms:
            parse_stms(stm->u.stms.stms);
            break;
        case A_stm_if_else:
            printf_fmt("if(");
            parse_exp(stm->u.cond.cond);
            printf_fmt(")");
            parse_stm(stm->u.cond.yes);
            printf_fmt("else");
            parse_stm(stm->u.cond.no);
            break;
        case A_stm_loop:
            printf_fmt("\nwhile(");
            RSHIFT;
            parse_exp(stm->u.loop.cond);
            printf_fmt(")");
            parse_stm(stm->u.loop.stm);
            break;
        case A_stm_print:
            printf_fmt("\nprint(");
            RSHIFT;
            parse_exp(stm->u.print.out);
            LSHIFT;
            printf_fmt("\n)");
            break;
        case A_stm_assign:
            parse_sym(stm->u.assign.name);
            printf_fmt("=");
            parse_exp(stm->u.assign.val);
            break;
        case A_stm_sub:
            parse_sym(stm->u.sub.name);
            printf_fmt("[");
            parse_exp(stm->u.sub.sub);
            printf_fmt("]=");
            parse_exp(stm->u.sub.val);
            break;
        case A_stm_var_dec:
            parse_var_dec(stm->u.var_dec.var_dec);
            break;
        default:
            assert(false);
            break;
    }
    LSHIFT;
    printf_fmt("\n)");
}

void parse_method(A_method m){
    printf_fmt("\nMethod(");
    RSHIFT;
    parse_type(m->type);
    printf_fmt(",");
    parse_sym(m->name);
    printf_fmt(",");
    parse_arg_decs(m->args);
    printf_fmt(",");
    parse_stms(m->stms);
    printf_fmt(",");
    parse_exp(m->ret);
    LSHIFT;
    printf_fmt(")");
}
void parse_type(A_type type){
    printf_fmt("\n");
    RSHIFT;
    switch (type->kind){
        case A_type_int:
            printf_fmt("Int");
            break;
        case A_type_array:
            printf_fmt("Array");
            break;
        case A_type_boolean:
            printf_fmt("Bool");
            break;
        case A_type_string:
            printf_fmt("String");
            break;
        case A_type_sym:
            parse_sym(type->u.id.name);
            break;
        default:
            assert(false);
            break;
    }
    LSHIFT;
}

void parse_var_dec(A_var_dec var_dec){
    printf_fmt("\nVarDeclaration(");
    RSHIFT;
    parse_type(var_dec->type);
    printf_fmt(",");
    parse_sym(var_dec->name);
    LSHIFT;
    printf_fmt("\n)");
}

void parse_arg_dec(A_arg_dec arg_dec){
    printf_fmt("\nArgDeclaration(");
    RSHIFT;
    parse_type(arg_dec->type);
    printf_fmt(",");
    parse_sym(arg_dec->name);
    LSHIFT;
    printf_fmt("\n)");
}

void parse_sym(S_sym sym){
    printf_fmt("\n%s",sym->name);
    /*
    printf_fmt("\nSymbol(%s)",sym->name);
    printf_fmt("\n");
    */
}

void parse_arg_decs(A_arg_dec_list list){
    A_arg_dec_list tmp = list;
    printf_fmt("\nArgDeclarationList(");
    RSHIFT;
    if(!tmp){
        printf_fmt("\n[NULL]");
    }else{
        while(tmp){
            parse_arg_dec(tmp->val);
            if(tmp->next)
                printf_fmt(",");
            tmp = tmp->next;
        }
    }
    LSHIFT;
    printf_fmt("\n)");
}


void parse_var_decs(A_var_dec_list list){
    A_var_dec_list tmp = list;
    printf_fmt("\nVarDeclarationList(");
    RSHIFT;
    if(!tmp){
        printf_fmt("\n[NULL]");
    }else{
        while(tmp){
            parse_var_dec(tmp->val);
            if(tmp->next)
                printf_fmt(",");
            tmp = tmp->next;
        }
    }
    LSHIFT;
    printf_fmt("\n)");
}

void parse_methods(A_method_list list){
    A_method_list tmp = list;
    printf_fmt("\nMethodList(");
    RSHIFT;
    if(!tmp){
        printf_fmt("\n[NULL]");
    }else{
        while(tmp){
            parse_method(tmp->val);
            if(tmp->next)
                printf_fmt(",");
            tmp = tmp->next;
        }
    }
    LSHIFT;
    printf_fmt("\n)");
}
void parse_stms(A_stm_list list){
    A_stm_list tmp = list;
    printf_fmt("\nStatementList(");
    RSHIFT;
    if(!tmp){
        printf_fmt("\n[NULL]");
    }else{
        while(tmp){
            parse_stm(tmp->val);
            if(tmp->next)
                printf_fmt(",");
            tmp = tmp->next;
        }
    }
    LSHIFT;
    printf_fmt("\n)");
}
void parse_exps(A_exp_list list){
    A_exp_list tmp = list;
    printf_fmt("\nExpList(");
    RSHIFT;
    if(!tmp){
        printf_fmt("\n[NULL]");
    }else{
        while(tmp){
            parse_exp(tmp->val);
            if(tmp->next)
                printf_fmt(",");
            tmp = tmp->next;
        }
    }
    LSHIFT;
    printf_fmt("\n)");
}
void parse_classes(A_class_list list){
    A_class_list tmp = list;
    printf_fmt("\nClassList(");
    RSHIFT;
    if(!tmp){
        printf_fmt("\n[NULL]");
    }else{
        while(tmp){
            parse_class(tmp->val);
            if(tmp->next)
                printf_fmt(",");
            tmp = tmp->next;
        }
    }
    LSHIFT;
    printf_fmt("\n)");
}


void parse_op_reverse(A_exp a){
    printf_fmt("\nReverse(");
    RSHIFT;
    parse_exp(a);
    LSHIFT;
    printf_fmt("\n)");
}

void parse_op_uminus(A_exp a){
    printf_fmt("\nUminus(");
    RSHIFT;
    parse_exp(a);
    LSHIFT;
    printf_fmt("\n)");
}
