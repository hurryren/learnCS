# 第一章







## 第二章 套接字类型与协议设置

### 2.1 套接字协议及其数据传输特性

协议就是为了完成数据交换而定好的约定。



```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

-   domain：套接字中使用的协议族信息
-   type：套接字数据传输类型信息
-   protocol：计算机通信中使用的协议信息



#### domain

| 名称      | 协议族               |
| --------- | -------------------- |
| PF_INET   | IPV4                 |
| PF_INET6  | IPV6                 |
| PF_LOCAL  | 本地通信的UNIX协议族 |
| PF_PACKET | 底层套接字的协议族   |
| PF_IPX    | IPX Novell协议族     |

#### type

-   SOCKET_STREAM：面向连接
-   SOCKET_DGRAM：面向消息

#### protocol

大部分情况下根据 domain 和 type 可以决定采用的协议，可以向第三个参数传递0，除非遇到同一协议族中存在多个数据传输方式相同的协议





## 第三章



```c
struct sockaddr_in
{
    sa_family_t		sin_family;  /* 地址族 （address family） */
    uint16_t 		sin_port;	/* 16 位 TCP/UDP 端口号 */
    struct in_addr 	sin_addr;  	/* 32 位 IP地址 */
    char 			sin_sero[8]; /* 不使用 */
};


struct in_addr 
{
    In_addr_t	s_addr;  /* 32 位 IPV4 地址 */
}
```



注：sockaddr_in 并非只为 IPV4 设计。



-   大端序：高位字节存放到低位地址。
-   小端序：高位字节存放到高位地址

网络字节序为大端序。



```c
#include <arpa/inet.h>

/* 成功返回 32 位大端序整数型值，失败返回 INADDR_NONE */
in_addr_t inet_addr(const char *string); 
```





```c
#include <arpa/inet.h>

int inet_aton(const char *string, struct in_addr *addr);
```





```c
#include <arpa/inet.h>

/* 将网络字节序整数型 IP 地址转换成字符串形式 */
char * inet_ntoa(struct in_addr adr);
```



#### INADDR_ANY

利用常数 INADDR_ANY 分配服务器端的 IP 的地址。 采用这种方式可以自动获取运行服务器端的计算机 IP 地址。而且如果统一计算机中已分配多个 IP 地址，则只要端口号一致，就可以从不同 IP 地址接收数据。





## 第五章



### 5.2

-   IO缓冲在每个TCP连接中单独存在
-   IO缓冲在创建套接字时自动生成
-   即使关闭套接字也会继续传递输出缓冲中遗留的数据
-   关闭套接字将会丢失输入缓冲中的数据



## 第六章

### 6.1

TCP比UDP慢的原因通常有两点：

-   收发数据前后进行连接设置以及清除过程。
-   收发数据过程中为保证可靠性而添加的流控制。



### 6.3 

#### UDP中的服务器端和客户端没有连接

#### UDP服务器端和客户端均只需要一个套接字

TCP中，套接字之间应该是一对一的关系。如果需要向10个客户端提供服务，则除了守门的服务器套接字外，还需要10个服务器端套接字。但在UDP中，不管是服务器端还是客户端都只需要一个套接字。



#### 给予UDP的数据IO函数

UDP套接字不会保持连接状态，因此妹每次传输数据都要添加目标地址信息。

```c
#include <sys/socket.h>

ssize_t sendto(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *to, socklen_t addrlen);
```

-   sock：用于传输数据的UDP套接字文件描述符
-   buff：保存待传输数据的缓冲地址
-   nbytes：待传输的数据长度，以字节为单位
-   flags： 可选项参数，没有则传递 0
-   to：存有目标地址信息的 sockaddr 结构体变量的地址值。
-   addrlen：传递给参数 to 的地址值结构体变量长度。

上述函数与TCP输出函数的最大区别在于，此函数需要向它传递目标地址信息。



```c
#include <sys/socket.h>

ssize_t recvfrom(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen);
```



UDP 不同于TCP，不存在请求连接和受理过程，因此在某种意义上无法明确区分服务器端和客户端。只是因其提供服务而被成为服务端。

针对UDP套接字调用connect函数并不意味着要与对方UDP套接字连接，这只是向UDP套接字注册目标IP核端口信息。

之后就与TCP套接字一样，每次调用 sendto 函数时只需要传输数据。因为已经指定了收发对象，所以不仅可以使用sendto,recvfrom函数，还可以使用 write, read函数进行通信。



```c
#include <sys/socket.h>

