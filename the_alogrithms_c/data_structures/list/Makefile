CC = gcc
CFLAGS = -g -c -Wall

default: main
	./main

main: main.o list.o
	$(CC) -g main.o list.o -o main

list.o: list.c
	$(CC) $(CFLAGS) list.c

clean:
	rm *.o main