CC=gcc

all: sym.o absyn.o check.o error.o y.tab.o lex.yy.o util.o
	gcc $^ 

parse: sym.o absyn.o parse.o error.o y.tab.o lex.yy.o util.o
	gcc $^ -o $@

lex.yy.c: minijava.lex
	lex $^
	
lex.yy.o: lex.yy.c
	gcc -c $^ -o $@

y.tab.o: y.tab.c
	gcc -c $^ -o $@

y.tab.c: minijava.y
	yacc -dv $^

clean:
	rm -rf *.o a.out y.tab.[ch] lex.yy.c
