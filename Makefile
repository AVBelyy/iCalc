COMPILER=gcc

all: icalc

icalc: lexer.o icalc.o
	$(COMPILER) -O2 lexer.o icalc.o -lm -o icalc

lexer.o: lexer.c
	$(COMPILER) -c lexer.c

icalc.o: icalc.c
	$(COMPILER) -c icalc.c

clean:
	rm lexer.o icalc.o icalc
