#ifndef UTIL_H
#define UTIL_H
#ifndef NULL
#define NULL 0
#endif
typedef char* string;
typedef unsigned char bool;

void* safe_malloc(int length);
string String(char *s);


#endif /* !UTIL_H */
