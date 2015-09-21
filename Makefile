
CC = gcc
CFLAGS = -O2 -Wall -std=c99

all: rcom

rcom: main.o remover.o
	$(CC) $(CFLAGS) -o rcom main.o remover.o
	
main.o: main.c 
	$(CC) $(CFLAGS) -c main.c
	
remover.o: remover.c
	$(CC) $(CFLAGS) -c remover.c
	
clean: 
	rm rcom main.o remover.o