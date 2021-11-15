CC = gcc
CFLAGS=-Wall -Werror -std=c99

all: bfs


bfs: bfs.c
	$(CC) -o bfs.exe bfs.c

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c

queue.o: queue.h queue.c
	$(CC) $(CFLAGS) -c queue.c

bfs_queue.o: bfs_queue.c
	$(CC) $(CFLAGS) -c bfs_queue.c

bfs_queue: graph.o queue.o bfs_queue.o
	$(CC) $(CFLAGS)  graph.o queue.o bfs_queue.o -o bfs_queue.exe

dfs_recursive.o: dfs_recursive.c
	$(CC) $(CFLAGS) -c dfs_recursive.c

dfs_recursive: graph.o dfs_recursive.o
	$(CC) $(CFLAGS)  graph.o dfs_recursive.o  -o dfs_recursive.exe

clean:
	rm *.exe *.o