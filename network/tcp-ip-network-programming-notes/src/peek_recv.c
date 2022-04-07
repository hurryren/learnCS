#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *buf);

int main(int argc, char *argv[]){
    int acpt_sock;
    int recv_sock;
    struct sockaddr_in acpt_adr;
    struct sockaddr_in recv_adr;
    int str_len;
    int state;
    socklen_t recv_adr_sz;
    char buf[BUF_SIZE];
    if(argc!=2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&acpt_adr, 0, sizeof(acpt_adr));
    acpt_adr.sin_family = AF_INET;
    acpt_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_adr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock, (struct sockaddr*)&acpt_adr,sizeof(acpt_adr)) == -1)
        error_handling("bind() error");
    listen(acpt_sock, 5);

    recv_adr_sz = sizeof(recv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&recv_adr, &recv_adr_sz);

    while(1){
        /*
         * 调用recv函数的同时传递MSG_PEEK可选项，是为了保证，即使不存在待读取数据也不进入阻塞状态。
         */
        str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_PEEK|MSG_DONTWAIT);
        if(str_len > 0)
            break;
    }

    buf[str_len] = 0;
    printf("Buffering %d bytes: %s \n", str_len, buf);
    /*
     * 再次调用recv函数，并未设置任何可选项，因此，本次读取的数将从输入缓冲中删除。
     */
    str_len = recv(recv_sock, buf, sizeof(buf)-1,0);
    buf[str_len] = 0;
    printf("read again: %s \n", buf);
    close(acpt_sock);
    close(recv_sock);
    return 0;
}

void error_handling(char *buf) {
    fprintf(stderr, "%s\n", buf);
    exit(1);
}
