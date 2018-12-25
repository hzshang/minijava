#include "stand.h"
#include "state.h"
#include "sym.h"
#include "error.h"
#include "absyn.h"
#include "util.h"

#define CHECK do{ \
    if(err_count) return;\
} while(0)
S_sym S_symbol_init(string name,int pos){
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
    r->kind = S_unknown;
    r->pos = pos;
    return r;
}
S_sym S_sym_lookup_method(S_table t,S_sym s){
    ERR("don't support method\n");
    return NULL;
}
S_sym S_sym_lookup_var(S_table t,S_sym s){
    S_table tmp = t;
    while(tmp){
        S_chain p = tmp->dec;
        while(p){
            if(!strcmp(p->val->name,s->name))
                return p->val;
            p = p->next;
        }
        tmp = tmp->parent;
    }
    return NULL;
}
S_sym S_sym_lookup(S_table t,S_sym s){
    if(s->kind == S_method){
        return S_sym_lookup_method(t,s);
    }else{
        return S_sym_lookup_var(t,s);
    }
}

S_table S_table_init(S_dom dom){
    S_table t = safe_malloc(sizeof(*t));
    t->use = NULL;
    t->dec = NULL;
    t->parent = NULL;
    t->dom = dom;
}
S_chain S_chain_init(S_sym id){
    S_chain c = safe_malloc(sizeof(*c));
    c->val = id;
    c->next = NULL;
    return c;
}
void S_table_add_use(S_table t, S_sym s){
    assert(s!=NULL);
    if(!s)return;
    S_chain c = S_chain_init(s);
    c->next = t->use;
    t->use = c;
}

void S_table_add_dec(S_table t,S_sym s){
    assert(s!=NULL);
    if(!s)return;
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
//            stm->tab->parent = tab;
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
/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
/*
 * check begin
 */
void S_check_use_chain(S_table tab){
    S_chain p = tab->use;
    while(p){
        if(p->val->kind !=S_method){
            if (S_sym_lookup(tab,p->val)){
                // found a symbol check
    //            ERR("found symbol :%s\n",p->val->name);
            }else{
                // can't find symbol
                record_error(p->val->pos,strlen(p->val->name),E_semantics);
                show_error("unresolvable symbol '%s'",p->val->name);
            }
        }
        p = p->next;
    }
}
void S_check_dec_chain(S_table tab){
    S_chain p = tab->dec;
    while(p){
        if (p->val->kind !=S_method){
            S_sym r = S_sym_lookup(tab,p->val);
            if (r !=NULL && r!= p->val){
                // found a symbol check
                ERR("redefine symbol found:\n");
                record_error(p->val->pos,strlen(p->val->name),E_semantics);
                show_error("symbol '%s'",p->val->name);
                record_error(r->pos,strlen(r->name),E_semantics);
                show_error("symbol '%s'",r->name);
            }
        }
        p = p->next;
    }
}
#define S_CHECK(x) do{\
    S_check_use_chain(x->tab);\
    S_check_dec_chain(x->tab);\
} while(0)
void S_check_goal(A_goal g){
    S_CHECK(g);
    S_check_main(g->main);
    A_class_list tmp = g->classes;
    while(tmp){
        S_check_class(tmp->val);
        tmp = tmp->next;
    }
}
void S_check_main(A_main main){
    S_CHECK(main);
    S_check_stm(main->stm);
}
void S_check_class(A_class c){
    S_CHECK(c);
    S_check_use_chain(c->tab);
    S_check_use_chain(c->tab);
    A_method_list m = c->methods;
    while(m){
        S_check_method(m->val);
        m = m->next;
    }
}
static inline void S_check_stm_list(A_stm_list list){
    A_stm_list tmp = list;
    while(tmp){
        S_check_stm(tmp->val);
        tmp = tmp->next;
    }
}
void S_check_method(A_method m){
    S_CHECK(m);
    S_check_stm_list(m->stms);
}
void S_check_stm(A_stm s){
    S_CHECK(s);
    switch(s->kind){
        case A_stm_stms:
            S_check_stm_list(s->u.stms.stms);
            break;
        case A_stm_if_else:
            S_check_stm(s->u.cond.yes);
            S_check_stm(s->u.cond.no);
            break;
        case A_stm_loop:
            S_check_stm(s->u.loop.stm);
            break;
        default:
            break;
    }
}

