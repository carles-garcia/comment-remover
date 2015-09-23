
CC = gcc
CFLAGS = -O2 -Wall -std=c99

all: rcom

rcom: main.o rcom.o parsing.o
	$(CC) $(CFLAGS) -o rcom main.o rcom.o parsing.o
	
main.o: main.c 
	$(CC) $(CFLAGS) -c main.c
	
rcom.o: rcom.c
	$(CC) $(CFLAGS) -c rcom.c

parsing.o: parsing.c
	$(CC) $(CFLAGS) -c parsing.c
	
clean: 
	rm rcom main.o rcom.o parsing.o