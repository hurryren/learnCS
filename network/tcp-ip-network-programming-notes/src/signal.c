#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/*
 * 调用函数的主体确实时操作系统，但金册灰姑娘处于睡眠状态时
 * 无法调用函数。因此，产生信号时，为了调用信号处理器，将唤
 * 醒由于调用sleep函数而进入阻塞状态的进程。而且，进程一旦
 * 被话逆行，就不会在进入睡眠状态。即使还未到sleep函数中规
 * 定的时间也是如此。
 */

void timeout(int sig)
{
    if(sig == SIGALRM)
        printf("Time out\n");
    alarm(2);
}

void keycontrol(int sig)
{
    if(sig == SIGINT)
        printf("CTRL+C pressed\n");
}

int main(int argc, char *argv[])
{
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);

    for(i=0;i<3;i++){
        printf("wait...\n");
        sleep(100);
    }
    return 0;
}