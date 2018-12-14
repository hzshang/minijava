#include "stand.h"
#include "util.h"

void *safe_malloc(int length){
    void *p = malloc(length);
    assert(p!=NULL);
    return p;
}

string String(char* p){
    string r = safe_malloc(strlen(p)+1);
    strcpy(r,p);
    return r;
}
