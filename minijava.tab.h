/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_MINIJAVA_TAB_H_INCLUDED
# define YY_YY_MINIJAVA_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    STRING = 259,
    INT = 260,
    CLASS = 261,
    PUBLIC = 262,
    STATIC = 263,
    VOID = 264,
    LPAREN = 265,
    RPAREN = 266,
    LBRACK = 267,
    RBRACK = 268,
    LBRACE = 269,
    RBRACE = 270,
    EXTENDS = 271,
    SEMICOLON = 272,
    BOOLEAN = 273,
    IF = 274,
    WHILE = 275,
    PRINT = 276,
    LENGTH = 277,
    DOT = 278,
    THIS = 279,
    NEW = 280,
    ASSIGN = 281,
    PLUS = 282,
    MINUS = 283,
    TIMES = 284,
    DIVIDE = 285,
    EQ = 286,
    LE = 287,
    LT = 288,
    GE = 289,
    GT = 290,
    AND = 291,
    REVERSE = 292,
    TRUE = 293,
    FALSE = 294,
    ELSE = 295,
    COMMA = 296,
    MAIN = 297,
    RETURN = 298,
    INT_ID = 299,
    BOOLEAN_ID = 300,
    STRING_ID = 301,
    UMINUS = 302
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 16 "minijava.y" /* yacc.c:1909  */

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
    A_exp_list exp_list;

#line 130 "minijava.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_MINIJAVA_TAB_H_INCLUDED  */
