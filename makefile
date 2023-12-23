# main makefile for MarvLang compiler
# Currently only makes lexer

CC = gcc
CFLAGS = -std=c17 -Wall -Werror -pedantic -g

.PHONY : clean

test : test.o lexer.o
	${CC} ${CFLAGS} -o $@ $^

test.o : test.c lexer.h
	${CC} ${CFLAGS} -c $^

lexer.o : lexer.c lexer.h
	${CC} ${CFLAGS} -c $^

clean :
	rm -rf *.o
	rm -rf *.gch
	rm -rf test