all: icalc

icalc: lexer.o icalc.o
	gcc -O2 lexer.o icalc.o -lm -o icalc

lexer.o: lexer.c
	gcc -c lexer.c

icalc.o: icalc.c
	gcc -c icalc.c

clean:
	rm lexer.o icalc.o icalc
