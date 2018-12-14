#include "stand.h"
#include "util.h"
#include "error.h"

#define MAX_LINE 4096
int line_num;
int lines[MAX_LINE];
int token_pos;
bool error;
static string filename;
extern FILE* yyin;
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
void state_reset(string file){
    error = 0;
    filename = file;
    line_num = 1;
    yyin = fopen(filename,"r");
    if(!yyin){
        fprintf(stderr,"open %s failed\n",filename);
        exit(1);
    }
}
