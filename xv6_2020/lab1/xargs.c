
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    //   char *argv[] = { "echo", "this", "is", "echo", 0 };
    int num = argc-1,j =0,i=0;
    char buf[10][20];
    char temp;
    for(i = 1;i<argc;i++){
        memcpy(buf[i-1],argv[i],strlen(argv[i]));
    }

    // for(j=0;j<num;j++){

    //     printf("com %s\n", buf[j]);
    // }

    j=0;
    while (read(0, &temp, 1) > 0)
    {
        if(temp == '\n'){
            buf[num++][j] = 0;
            j=0;
        }else{
            buf[num][j++] = temp;
        }
        // buf[i++] = temp;
    }

    // for(j=0;j<num;j++){

    //     printf("data  %s\n", buf[j]);
    // }

    char *com[] = { "echo", "this", "is", "echo", 0 };

    for(j=0;j<num;j++){
        com[j] = buf[j];
    }
    com[j] =0;


    int pid;


    pid = fork();
    if(pid == 0){
        exec(com[0], com);
    }else{
        wait(0);
    }

    exit(0);
}