#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char *message);

int main()
{
    int sock;
    int send_buf;
    int recv_buf;
    int state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = state = sizeof(send_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&send_buf, &len);
    if(state)
        error_handling("getsockopt() error");

    len = sizeof(recv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&recv_buf, &len);
    if(state)
        error_handling("getsockopt() error");

    printf("Input buffer sizeo: %d \n", recv_buf);
    printf("Output buffer sizeo: %d \n", send_buf);
    return 0;
}


void error_handling(char *message)
{
    fprintf(stderr,"%s\n", message);
    exit(1);
}