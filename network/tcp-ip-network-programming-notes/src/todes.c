#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(){
    FILE *fp;
    int fd= open("data.dat",O_WRONLY|O_CREAT|O_TRUNC);
    if(fd == -1){
        printf("file open error\n");
        exit(1);
    }

    printf("First file descriptor: %d\n",fd);
    fp = fdopen(fd,"w");
    fputs("TCP/IP socket programming\n",fp);
    printf("second file descriptor: %d\n",fileno(fp));
    fclose(fp);
    return 0;
}