/*
 * absyn.h
 * Copyright (C) 2018 eric <eric_liu@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef ABSYN_H
#define ABSYN_H
typedef A_stm_* A_stm;
typedef A_exp_* A_exp;
typedef A_op_* A_op;
typedef A_explist_* A_explist;
typedef enum{ A_and,A_plus,A_minus,A_times,A_lt} A_op;

struct A_stm_ {
    enum {A_stm,A_if_else,A_while,A_print,A_assign,A_sub_assign} kind;
    union{
        struct{A_stm stm;} stm;
        struct{A_exp cond; A_stm stm;} cond;
        struct{A_exp cond; A_stm stm;} loop;
        struct{A_exp out;} print;
        struct{S_sym name; A_exp val;} assign;
        struct{S_sym name; A_exp sub;A_exp val;} sub;
    } u;
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
    }u;
};

struct A_explist_{
    A_exp val;
    A_explist_ next;
};

#endif /* !ABSYN_H */
