all: httpd client

httpd: httpd.c
	gcc -g -W -Wall  -o httpd.exe httpd.c  -lpthread

client: simpleclient.c
	gcc -W -Wall -o clent.exe  simpleclient.c
clean:
	rm *.exe
