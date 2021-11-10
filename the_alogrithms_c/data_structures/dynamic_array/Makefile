CC = gcc
CFLAGS = -g -Wall

default: main
	./main

main: main.o dynamic_array.o
	$(CC) $(CFLAGS) $^ -o $@

dynamic_array:  dynamic_array.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm *.o main