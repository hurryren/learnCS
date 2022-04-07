#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();

    if(pid == 0)
        printf("Hi, i am a child process");
    else{
        printf("child process ID: %d\n", pid);
        sleep(30);
    }

    if(pid == 0)
        printf("end child process");
    else
        printf("end parent process");

    return 0;

}