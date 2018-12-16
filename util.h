#ifndef UTIL_H
#define UTIL_H
#ifndef NULL
#define NULL ((void*)0)
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

typedef char* string;
typedef unsigned char bool;

void* safe_malloc(int length);
string String(char *s);


#endif /* !UTIL_H */
