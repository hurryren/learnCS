/*
    the first process feeds the numbers 2 through 35 into
    the pipeline. for each prime number, you will arrange to create
    one process that reads from its left neighbor over a pipe and writes to its
    right neighbor over another pipe.
    the first process can stop at 35
*/
/*
1.  be carefull to close file descriptors that a process doesn't need
2.  one the first process reached 35, it should wait until the entire
    pipeline terminates, including all children, grandchildren. The main primes
    should only exit after all the output has been printed, and after all the oter primes
    processes hace exited
3.  read returns zero when the write-side of a pipe is closed
4.  use getpid to find the process ID of the calling process

*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void primes_func(int *p_fd){
    int pid;
    int c_fd[2];
    int p,n;
    int has_child=0;

    pid = fork();
    if(pid == 0){
        close(p_fd[1]);
        p = 0;
        read(p_fd[0],&p,4);
        printf("prime %d\n",p);

        if(p == 35){
            close(p_fd[0]);
            exit(0);
        }

        while(read(p_fd[0],&n,4)){
            if((n % p) != 0){
                if(has_child){
                    write(c_fd[1],&n,4);
                }else{
                    pipe(c_fd);
                    write(c_fd[1],&n,4);
                    has_child = 1;
                    primes_func(c_fd);
                    close(c_fd[0]);
                }
            }
        }
        close(c_fd[1]);
        // printf("%d waiting \n",p);
        wait(0);
    }
}


int
main(int argc, char *argv[])
{
    int c_fd[2];
    int num=2;

    pipe(c_fd);
    while(num < 36){
        write(c_fd[1],&num,4);
        num++;
    }
    primes_func(c_fd);
    close(c_fd[1]);


    wait(0);
    exit(0);



}


