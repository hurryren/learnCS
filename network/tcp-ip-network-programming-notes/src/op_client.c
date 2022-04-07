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
    int sock;
    struct cal_info message;
    struct sockaddr_in serv_adr;
    int result;
    char input[10];


    if(argc != 3){
        printf("Usage: %s <IP> <port>\n",argv[0]);
        exit(1);
    }
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        printf("connected ......\n");

    while(1){
        memset(input, 0, sizeof(input));
        printf("operand count: ");
        fgets(input, 10, stdin);
        message.count = atoi(input);

        if(message.count > 9)
            goto op_count_error;

        for(int i=0; i<message.count; i++){
            memset(input, 0, sizeof(input));
            printf("operand [%d]: ",i);
            fgets(input, 10, stdin);
            message.num[i] = atoi(input);
        }
        memset(input, 0, sizeof(input));
        printf("operator : ");
        fgets(input,10,stdin);
        if(input[0] == '+')
            message.operator = PLUS;
        else if(input[0] == '-')
            message.operator = MINUS;
        else if(input[0] == '*')
            message.operator = MULTIPLE;
        else{
            printf("operator error\n");
            goto op_count_error;
        }

        write(sock, &message, sizeof(message));

        read(sock, &result, sizeof(result));
        printf("operation result: %d\n", result);
//        break;
    }

op_count_error:
    close(sock);
    return 0;
}


void error_handling(char *message)
{
    fprintf(stderr,"%s\n", message);
    exit(1);
}