int shutdown(int sock, int howto);
```

-   sock: 需要断开的套接字文件描述符
-   howto: 传递断开方式信息

调用上述函数时，第二个参数决定断开连接的方式：

-   SHUT_RD：断开输入流
-   SHUT_WR：断开输出流
-   SHUT_RDWR：同时断开 IO 流



利用一下函数可以通过传递字符串格式的域名获取IP地址。

```c
#include <netdb.h>

/* 成功时饭hi hostent 结构体指针，失败时返回 NULL 指针 */
struct hostent * gethostbyname(const char *hostname);

struct hostent
{
    char * h_name;   /* official name */
    char ** h_aliases;  /* alias list */
    int h_addrtype;  /* host address type */
    int h_length;  /* address length */
    char ** h_addr_list;  /* address list */
};
```



## 第九章

```c
#include <sys/socket.h>

int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen);
int setsockopt(int sock, int level, int optname, const void *optval, socklen_t optlen);
```



套接字在经过四次握手过程后并非立即消除，而是要经过一段时间的 Time-wait 状态。只有先断开（先发送FIN消息的）的主机才经过Time-wait过程。因此如果服务器先断开连接，则无法立即重新运行。套接字在处于Time-wait过程时，相应端口是正在使用的状态。

不管是服务器端还是客户端，套接字都有time-wait过程。先断开连接的套接字必然会经过Time-wait状态。但无需考虑客户端Time-wait状态。因为客户端套接字的端口号是任意指定的。

存在Time-wait的原因是为了确保发送的FIN信息成功到达客户端。

可以在套接字设置中更改 SO_REUSEADDR状态。适当调整该参数，可以将Time-wait状态下的套接字端口号重新分配给新的套接字。



#### Nagle 算法

为防止因数据包过多而发生网络过载，Nagle算法在1984年诞生了。他应用于TCP层。

只有收到前一数据的ACK消息时，Nagle算法才发送下一数据。



## 第十章



```c
#include <unistd.h>

pid_t fork(void);
```



进程完成工作后应该被销毁，但有时候这些进程会编程僵尸进程，占用系统中的重要资源。这种状态下的金册灰姑娘被称为僵尸进程。



```c
#include <sys/wait.h>

pid_t wait(int * statloc);
```



wait函数会引起程序阻塞，还可以考虑调用waitpid函数。这是防止僵尸进程的第二种方法，也是防止阻塞的方法。

```c
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *statloc, int options);
```



#### 信号与 signal 函数

-   进程：嘿，操作系统，如果我之前创建的子进程终止，就帮我调用 sombie_handler 函数。
-   操作系统：好的，如果你的子进程终止，我会帮你调用 zomebie_handler 函数，你先把该函数准备好。



```c
#include <signal.h>

void (*signal(int signo, void (*func)(int)))(int);

/*
 * signal(int signo, void(*func)(int))
 * signal 函数有两个参数，第一个是int，第二个是无返回值，带一个int参数的函数指针。
 * 
 * void (*signal(xxx)) (int)
 * signal 函数返回的是一个函数指针，无返回值，有一个 int 参数
 * 最后i就变成了
 * sigfunc *signal(int, sigfunc *);
 */
```

发生第一个参数代表的情况时，调用第二个参数所指的函数。

-   SIGALRM: 已到通过调用 alarm 函数注册的时间
-   SIGINT: 输入CTRL+C
-   SIGCHLD: 子进程终止



sigaction函数类似signal函数，可以代替signal函数，也更稳定。signal函数在UNIX系列的不同操作系统中可能存在去呗，但sigaction函数完全相同。

```c
#include <signal.h>


/*
 * signo: 与signal函数相同，传递信号信息
 * act: 对应于第一个参数的信号处理函数信息
 * oldact: 通过此参数获取之前注册的信号处理函数指针，若不需要则传递 0
 */
int sigaction(int signo, const struct sigaction *act, struct sigaction * oldact);

struct sigaction {
    void (*sa_handler)(int);
    sigset_t sa_mask;
    int sa_flags;
}
```

结构体 sigaction 的 sa_handler 成员保存信号处理函数的指针值（地址）。sa_mask和sa_flags的所有位均初始化为0即可。这两个成员用于指定信号相关的选项和特性，防止僵尸进程用不上。





## 第十章

进程间通信意味着两个不同进程间可以交换数据，为了完成这一点，操作系统提供两个进程可以同时访问的内存空间。



#### 通过管道实现进程间通信

```c
#include <unistd.h>

