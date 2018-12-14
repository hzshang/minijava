CC=gcc

all: check.o error.o lex.yy.o util.o
	gcc $^ 
lex.yy.c: minijava.lex
	lex $^
lex.yy.o: lex.yy.c
	gcc -c $^ -o $@

clean:
	rm -rf *.o a.out
