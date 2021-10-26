/*
    The parent should send a byte to the child,
    the child should print "pid: received ping",
    where pid is its proces ID,
    write the byte on the pipe to the parent,and exit;
    the parent should read the byte from the child,
    print "pid: receved pong", and exit
*/
/*
1.  use pipe to create pipe
2.  use fork to create a child
3.  use read to find the process ID of the calling process
4.  use getpid to find the process ID of the calling process

*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int pid,status;
    int fds[2];
    int buf_pid,pid_num;
    pipe(fds);

    pid = fork();
    if(pid == 0){
        read(fds[0],&buf_pid,4);
        close(fds[0]);
        printf("%d: received ping\n",buf_pid);

        pid_num = getpid();
        write(fds[1],&pid_num,4);
        close(fds[1]);
    }else{
        pid_num = getpid();
        printf("pid_num %d\n",pid_num);
        write(fds[1],&pid_num,4);
        close(fds[1]);

        // sleep(10);
        wait(&status);

        read(fds[0],&buf_pid,4);
        close(fds[0]);
        printf("%d: received pong\n",buf_pid);
    }

  exit(0);
}
