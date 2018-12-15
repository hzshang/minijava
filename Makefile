CC=gcc

all: sym.o absyn.o check.o error.o lex.yy.o y.tab.o util.o
	gcc $^ 

lex.yy.c: minijava.lex
	lex $^
	
lex.yy.o: lex.yy.c
	gcc -c $^ -o $@

y.tab.o: y.tab.c
	gcc -c $^ -o $@

y.tab.c: minijava.y
	yacc -dv $^

clean:
	rm -rf *.o a.out
