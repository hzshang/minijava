#include "stand.h"
#include "state.h"
#include "sym.h"
#include "error.h"
#include "absyn.h"

S_sym S_symbol_init(string name){
    /*
    S_sym tmp = S_sym_lookup(t,name);
    if(!tmp){
        tmp = S_sym_init(name);
        S_table_add(t,tmp);
    }
    return tmp;
    */
    S_sym r = safe_malloc(sizeof(*r));
    string tmp_name = safe_malloc(strlen(name)+1);
    strcpy(tmp_name,name);

    r->name = tmp_name;
    r->pos = E_pos_init();
    r->kind = S_unknown;
    E_pos_locate(r->pos,token_pos);
    return r;
}
S_sym S_sym_lookup(S_table t,string name){
    /*
    S_table tmp = t;
    while(tmp){
        S_sym p = tmp->chain;
        while(p){
            if(!strcmp(p->name,name))
                return p;
            p = p->next;
        }
        tmp = tmp->parent;
    }*/
    return NULL;
}

S_table S_table_init(){
    S_table t = safe_malloc(sizeof(*t));
    t->use = NULL;
    t->dec = NULL;
    t->parent = NULL;
}
S_chain S_chain_init(S_sym id){
    S_chain c = safe_malloc(sizeof(*c));
    c->val = id;
    c->next = NULL;
    return c;
}
void S_table_add_use(S_table t, S_sym s){
    S_chain c = S_chain_init(s);
    c->next = t->use;
    t->use = c;
}

void S_table_add_dec(S_table t,S_sym s){
    S_chain c = S_chain_init(s);
    c->next = t->dec;
    t->dec = c;
}
void S_table_add_var_dec(S_table t,A_var_dec v){
    S_table_add_dec(t,v->name);
    S_table_add_type(t,v->type);
}

void S_table_add_arg_dec_list(S_table tab,A_arg_dec_list args){
    A_arg_dec_list tmp = args;
    while(tmp){
        S_table_add_dec(tab,tmp->val->name);
        S_table_add_type(tab,tmp->val->type);
        tmp = tmp->next;
    }
}
void S_table_add_stm_list(S_table tab,A_stm_list list){
    A_stm_list tmp = list;
    while(tmp){
        S_table_add_stm(tab,tmp->val);
        tmp = tmp->next;
    }
}
void S_table_add_type(S_table tab,A_type t){
    if(t->kind == A_type_sym){
        S_table_add_use(tab,t->u.id.name);
    }
}
void S_table_add_exp_list(S_table tab,A_exp_list list){
    A_exp_list tmp = list;
    while(tmp){
        S_table_add_exp(tab,tmp->val);
        tmp = tmp->next;
    }
}

void S_table_add_exp(S_table tab,A_exp t){
    switch(t->kind){
        case A_exp_ops:
            S_table_add_exp(tab,t->u.op.a);
            S_table_add_exp(tab,t->u.op.b);
            break;
        case A_exp_sub:
            S_table_add_exp(tab,t->u.sub.exp);
            S_table_add_exp(tab,t->u.sub.sub);
            break;
        case A_exp_length:
            S_table_add_exp(tab,t->u.length.exp);
            break;
        case A_exp_method:
            S_table_add_exp(tab,t->u.method.exp);
            S_table_add_use(tab,t->u.method.method);
            S_table_add_exp_list(tab,t->u.method.args);
            break;
        case A_exp_id:
            S_table_add_use(tab,t->u.id.name);
            break;
        case A_exp_array:
            S_table_add_exp(tab,t->u.array.size);
            break;
        case A_exp_new_id:
            S_table_add_use(tab,t->u.new_id.name);
            break;
        case A_exp_reverse: 
            S_table_add_exp(tab,t->u.reverse.exp);
            break;
        case A_exp_exp:
            S_table_add_exp(tab,t->u.exp.exp);
            break;
        case A_exp_uminus:
            S_table_add_exp(tab,t->u.uminus.exp);
            break;
        default:
            break;
    }
}

void S_table_add_stm(S_table tab,A_stm stm){
    switch(stm->kind){
        case A_stm_stms:
            S_table_add_stm_list(tab,stm->u.stms.stms);
            break;
        case A_stm_if_else:
            S_table_add_exp(tab,stm->u.cond.cond);
            stm->u.cond.yes->tab->parent = tab;
            stm->u.cond.no->tab->parent = tab;
            break;
        case A_stm_loop:
            S_table_add_exp(tab,stm->u.loop.cond);
            stm->u.loop.stm->tab->parent = tab;
            break;
        case A_stm_print:
            S_table_add_exp(tab,stm->u.print.out);
            break;
        case A_stm_assign:
            S_table_add_use(tab,stm->u.assign.name);
            S_table_add_exp(tab,stm->u.assign.val);
            break;
        case A_stm_sub:
            S_table_add_use(tab,stm->u.sub.name);
            S_table_add_exp(tab,stm->u.sub.sub);
            S_table_add_exp(tab,stm->u.sub.val);
            break;
        case A_stm_var_dec:
            S_table_add_var_dec(tab,stm->u.var_dec.var_dec);
            break;
        default:
            assert(false);
            break;
    }
}

