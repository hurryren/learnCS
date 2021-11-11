CC = gcc
CFLAGS = -g -Wall

avl: avl_tree.c
	$(CC) $(CFLAGS) $^ -o avl.exe
	./avl.exe

bst: binary_search_tree.c
	$(CC) $(CFLAGS) $^ -o bst.exe
	./bst.exe

clean:
	rm *.exe