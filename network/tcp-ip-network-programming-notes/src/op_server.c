#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);
enum cal_op{
    PLUS,MINUS,MULTIPLE
};
struct cal_info {
    int count;
    int num[10];
    enum cal_op operator;
};


int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;
    struct cal_info message;
    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t  clnt_addr_size;
    int str_len;
    int result;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 1)==-1)
        error_handling("listen() error");

    clnt_addr_size = sizeof(clnt_adr);

    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_addr_size);
    if(clnt_sock == -1)
        error_handling("accept() error");
    else
        printf("Client connected\n");
    
    while(str_len=read(clnt_sock, &message, sizeof(message)) != 0){
        switch (message.operator) {
            case PLUS:
                result = message.num[0];
                for(int i=1; i < message.count; i++){
                    result += message.num[i];
                }
                break;
            case MINUS:
                result = message.num[0];
                for(int i=1; i < message.count; i++){
                    result -= message.num[i];
                }
                break;
            case MULTIPLE:
                result = message.num[0];
                for(int i=1; i < message.count; i++){
                    result *= message.num[i];
                }
                break;
        };
        write(clnt_sock, &result, sizeof(result));
    }

    close(clnt_sock);
    close(serv_sock);
    return 0;
}




void error_handling(char *message)
{
    fprintf(stderr,"%s\n", message);
    exit(1);
}