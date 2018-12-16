CC=gcc
CFLAGS=-D YYDEBUG

all: check parse debug.html
	@echo done
debug.html: minijava.y
	bison -x minijava.y
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl minijava.xml > $@

check: sym.o absyn.o check.o error.o y.tab.o lex.yy.o util.o
	gcc $^ -o $@

parse: sym.o absyn.o parse.o error.o y.tab.o lex.yy.o util.o
	gcc $^ -o $@

lex.yy.c: minijava.lex
	lex $^
	
lex.yy.o: lex.yy.c
	gcc $(CFLAGS) -c $^ -o $@

y.tab.o: y.tab.c
	gcc $(CFLAGS) -c $^ -o $@

y.tab.c: minijava.y
	yacc -dv $^

clean:
	rm -rf *.o a.out y.tab.[ch] lex.yy.c minijava.xml minijava.tab.c
