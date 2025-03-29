.PHONY: all clean

CXX = g++
FLEX = flex

all: lexer

lexer: lex.yy.c
	$(CXX) -o $@ $<

lex.yy.c: lexer.l
	$(FLEX) -o $@ $<

clean:
	rm -f lexer lex.yy.c