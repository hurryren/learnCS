### 1 打开文件

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *path, int flag);
/*
成功时返回文件描述符，失败返回 -1
path : 文件名的字符串地址
flag : 文件打开模式信息
*/
```

文件打开模式如下表：

| 打开模式 | 含义                       |
| -------- | -------------------------- |
| O_CREATE | 必要时创建文件             |
| O_TRUNC  | 删除全部现有数据           |
| O_APPEND | 维持现有数据，保存到其后面 |
| O_RDONLY | 只读打开                   |
| O_WRONLY | 只写打开                   |
| O_RDWR   | 读写打开                   |



### 2 关闭文件

```c
#include <unistd.h>
int close(int fd);
/*
成功返回 0，失败返回 -1
fd : 需要关闭的文件或套接字的文件描述符
*/
```



### 3 写入文件

```c
#include <unisted.h>
ssize_t write(int fd, const void *buf, size_t nbytes);
/*
成功时返回写入的字节数，失败返回 -1
fd : 显示数据传输对象的文件描述符
buf : 保存数据的buffer地址
nbytes : 要传输的数据的长度
```

在此函数定义中，size_t是通过typedef声明的unsigned int类型。对ssize_t来说，ssize_t前面多加的s代表signed，既ssize_t是通过typedef声明的signed int类型。



### 4 读取文件

```c
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t nbytes);
/*
成功时返回接受的字节数（遇到文件结尾则返回0），失败返回-1
fd : 显示数据接收对象的文件描述符
buf : 要保存接收的数据的缓冲地址值
nbytes : 要接收数据的最大字节数
```







































