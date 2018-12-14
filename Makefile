

lex.yy.c: minijava.lex
	lex $^
lex.yy.o: lex.yy.c
	gcc -c $^ -o $@
