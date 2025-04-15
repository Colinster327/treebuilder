.PHONY: all clean

CXX = g++
FLEX = flex
BISON = bison

all: tree_builder

tree_builder: lex.yy.c parser.tab.c parse_tree.h tree_node.h
	$(CXX) -o $@ lex.yy.c parser.tab.c

lex.yy.c: tree_builder.l
	$(FLEX) -o $@ $<

parser.tab.c: tree_builder.y
	$(BISON) -d -o $@ $<

clean:
	rm -f lexer lex.yy.c
