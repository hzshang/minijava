/*
 * sym.h
 * Copyright (C) 2018 eric <eric_liu@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SYM_H
#define SYM_H
#include "util.h"
#include "error.h"
typedef struct A_stm_* A_stm;
typedef struct A_stm_list_* A_stm_list;

typedef struct A_exp_* A_exp;
typedef struct A_exp_list_* A_exp_list;

typedef struct A_goal_* A_goal;
typedef struct A_main_* A_main;

typedef struct A_class_* A_class;
typedef struct A_class_list_* A_class_list;

typedef struct A_var_dec_* A_var_dec;
typedef struct A_var_dec_list_* A_var_dec_list;

typedef struct A_method_* A_method;
typedef struct A_method_list_* A_method_list;

typedef struct A_arg_dec_* A_arg_dec;
typedef struct A_arg_dec_list_* A_arg_dec_list;
typedef struct A_type_* A_type;
typedef struct S_sym_* S_sym;
typedef struct S_table_* S_table;
typedef struct S_chain_* S_chain;
typedef enum {S_method,S_class,S_var,S_mainclass,S_unknown} S_type;

#define S_SYM_THIS ((S_sym)1)
#define S_SYM_BAD ((S_sym)2)

struct S_sym_ {
    string name;
    E_pos pos;
    S_type kind;
    union {
        struct{ S_sym parent;} method;
    }u;
};
struct S_chain_{
    S_sym val;
    S_chain next;
};
struct S_table_ {
    S_chain dec;
    S_chain use;
    S_table parent;
};

S_sym S_symbol_init(string name);
S_sym S_sym_lookup(S_table,string);

S_table S_table_init();
void S_table_add_use(S_table,S_sym);
void S_table_add_dec(S_table,S_sym);

void S_table_add_exp();
void S_table_add_type(S_table tab,A_type t);
void S_table_add_stm_list(S_table tab,A_stm_list list);
void S_table_add_arg_dec_list(S_table tab,A_arg_dec_list args);

void S_table_add_var_dec(S_table t,A_var_dec v);
void S_table_add_type(S_table tab,A_type t);
void S_table_add_exp_list(S_table tab,A_exp_list list);
void S_table_add_exp(S_table tab,A_exp t);
void S_table_add_stm(S_table tab,A_stm stm);

#endif /* !SYM_H */
