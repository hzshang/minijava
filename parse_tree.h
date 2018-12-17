/*
 * parse_tree.h
 * Copyright (C) 2018 eric <eric_liu@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PARSE_TREE_H
#define PARSE_TREE_H
#include "sym.h"
#include "absyn.h"
void parse_goal(A_goal root);
void parse_main(A_main main);

void parse_classes(A_class_list list);
void parse_class(A_class cls);

void parse_exps(A_exp_list list);
void parse_exp(A_exp exp);

void parse_stms(A_stm_list list);
void parse_stm(A_stm stm);

void parse_methods(A_method_list list);
void parse_method(A_method m);

void parse_type(A_type type);

void parse_var_dec(A_var_dec var_dec);
void parse_var_decs(A_var_dec_list list);

void parse_arg_dec(A_arg_dec arg_dec);
void parse_arg_decs(A_arg_dec_list list);

void parse_sym(S_sym sym);







#endif /* !PARSE_TREE_H */
