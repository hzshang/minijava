#ifndef ERROR_H
#define ERROR_H
void state_error(int,char*,...);
void state_newline();
void state_reset(char*);
extern int token_pos;

#endif /* !ERROR_H */
