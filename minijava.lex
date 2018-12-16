%{
#include "stand.h"
#include "util.h"
//#include "tokens.h"
#include "error.h"
#include "absyn.h"
#include "y.tab.h"
int state_pos = 1;

int yywrap(void){
    state_pos = 1;
    return 1;
}

#define ADJ() do{\
    token_pos = state_pos;\
    state_pos += yyleng;\
} while (0)
#define MAX_LENGTH 2048
int com_depth;
char string_buf [MAX_LENGTH];
char *string_ptr;

#define ADJ_STR() do{\
    state_pos += yyleng;\
} while(0)
%}
%x string comment1 comment2
%%
<string>{

  \"  {
    ADJ_STR();
    *string_ptr='\0';
    if (string_buf[0] != '\0')
      yylval.sval=String(string_buf);
    else
      yylval.sval=String("[EMPTY]");
    BEGIN(INITIAL);
    return STRING;
  }

  \\[0-9]{3} {
    ADJ_STR();
    int result = atoi(yytext + 1);
    if (result > 0xff) {
      state_error(token_pos, "illegal character");
      continue;
    }
    *string_ptr++ = result;
  }

  \\n     {ADJ_STR(); *string_ptr++ = '\n';}
  \\t     {ADJ_STR(); *string_ptr++ = '\t';}
  \\\"    {ADJ_STR(); *string_ptr++ = '\"';}
  \\\\    {ADJ_STR(); *string_ptr++ = '\\';}
  \\\^[\0-\037]   {
    ADJ_STR();
    *string_ptr++ = yytext[2];
  }

  \\[ \t\n\r]+\\ {
    ADJ_STR();
    char *yytextptr = yytext;
    while (*yytextptr != '\0')
    {
      if (*yytextptr == '\n')
        state_newline();
      ++yytextptr;
    }
  }

  \\. {ADJ_STR(); state_error(token_pos, "illegal escape char");}

  \n  {
    ADJ_STR();
    state_newline();
    state_error(token_pos, "string terminated with newline");
    continue;
  }

  [^\\\n\"]+        {
    ADJ_STR();
    char *yptr = yytext;

    while (*yptr)
      *string_ptr++ = *yptr++;
  }
}

<*>"/*" {
    ADJ();
    com_depth++;
    BEGIN(comment1);
}
<comment1>{
    \n  {
        ADJ();
        state_newline();
    }
    "*/"  {
        ADJ();
        --com_depth;
        if(!com_depth) BEGIN(INITIAL);
    }
    .  {
        ADJ();
    }
}
<*>"//" {
    ADJ();
    BEGIN(comment2);
}
<comment2>{
    \n {
        ADJ();
        BEGIN(INITIAL);
    }
    . {
        ADJ();
    }
}
<INITIAL>{
    (" "|"\t")  {ADJ();} 
    "\n"	  {ADJ(); state_newline();}
    "," {ADJ();return COMMA;}
    ";" {ADJ();return SEMICOLON;}
    "(" {ADJ();return LPAREN;}
    ")" {ADJ();return RPAREN;}
    "{" {ADJ();return LBRACE;}
    "}" {ADJ();return RBRACE;}
    "[" {ADJ();return LBRACK;}
    "]" {ADJ();return RBRACK;}
    "=" {ADJ();return ASSIGN;}
    "!" {ADJ();return REVERSE;}
    "<" {ADJ();return LT;}
    ">" {ADJ();return GT;}
    "&&" {ADJ();return AND;}
    "+" {ADJ();return PLUS;}
    "-" {ADJ();return MINUS;}
    "*" {ADJ();return TIMES;}
    "." {ADJ();return DOT;}
    class {ADJ();return CLASS;}
    void {ADJ(); return VOID;}
    public {ADJ();return PUBLIC;}
    static {ADJ();return STATIC;}
    main {ADJ();return MAIN;}
    extends {ADJ();return EXTENDS;}
    return {ADJ();return RETURN;}
    int {ADJ(); return INT_ID;}
    boolean {ADJ();return BOOLEAN_ID;}
    String {ADJ();return STRING_ID;}
    if {ADJ();return IF;}
    else {ADJ();return ELSE;}
    while {ADJ();return WHILE;}
    "System.out.println" {ADJ();return PRINT;}
    length {ADJ();return LENGTH;}
    true {ADJ();return TRUE;}
    false {ADJ();return FALSE;}
    new {ADJ();return NEW;}
    this {ADJ();return THIS;}
    
    [0-9]+	 {ADJ(); yylval.ival=atoi(yytext); return INT;}
    [a-zA-Z][a-zA-Z0-9_]* {ADJ(); yylval.sval=String(yytext); return ID;}
    
    \"   {
        ADJ();
        string_ptr = string_buf;
        BEGIN(string);
    }
}

. {
    ADJ();
    state_error(token_pos,"illegal token");
}

