#include "stand.h"
#include "sym.h"
#include "absyn.h"
#include "error.h"
A_goal A_goal_init(A_main main,A_class_list list){
    A_goal g = safe_malloc(sizeof(*g));
    g->main = main;
    g->classes = list;
    g->tab = S_table_init(S_dom_goal);
    if(err_count)
        return g;
    main->tab->parent = g->tab;
    S_table_add_dec(g->tab,main->id);

    A_class_list tmp = list;
    while(tmp){
        tmp->val->tab->parent = g->tab;
        S_table_add_dec(g->tab,tmp->val->id);
        tmp = tmp->next;
    };
    return g;
}

A_main A_main_init(S_sym id,S_sym arg_id,A_stm stm){
    A_main mc = safe_malloc(sizeof(*mc));
    mc->id = id;
    mc->arg_id = arg_id;
    mc->stm = stm;
    mc->tab = S_table_init(S_dom_main);
    if(err_count)
        return mc;
    S_table_add_dec(mc->tab,arg_id);
    S_table_add_stm(mc->tab,stm);
    return mc;
}

A_class A_class_init(S_sym id ,S_sym extend,A_var_dec_list vars,A_method_list methods){
    A_class cls = safe_malloc(sizeof(*cls));
    cls->id = id;
    cls->extend = extend;
    cls->methods = methods;
    cls->vars = vars;
    cls->tab = S_table_init(S_dom_class);
    if(err_count)
        return cls;
    if(extend)
        S_table_add_use(cls->tab,extend);
    A_var_dec_list t = vars;
    while(t){
        S_table_add_var_dec(cls->tab,t->val);
        t = t->next;
    }
    A_method_list tmp = methods;
    while(tmp){
        tmp->val->tab->parent = cls->tab;
        S_table_add_dec(cls->tab,tmp->val->name);
        tmp = tmp->next;
    }

    return cls;
}

A_var_dec A_var_dec_init(A_type type,S_sym name){
    A_var_dec v = safe_malloc(sizeof(*v));
    v->type = type;
    v->name = name;
    return v;
}
A_method A_method_init(A_type type,S_sym name,A_arg_dec_list args, A_stm_list stms,A_exp ret){
    A_method m = safe_malloc(sizeof(*m));
    m->type = type;
    m->name = name;
    m->args = args;
    m->stms = stms;
    m->ret = ret;
    m->tab = S_table_init(S_dom_method);
    if(err_count)
        return m;
    S_table_add_type(m->tab,type);
    S_table_add_arg_dec_list(m->tab,args);
    S_table_add_stm_list(m->tab,stms);
    S_table_add_exp(m->tab,ret);
    return m;
}

A_arg_dec A_arg_dec_init(A_type type,S_sym name){
    A_arg_dec a = safe_malloc(sizeof(*a));
    a->type = type;
    a->name = name;
    return a;
}

A_type A_type_init_int(){
    A_type t = safe_malloc(sizeof(*t));
    t->kind = A_type_int;
    return t;
}
A_type A_type_init_array(){
    A_type t = safe_malloc(sizeof(*t));
    t->kind = A_type_array;
    return t;
}
A_type A_type_init_boolean(){
    A_type t = safe_malloc(sizeof(*t));
    t->kind = A_type_boolean;
    return t;
}
A_type A_type_init_string(){
    A_type t = safe_malloc(sizeof(*t));
    t->kind = A_type_string;
    return t;
}
A_type A_type_init_sym(S_sym name){
    A_type t = safe_malloc(sizeof(*t));
    t->kind = A_type_sym;
    t->u.id.name = name;
    return t;
}

A_stm A_stm_init_stm_list(A_stm_list stms){
    A_stm s = safe_malloc(sizeof(*s));
    s->kind = A_stm_stms;
    s->u.stms.stms = stms;
    s->tab = S_table_init(S_dom_stm);
    if(err_count)
        return s;
    S_table_add_stm_list(s->tab,stms);
    return s;
}
A_stm A_stm_init_cond(A_exp cond,A_stm yes, A_stm no){
    A_stm s = safe_malloc(sizeof(*s));
    s->kind = A_stm_if_else;
    s->u.cond.cond = cond;
    s->u.cond.yes = yes;
    s->u.cond.no = no;

    s->tab = S_table_init(S_dom_stm);
    //S_table_add_stm(s->tab,yes);
    //S_table_add_stm(s->tab,no);
    return s;
}
A_stm A_stm_init_loop(A_exp cond, A_stm stm){
    A_stm s = safe_malloc(sizeof(*s));
    s->kind = A_stm_loop;
    s->u.loop.cond = cond;
    s->u.loop.stm = stm;
    s->tab = S_table_init(S_dom_stm);
//    S_table_add_stm(s->tab,stm);
    return s;
}
A_stm A_stm_init_print(A_exp out){
    A_stm s = safe_malloc(sizeof(*s));
    s->kind = A_stm_print;
    s->u.print.out = out;
    s->tab = S_table_init(S_dom_stm);
    return s;
}
A_stm A_stm_init_assign(S_sym name,A_exp val){
    A_stm s = safe_malloc(sizeof(*s));
    s->kind = A_stm_assign;
    s->u.assign.name = name;
    s->u.assign.val = val;
    s->tab = S_table_init(S_dom_stm);
    return s;
}
A_stm A_stm_init_sub(S_sym name,A_exp sub,A_exp val){
    A_stm s = safe_malloc(sizeof(*s));
    s->kind = A_stm_sub;
    s->u.sub.name = name;
    s->u.sub.sub = sub;
    s->u.sub.val = val;
    s->tab = S_table_init(S_dom_stm);
    return s;
}