int pipe(int filedes[2]);
```

一写零读

向管道传递数据时，先读的进程会把数据取走

一般创建两个管道完成双向通信任务，各自负责不同的数据流动方向。



## 第十二章

#### 多进程服务器端的缺点和解决办法

为了构建并发服务器，只要有客户端连接请求就会创建新进程。这的确是实际操作中采用的一种方案，但并非十全十美，因为创建进程需要付出比较大的代价。每个进程都具有独立的内存空间，所以相互间的数据交换也要采用相对复杂的方法。

运用select函数是最具代表性的实现复用服务器端方法。

再 fd_set 变量中注册或更改值的操作都由下列宏完成：

-   FD_ZERO(fd_set *fdset) : 将 fd_set变量那个的所有位初始化为 0。
-   FD_SET(int fd, fd_set * fdset): 从参数 fdset 指向的变量中注册文件描述符 fd 的信息。
-   FD_CLR(int fd, fd_set * fdset): 从参数 fdset 指向的变量中清除文件描述符 fd 的信息。
-   FD_ISSET(int fd, fd_set *fdset):若参数fdset指向的变量中包含文件描述符的信息，则返回真。

```c
#include <sys/select.h>
#include <sys/time.h>

int select(int maxfd, fd_set *readset, fd_set *writeset, fd_set *exceptset, const struct timeval *timeout);
```

-   maxfd   监视对象文件描述符数量
-   readset 将所有关注  是否存在待读取的文件描述符注册到 fd_set 变量中，并传递其地址
-   writeset 将所有关注是否可传输无阻塞数据的文件描述符注册刀片fd_set型变量，并传递其地址值。
-   exceptset 将所有关注是否发生异常的文件描述符注册到 fd_set 型变量，并传递其地址值
-   timeout  调用select函数后，为防止陷入无限阻塞的状态，传递超时信息。
-   返回值  发生错误时返回 -1，超时返回时返回0。因为发生关注的事件返回时，返回大于零的值，该值是发生事件的文件描述符数。

select函数的超时时间与 select 函数的最后一个参数有关，其中timeval结构体定义如下：

```c
struct timeval {
    long tv_sec; /* seconds */
    long tv_usec;  /* microseconds */
}
```

select函数调用完成后，向其传递的 fd_set 变量中将发生变化。原来为1的所有位均变为0，但发生变化的文件描述符除外。因此可以任务值仍为1的位置上的文件描述符发生了变化。



## 第十三章

```c
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t nbytes, int flags);
ssize_t recv(int sockfd, void *buf, size_t nbytes, int flags);
```

flags:

-   MSG_OOB: 用于传输带外数据
-   MSG_PEEK: 验证输入缓冲中是否存在接收的数据
-   MSG_DONTROUTE: 数据传输过程中不参照路由表，再本地网络中寻找目的地
-   MSG_DONTWAIT: 调用IO函数时不阻塞，用于使用非阻塞IO
-   MSG_WAITALL: 防止函数返回，直到接收全部请求的字节数。



### 13.2 readv 和 writev 函数

通过writev函数可以将分散保存再多个缓冲区的数据一并发送，通过readv函数可以由多个缓冲分别接收。适当使用这两个函数可以减少IO函数的调用次数。

```c
#include <sys/uio.h>

ssize_t writev(int filedes, const struct iovec *lov, int iovcnt);

struct iovec
{
    void *iov_base; /* 缓冲地址 */
    size_t iov_len; /* 缓冲大小 */
}
```

-   filedes 表示数据传输对象的套接字文件描述符。但该函数并不只限于套接字，因此可以像read函数一样向其换地文件或标准输出描述符。
-   iov   iovec结构体数组的地址值，结构体iovec中包含待发送数据的位置和大小信息。
-   iovcnt 向第二个参数传递的数组长度



```c
#include <sys/uio.h>

