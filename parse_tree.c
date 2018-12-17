#include "stand.h"
#include "parse_tree.h"

void parse_goal(A_goal root){
    printf("Goal(\n");
    parse_main(root->main);
    printf(",\n");
    parse_classes(root->classes);
    printf(")\n");
}

void parse_main(A_main main){
    printf("MainClass(\n");
    parse_sym(main->id);
    printf(",\n");
    parse_sym(main->arg_id);
    printf(",\n");
    parse_stm(main->stm);
    printf(")");
}

void parse_class(A_class cls){
    printf("Class(\n");
    parse_sym(cls->id);
    printf(",\n");
    if(cls->extend)
        parse_sym(cls->extend);
    else
        printf("[NULL]");
    printf(",\n");
    parse_var_decs(cls->vars);
    printf(",\n");
    parse_methods(cls->methods);
    printf(")");
}


void parse_op(A_op op){
    switch(op){
        case A_and:
            printf("&&");
            break;
        case A_plus:
            printf("+");
            break;
        case A_minus:
            printf("-");
            break;
        case A_times:
            printf("*");
            break;
        case A_lt:
            printf("<");
            break;
        default:
            assert(false);
            break;
    }

}
void parse_exp(A_exp exp){
    printf("Exp(");
    switch(exp->kind){
        case A_exp_ops:
            parse_exp(exp->u.op.a);
            parse_op(exp->u.op.op);
            parse_exp(exp->u.op.b);
            break;
        case A_exp_sub:
            parse_exp(exp->u.sub.exp);
            printf("[");
            parse_exp(exp->u.sub.sub);
            printf("]");
            break;
        case A_exp_length:
            parse_exp(exp->u.length.exp);
            printf(".length");
            break;
        case A_exp_method:
            parse_exp(exp->u.method.exp);
            printf(".");
            parse_sym(exp->u.method.method);
            printf("(");
            parse_exps(exp->u.method.args);
            printf(")");
            break;
        case A_exp_int:
            printf("%d",exp->u.intval.val);
            break;
        case A_exp_bool:
            printf("%s",exp->u.boolval.val?"true":"false");
            break;
        case A_exp_id:
            parse_sym(exp->u.id.name);
            break;
        case A_exp_this:
            printf("this");
            break;
        case A_exp_array:
            printf("new Int [");
            parse_exp(exp->u.array.size);
            printf("]");
            break;
        case A_exp_new_id:
            printf("new ");
            parse_sym(exp->u.new_id.name);
            printf("()");
            break;
        case A_exp_reverse:
            printf("!");
            parse_exp(exp->u.reverse.exp);
            break;
        case A_exp_exp:
            printf("(");
            parse_exp(exp->u.exp.exp);
            printf(")");
            break;
        case A_exp_uminus:
            printf("-");
            parse_exp(exp->u.uminus.exp);
            break;
        default:
            assert(false);
            break;
    }
    printf(")");
}

void parse_stm(A_stm stm){
    printf("Statement(\n");
    switch(stm->kind){
        case A_stm_stms:
            parse_stms(stm->u.stms.stms);
            break;
        case A_stm_if_else:
            printf("if(\n");
            parse_exp(stm->u.cond.cond);
            printf(")");
            parse_stm(stm->u.cond.yes);
            printf("else \n");
            parse_stm(stm->u.cond.no);
            break;
        case A_stm_loop:
            printf("while(\n");
            parse_exp(stm->u.loop.cond);
            printf(")");
            parse_stm(stm->u.loop.stm);
            break;
        case A_stm_print:
            printf("print(\n");
            parse_exp(stm->u.print.out);
            printf(")");
            break;
        case A_stm_assign:
            parse_sym(stm->u.assign.name);
            printf("=");
            parse_exp(stm->u.assign.val);
            break;
        case A_stm_sub:
            parse_sym(stm->u.sub.name);
            printf("[");
            parse_exp(stm->u.sub.sub);
            printf("]=");
            parse_exp(stm->u.sub.val);
            break;
        case A_stm_var_dec:
            parse_var_dec(stm->u.var_dec.var_dec);
            break;
        default:
            assert(false);
            break;
    }
    printf(")");
}

void parse_method(A_method m){
    printf("Method(");
    parse_type(m->type);
    printf(",");
    parse_sym(m->name);
    printf(",");
    parse_arg_decs(m->args);
    printf(",");
    parse_stms(m->stms);
    printf(",");
    parse_exp(m->ret);
    printf(")");
}
void parse_type(A_type type){
    switch (type->kind){
        case A_type_int:
            printf("Int");
            break;
        case A_type_array:
            printf("Array");
            break;
        case A_type_boolean:
            printf("Bool");
            break;
        case A_type_string:
            printf("String");
            break;
        case A_type_sym:
            parse_sym(type->u.id.name);
            break;
        default:
            assert(false);
            break;
    }
}

void parse_var_dec(A_var_dec var_dec){
    printf("VarDeclaration(");
    parse_type(var_dec->type);
    printf(",");
    parse_sym(var_dec->name);
    printf(")");
}

void parse_arg_dec(A_arg_dec arg_dec){
    printf("ArgDeclaration(");
    parse_type(arg_dec->type);
    printf(",");
    parse_sym(arg_dec->name);
    printf(")");
}

void parse_sym(S_sym sym){
    printf("%s",sym->name);
}

void parse_arg_decs(A_arg_dec_list list){
    A_arg_dec_list tmp = list;
    printf("ArgDeclarationList(");
    if(!tmp){
        printf("[NULL]");
    }else{
        while(tmp){
            parse_arg_dec(tmp->val);
            if(tmp->next)
                printf(",");
            tmp = tmp->next;
        }
    }
    printf(")");
}


void parse_var_decs(A_var_dec_list list){
    A_var_dec_list tmp = list;
    printf("VarDeclarationList(");
    if(!tmp){
        printf("[NULL]");
    }else{
        while(tmp){
            parse_var_dec(tmp->val);
            if(tmp->next)
                printf(",");
            tmp = tmp->next;
        }
    }
    printf(")");
}

void parse_methods(A_method_list list){
    A_method_list tmp = list;
    printf("MethodList(");
    if(!tmp){
        printf("[NULL]");
    }else{
        while(tmp){
            parse_method(tmp->val);
            if(tmp->next)
                printf(",");
            tmp = tmp->next;
        }
    }
    printf(")");
}
void parse_stms(A_stm_list list){
    A_stm_list tmp = list;
    printf("StatementList(");
    if(!tmp){
        printf("[NULL]");
    }else{
        while(tmp){
            parse_stm(tmp->val);
            if(tmp->next)
                printf(",");
            tmp = tmp->next;
        }
    }
    printf(")");
}
void parse_exps(A_exp_list list){
    A_exp_list tmp = list;
    printf("ExpList(");
    if(!tmp){
        printf("[NULL]");
    }else{
        while(tmp){
            parse_exp(tmp->val);
            if(tmp->next)
                printf(",");
            tmp = tmp->next;
        }
    }
    printf(")");
}
void parse_classes(A_class_list list){
    A_class_list tmp = list;
    printf("ClassList(");
    if(!tmp){
        printf("[NULL]");
    }else{
        while(tmp){
            parse_class(tmp->val);
            if(tmp->next)
                printf(",");
            tmp = tmp->next;
        }
    }
    printf(")");
}

