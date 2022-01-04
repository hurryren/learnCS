#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 100
void error_handling(char *message);

int main(){
    int fd;
    unsigned char buf[BUF_SIZE];

    fd = open("data.txt",O_RDONLY);
    if(fd == -1){
        error_handling("open() error!");
    }
    printf("file open success, file descriptor is: %d\n", fd);

    if(read(fd,buf,sizeof(buf)) == -1){
        error_handling("read() error");
    }
    printf("file data: %s",buf);
    close(fd);
    return 0;
}

void error_handling(char *message){
    fprintf(stderr,"%s\n",message);
    exit(1);
}