ssize_t readv(int filedes, const struct iovec *iov, int iovcnt);
```



需要传输的数据分别位于不同的缓冲区时，可以减少系统调用次数/IO次数，从而提高程序运行效率



## 第十四章

多播方式的数据传输是基于UDP完成的。

采用多播方式时，可以同时向多个主机传递数据。



#### 多播的数据传输及流量方面的优点

-   多播服务器端支队特定多播组，只发送一次数据
-   即使只发送一次数据，但该组内的所有客户端都会接收数据。
-   多播组数可以再地址范围内任意增加
-   加入特定组即可接收发往该多播组的数据
-   加入特定组即可接收发往多播组的数据

多播组时D类IP地址。加入多播组可以理解为通过程序完成声明：再D类地址中，我希望接收发往目标xxx.xxx.xxx.xxx的多播数据。

多播是基于UDP完成的，也就是说，多播数据包的格式与UDP数据包相同。指示与一般的UDP数据包不同，向网络传递一个多播数据包时，路由器将复制该数据包并传递到多个主机。

多播主要用于多媒体数据的实时传输。

另外，虽然理论上可以完成多播通信，但不少路由器并不支持多播，或即便支持也因网络拥堵问题故意阻断多播。因此，为了再不支持多播的路由器中完成多播通信，也会使用隧道技术。

为了传递多播数据包，必须设置TTL。TTL值设置过大会影响网络流量，设置过小会导致无法传递到目标。



#### 广播

广播在一次性向多个主机发送数据这一点上与多播类似，但传输数据的范围有区别。多播即使在跨越不同网络的情况下，只要加入多播组就能接收数据。相反，广播只能向同一网络中的主机传输数据





## 第十五章

标准啊IO函数具有良好的移植性，并且可以利用缓冲提高性能。

标准IO函数不容易进行双向通信，有时候可能频繁调用fflush函数

需要以FILE结构体指针的形式返回文件描述符。



```c
#include <stdio.h>

/* 将文件描述符转换成 FILE结构体 */
FILE *fdopen(int fildes, const char *mode);
```





```c
#include <stdio.h>

/* 将 FILE结构体转换成文件描述符 */
int fileno(FILE *stream);
```



实际并不存在适用于所有情况的模型。应该理解各种你=模型的优缺点，并具备合理运用这些模型的能力。



## 第十六章 关于IO流分离的其他内容

通过分开输入过程和输出过程降低实现难度。

与输入无关的输出操作可以提高速度。





## 第十七章 优于 select 的 epoll

#### 基于select的IO复用技术速度慢的原因

-   调用select函数后常见的针对所有文件描述符的循环语句
-   每次调用select函数时都需要向该函数传递监视对象信息。

select优点：

-   服务端接入者少
-   程序具有兼容性



```c
struct epoll_event {
    __uint32_t events;
    epoll_data_t data;
}

typedef union epoll_data {
    void * ptr;
    int fd;
    __uint32_t u32;
    __uint64_t u64;
}epoll_data_t;
```



```c
#include <sys/epoll.h>

int epoll_create(int size);

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeouit);
```



#### 条件触发和边缘触发（level trigger) （edge trigger)

条件触发方式中，只要输入缓冲有数据就会一直通知该事件。



边缘触发：

-   通过errno变量验证错误原因
-   为了完成非阻塞IO，更改套接字特性



边缘触发方式中，接收数据时仅注册一次该事件。

因为这个特点，一旦发生输入相关事件，就应该读取输入缓冲中的全部数据。因此需要验证输入缓冲是否为空。

>   read函数返回-1，变量errno中的值为EAGAIN时，说明没有数据可读。

既然这样，为什么需要将套接字百年城非阻塞模式？边缘触发方式下，以阻塞方式工作的read&write函数有可能引起服务器端的长时间停顿。因此，边缘触发方式中一定要采用非阻塞read&write函数。



边缘触发可以分离接收数据和处理数据的时间点。

条件触发就没有办法区分数据接收和处理吗？并非不可能，但是在输入缓冲收到数据的情况下，如果不读取（延迟处理），则每次调用epoll_wait函数都会产生相应事件。而且事件数也会累加，服务器能承受吗？这在显示中式不可能的。

从实现模型的角度看，边缘触发更有可能带来高性能，但是不能简单认为只要使用边缘触发就一定能提高速度。



## 第十八章 

多进程模型缺点：

-   创建进程的过程会带来一定的开销
-   为了完成进程间的数据交换，需要特殊的IPC技术。

线程的创建和上下文切换比进程快，线程间交换数据无需特殊技术。



```c
#include <pthread.h>

int pthread_create(
	pthread_t *restrict thread, const pthread_attr_t  * restrict attr,
    void *(* start_routine)(void *), void restrict arg
);
```



线程安全函数

非线程安全函数



任何内存空间，之哟啊被同时访问，都有可能发生问题。



















