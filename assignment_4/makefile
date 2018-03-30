#
# "makefile" for the final implementation of KWIC (kwic4.c)
# Spring 2018, assignment #4)
#

CC=gcc
CFLAGS=-c -Wall -g

kwic4: kwic4.o linkedlist.o
	$(CC) kwic4.o linkedlist.o -o kwic4

kwic4.o: kwic4.c linkedlist.h
	$(CC) $(CFLAGS) kwic4.c

kwic4test: kwic4test.o linkedlist.o
	$(CC) kwic4test.o linkedlist.o -o kwic4test

kwic4test.o: kwic4test.c linkedlist.h
	$(CC) $(CFLAGS) kwic4test.c

linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) linkedlist.c	

all: kwic4 kwic4test 
	
clean:
	rm -rf *.o kwic4 kwic4test
