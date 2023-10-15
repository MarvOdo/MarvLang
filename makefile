# main makefile for MarvLang compiler
# Currently only makes lexer

CC = gcc
CFLAGS = -std=c17 -Wall -Werror -pedantic -g

.PHONY : clean

lexer : lexer.o
	${CC} ${CFLAGS} -o $@ $^

lexer.o : lexer.c lexer.h
	${CC} ${CFLAGS} -c $^

clean :
	rm -rf *.o
	rm -rf *.gch
	rm -rf lexer