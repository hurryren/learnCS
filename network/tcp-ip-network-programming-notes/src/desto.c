#include <stdio.h>

#include <fcntl.h>

int main() {
    FILE *fp;

    int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1) {
        printf("file poen error\n");
        return -1;
    }

    fp = fdopen(fd, "w");
    fputs("network c programming \n", fp);
    fclose(fp);
    return 0;
}