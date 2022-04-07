#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>


int gval = 10;

int main(int argc, char *argv[]) {
    pid_t pid;
    int lval = 20;
    gval++;
    lval += 5;

    pid = fork();
    if (pid == 0) {
        gval += 2;
        lval += 2;
    } else {
        gval -= 2;
        lval -= 2;
    }

    if(pid == 0){
        printf("Child proc: [%d, %d] \n", gval, lval);
        exit(0);
    }
    else{
        printf("parent proc: [%d, %d] \n", gval, lval);
        wait((int*)0);
    }

    return 0;
}