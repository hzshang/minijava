#ifndef ERROR_H
#define ERROR_H
#define E_NO_SEMICOLON "Expect a ';'"
#define E_NO_RETURN "Expect a return at method end"
#define E_NO_MATCH "Expect a %s, given '%s'"

typedef enum {E_lexicon,E_syntax,E_semantics,E_unknown,E_none} E_type;

typedef struct E_error_* E_error;
typedef struct E_pos_* E_pos;
struct E_pos_ {
    int row;
    int column;
};

struct E_error_ {
    E_pos pos;
    string line;
    string given;
    E_type kind;
    union{
        struct{} stm;
        struct{} exp;
    } u;
};

void record_error(int,int,E_type);
void show_error(char const *,...);

E_error E_error_init();
E_pos E_pos_init();
void E_pos_locate(E_pos,int);

extern E_error err;
extern FILE* copy;
extern int err_count;
#endif /* !ERROR_H */
