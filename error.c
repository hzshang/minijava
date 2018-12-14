#include "stdand.h"
#include "error.h"

#define MAX_LINE 4096
int line_num = 1;
int lines[MAX_LINE];
int token_pos = 1;
void state_error(int pos, char* msg,...){
    va_list ap;
    int num = line_num;
    while(lines[num] > pos)
        num--;
    fprintf(stderr,"%d:%d: ",num,pos-lines[num]);
    va_start(ap,msg);
    vfprintf(stderr,msg,ap);
    va_end(ap);
}
void state_newline(){
    line_num ++;
    lines[line_num] = token_pos;
}