A_stm A_stm_init_var(A_var_dec v){
    A_stm s = safe_malloc(sizeof(*s));
    s->kind = A_stm_var_dec;
    s->u.var_dec.var_dec = v;
    s->tab = S_table_init(S_dom_stm);
    return s;
}

A_exp A_exp_init_op(A_exp a,A_op op,A_exp b){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_ops;
    e->u.op.a = a;
    e->u.op.op = op;
    e->u.op.b = b;
    return e;
}
A_exp A_exp_init_sub(A_exp exp,A_exp sub){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_sub;
    e->u.sub.exp = exp;
    e->u.sub.sub = sub;
    return e;
}
A_exp A_exp_init_length(A_exp exp){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_length;
    e->u.length.exp = exp;
    return e;
}
A_exp A_exp_init_method(A_exp exp,S_sym name,A_exp_list args){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_method;
    e->u.method.exp = exp;
    e->u.method.method = name;
    e->u.method.args = args;
    return e;
}
A_exp A_exp_init_intval(int val){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_int;
    e->u.intval.val = val;
    return e;
}
A_exp A_exp_init_boolval(bool val){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_bool;
    e->u.boolval.val = val;
    return e;
}
A_exp A_exp_init_id(S_sym name){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_id;
    e->u.id.name = name;
    return e;
}
A_exp A_exp_init_this(){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_this;
    return e;
}
A_exp A_exp_init_array(A_exp size){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_array;
    e->u.array.size = size;
    return e;
}
A_exp A_exp_init_newid(S_sym name){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_new_id;
    e->u.new_id.name = name;
    return e;
}
A_exp A_exp_init_reverse(A_exp exp){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_reverse;
    e->u.reverse.exp = exp;
    return e;
}
A_exp A_exp_init_exp(A_exp exp){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_exp;
    e->u.exp.exp = exp;
    return e;
}
A_exp A_exp_init_uminus(A_exp exp){
    A_exp e = safe_malloc(sizeof(*e));
    e->kind = A_exp_uminus;
    e->u.uminus.exp = exp;
    return e;
}

A_class_list A_class_list_init_class(A_class c){
    if(!c) return NULL;
    A_class_list l = safe_malloc(sizeof(*l));
    l->val = c;
    l->next = NULL;
    return l;
}

A_class_list A_class_list_init_classes(A_class c,A_class_list next){
    if(!c)return NULL;
    A_class_list l = safe_malloc(sizeof(*l));
    l->val = c;
    l->next = next;
    return l;
}

A_stm_list A_stm_list_init_stm(A_stm s){
    if(!s)return NULL;
    A_stm_list l = safe_malloc(sizeof(*l));
    l->val = s;
    l->next = NULL;
    return l;
}

A_stm_list A_stm_list_init_stms(A_stm s,A_stm_list next){
    if(!s)return NULL;
    A_stm_list l = safe_malloc(sizeof(*l));
    l->val = s;
    l->next = next;
    return l;
}

A_var_dec_list A_var_dec_list_init_var(A_var_dec val){
    if(!val)return NULL;
    A_var_dec_list l = safe_malloc(sizeof(*l));
    l->val = val;
    l->next = NULL;
    return l;
}
A_var_dec_list A_var_dec_list_init_vars(A_var_dec val,A_var_dec_list next){
    if(!val)return NULL;
    A_var_dec_list l = safe_malloc(sizeof(*l));
    l->val = val;
    l->next = next;
    return l;
}
A_arg_dec_list A_arg_dec_list_init_arg_dec(A_arg_dec arg){
    if(!arg) return NULL;
    A_arg_dec_list l = safe_malloc(sizeof(*l));
    l->val = arg;
    l->next = NULL;
    return l;
}
A_arg_dec_list A_arg_dec_list_init_arg_decs(A_arg_dec arg,A_arg_dec_list next){
    if(!arg) return NULL;
    A_arg_dec_list l = safe_malloc(sizeof(*l));
    l->val = arg;
    l->next = next;
    return l;
}
A_method_list A_method_list_init_method(A_method val){
    if(!val) return NULL;
    A_method_list l = safe_malloc(sizeof(*l));
    l->val = val;
    l->next = NULL;
    return l;
}
A_method_list A_method_list_init_methods(A_method val,A_method_list next){
    if(!val) return NULL;
    A_method_list l = safe_malloc(sizeof(*l));
    l->val = val;
    l->next = next;
    return l;
}

A_exp_list A_exp_list_init_exp(A_exp val){
    if(!val) return NULL;
    A_exp_list l = safe_malloc(sizeof(*l));
    l->val = val;
    l->next = NULL;
    return l;
}
A_exp_list A_exp_list_init_exps(A_exp val,A_exp_list next){
    if(!val) return NULL;
    A_exp_list l = safe_malloc(sizeof(*l));
    l->val = val;
    l->next = next;
    return l;
}
A_class_list A_class_list_init_null(){
    return NULL;
}

A_method_list A_method_list_init_null(){
    return NULL;
}

A_var_dec_list A_var_dec_list_init_null(){
    return NULL;
}

A_arg_dec_list A_arg_dec_list_init_null(){
    return NULL;
}
A_stm_list A_stm_list_init_null(){
    return NULL;
}

A_exp_list A_exp_list_init_null(){
    return NULL;
}

