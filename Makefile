CC=gcc
CFLAGS=-D YYDEBUG -g -Wno-varargs

OBJ=minijava.tab.o sym.o absyn.o error.o lex.yy.o util.o parse_tree.o
all: check parse parse_out
	@echo done

check: $(OBJ) check.o
	gcc $^ -o $@

parse: $(OBJ) parse.o
	gcc $^ -o $@

parse_out: $(OBJ) parse_out.o
	gcc $^ -o $@

lex.yy.c: minijava.lex
	lex $^
	
lex.yy.o: lex.yy.c
	gcc $(CFLAGS) -c $^ -o $@

minijava.tab.o: minijava.tab.c
	gcc $(CFLAGS) -c $^ -o $@

minijava.tab.c: minijava.y
	bison -d -x $^
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl minijava.xml > debug.html

clean:
	rm -rf *.o a.out y.tab.[ch] lex.yy.c minijava.xml minijava.tab.[ch] debug.html \
		parse parse_out check
