CC = gcc
CFLAGS = -g -Wall

default: dict.o test_dict.c
	$(CC) $(CFLAGS) $^ -o dict_test.exe
	./dict_test.exe

dict: dict.c
	$(CC) $(CFLAGS) $^ $@


clean:
	rm *.exe *.o