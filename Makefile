CC=gcc
CFLAGS=-D YYDEBUG

all: check parse
	@echo done

check: sym.o absyn.o check.o error.o minijava.tab.o lex.yy.o util.o
	gcc $^ -o $@

parse: sym.o absyn.o parse.o error.o minijava.tab.o lex.yy.o util.o
	gcc $^ -o $@

lex.yy.c: minijava.lex | 
	lex $^
	
lex.yy.o: lex.yy.c
	gcc $(CFLAGS) -c $^ -o $@

minijava.tab.o: minijava.tab.c
	gcc $(CFLAGS) -c $^ -o $@

minijava.tab.c: minijava.y
	bison -d -x $^
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl minijava.xml > debug.html

clean:
	rm -rf *.o a.out y.tab.[ch] lex.yy.c minijava.xml minijava.tab.[ch] debug.html 
