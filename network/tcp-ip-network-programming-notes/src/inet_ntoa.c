#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>



/*
 * inet_ntoa 转换的结果存储在一个不知道的位置，并且在调用后没有被释放。
 * 下一次调用的结果还是存储到这个地址。
 * 我不能理解？内部 malloc,进程结束的时候才 free??????
 */
int main(int argc, char *argv[])
{
    struct sockaddr_in addr1, addr2;
    char *str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr=htonl(0x10203040);
    addr2.sin_addr.s_addr=htonl(0x10101010);

    str_ptr = inet_ntoa(addr1.sin_addr);
    strcpy(str_arr, str_ptr);
    printf("Dotted-Decimal notation1: %s \n", str_ptr);

    inet_ntoa(addr2.sin_addr);
    printf("Dotted-Decimal notation2: %s \n", str_ptr);
    printf("Dotted-Decimal notation3: %s \n", str_arr);

    return 0;
}