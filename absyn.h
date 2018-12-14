/*
 * absyn.h
 * Copyright (C) 2018 eric <eric_liu@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef ABSYN_H
#define ABSYN_H
typedef A_stm_* A_stm;
typedef A_stm_list_* A_stm_list;

typedef A_exp_* A_exp;
typedef A_explist_* A_explist;

typedef A_op_* A_op;
typedef A_goal_* A_goal;
typedef A_main_* A_main;

typedef A_class_* A_class;
typedef A_classlist_* A_classlist;

typedef A_var_dec_* A_var_dec;
typedef A_var_dec_list_* A_var_dec_list;

typedef A_method_dec_* A_method_dec;
typedef A_method_dec_list_* A_method_dec_list;

typedef A_arg_dec_* A_arg_dec;
typedef A_arg_dec_list_* A_arg_dec_list;
typedef A_type_* A_type;

typedef enum{A_and,A_plus,A_minus,A_times,A_lt} A_op;

struct A_goal_ {
    A_main mc;
    A_classlist cls;
};

struct A_main_ {
    S_sym id;
    S_sym arg_id;
    A_stm stm;
};

struct A_class_ {
    S_sym id;
    S_sym extend; // if no extend, it will be null
    A_var_dec_list vars;
    A_method_dec_list methods;
};

struct A_classlist_ {
    A_class val;
    A_classlist next;
};

struct A_var_dec_ {
    A_type type;
    S_sym name;
};

struct A_var_dec_list_{
    A_var_dec val;
    A_var_dec_list next;
};

struct A_method_dec_ {
    A_type type;
    S_sym name;
    A_arg_dec_list args;
};

struct A_method_dec_list{
    A_method_dec val;
    A_method_dec_list next;
};

struct A_arg_dec_ {
    A_type type;
    S_sym name;
};

struct A_arg_dec_list_ {
    A_arg_dec val;
    A_arg_dec_list next;
};

struct A_type_{
    enum{A_int,A_array,A_boolean,A_sym} kind;
    union{
        struct{S_sym name;} id;
    }u;
};


struct A_stm_ {
    enum {A_stm,A_if_else,A_while,A_print,A_assign,A_sub_assign} kind;
    union{
        struct{A_stm_list stms;} stms;
        struct{A_exp cond; A_stm stm;} cond;
        struct{A_exp cond; A_stm stm;} loop;
        struct{A_exp out;} print;
        struct{S_sym name; A_exp val;} assign;
        struct{S_sym name; A_exp sub;A_exp val;} sub;
    } u;
};

struct A_stm_list_{
    A_stm val;
    A_stm_list next;
};

struct A_exp_ {
    enum{A_op,A_sub,A_length,A_method,A_int,A_bool,A_id,A_this,A_array,A_new_id,A_reverse,A_exps} kind;
    union{
        struct{A_exp a;A_op op; A_exp b;} op;
        struct{A_exp exp;A_exp sub;} sub;
        struct{A_exp exp;} length;
        struct{A_exp exp;S_sym method;A_explist args} method;
        struct{int val;} intval;
        struct{bool val;} boolval;
        struct{S_sym name;} id;
        //struct{} pointer; this
        struct{A_exp size;} array;
        struct{S_sym name;} new_id;
        struct{A_exp exp;} reverse;
        struct{A_exp exp;} exps;
    } u;
};

struct A_explist_{
    A_exp val;
    A_explist next;
};

A_stm A_stm_init_stmlist(A_stm_list stms);
A_stm A_stm_init_cond(A_exp,A_stm);
A_stm A_stm_init_loop(A_exp,A_stm);
A_stm A_stm_init_print(A_exp);
A_stm A_stm_init_assign(S_sym,A_exp);
A_stm A_stm_init_sub(S_sym,A_exp,A_exp);

A_exp A_exp_init_op(A_exp,A_op,A_exp);
A_exp A_exp_init_sub(A_exp,A_exp);
A_exp A_exp_init_length(A_exp);
A_exp A_exp_init_method(A_exp,S_sym,A_explist);
A_exp A_exp_init_intval(int);
A_exp A_exp_init_boolval(bool);
A_exp A_exp_init_id(S_sym);
A_exp A_exp_init_this();
A_exp A_exp_init_array(A_exp);
A_exp A_exp_init_newid(S_sym);
A_exp A_exp_init_reverse(A_exp);
A_exp A_exp_init_exps(A_exp);


#endif /* !ABSYN_H */
