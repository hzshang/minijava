#include "stand.h"
#include "util.h"
#include "error.h"
#include "state.h"

E_error err;
FILE* copy;
int err_count;
static string str_dup_line(string p){
    string it = p;
    while(*it && *it!='\n')
        it++;
    int len = it-p;
    string r = safe_malloc(len+1);
    memcpy(r,p,len);
    r[len] = '\x00';
    return r;
}

void record_error(int pos,int len,E_type type){
    size_t alloc_size;
    string line = NULL;
    E_pos_locate(err->pos,pos);
    fseek(copy,lines[err->pos->row],SEEK_SET);
    getline(&line,&alloc_size,copy);
    int length = strlen(line);
    if(line[length-1]=='\n')
        line[length-1]='\x00';
    err->line = line;
    // given char
    fseek(copy,pos,SEEK_SET);
    err->given = String_init_len(len);
    err->given[fread(err->given,len,sizeof(char),copy)] = '\x00';
    err->kind = type;
}

void show_error(char const * msg,...){
    va_list ap;
    string err_type;
    switch (err->kind){
        case E_lexicon:
            err_type = "Lexical error";
            break;
        case E_syntax:
            err_type = "Syntax error";
            break;
        case E_semantics:
            err_type = "Semantic error";
            break;
        case E_unknown:
            err_type = "Unknown error";
            break;
        default:
            assert(false);
            break;
    }
    ERR("%s: %d:%d: ",err_type,err->pos->row,err->pos->column);
    va_start(ap,msg);
    vfprintf(stderr,msg,ap);
    va_end(ap);
    ERR("\n> %s\n",err->line);
    // \t == 4 char
    int len = err->pos->column-1;
    int msg_len = 2;
    for(int i=0;i<len;i++){
        if(err->line[i] == '\t')
            msg_len+=4;
        else
            msg_len++;
    }
    ERR("%*c%s\n",msg_len,' ',GREEN("^"));
    err_count++;
}

E_error E_error_init(){
    E_error e = safe_malloc(sizeof(*e));
    memset(e,0,sizeof(*e));
    e->kind = E_none;
    e->pos = E_pos_init();
    return e;
}

E_pos E_pos_init(){
    E_pos p = safe_malloc(sizeof(*p));
    memset(p,0,sizeof(*p));
    return p;
}

void E_pos_locate(E_pos e,int len){
    int num = line_num;
    while(lines[num] > len)
        num--;
    e->row = num;
    e->column = len - lines[num]+1;
}

