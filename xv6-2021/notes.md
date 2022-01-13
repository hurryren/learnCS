# xv6: a simple, Unix-like teaching operating system





## Chapter 1 operating system interface

the job of an operating system is to share a computer among multiple programs and to provide a more useful set of services than the hardware along supports. An operating system manages and abstracts the low-level hardware, so that for example, a  word processor need not concern itself with which type of disk hardware is being used. An operating system shares the hardware among multiple programs so that they run at the same time. Finally, operating systems provide controlled ways for programs to interact, so that they can share data or work together.  

An operating system provides services to user programs through an interface. Designing a good interface turns out to be difficult. On the one hand, we would like the interface to be simple and narrow because that makes it easier to get the implementation. On the other hand, we may be tempted to offer many sophisticateds features to applications. The trick in resolving this tension is to design interfaces that rely on a few mechanisms that can be combined to provide much generality.

When a process needs to invoke a kernel service, it invokes a system call, one of the calls in the operating system’s interface. The system call enters the kernel; the kernel perrforms the service and return. Thus a process alternates between executing in user space and kernel space.

The kernel uses the hardware protrection mechanisms provided by a CPU to ensure that each process executing in user space can access only its own memory. The kernel executes with the hardware provileges required to implement these protections; user programs execute without those provileges. When a user program invokes a system call, the hardware raise the privilege level and starts executing a pre-arranged functions in the kernel.

The shell is an ordinary program that read commands from the user and executes them. The facts that the shell is a user program, and not part of the kernel, illustrates the power of the system call interface: there is nothing special about the shell. It also mean sthat the shell is easy to replace; as a result, moderne Unis systems hace a variety of shells to choose from, each with its own user interface and scripting features. The xv6 shell is a simple implementation of the essence of the Unix Bourne shell.

## 1.1  processes and memeory

An xv6 process consist of user-space memory (instructions, data, and stack) per-process state private to the kernel. Xv6 time-share precesses: it transparetly switches the available CPUs among the set of processes waiting to execute.When a process is not executing, xv6 saves its CPU registers, restoring them when is next run the process. The kernel associates a process identifier, or PID, with each process.

A process may create a new process using the fork system call. Fork gives the new process exactly the same memory contents (both instructions and data) as the calling process. Fork returns in both the original and new processes. In the original process, fork returns the new process’s PID. In the new process, fork returns zero. The original and new processes are often called the parent and child.

| system call                            | description                                                  |
| -------------------------------------- | ------------------------------------------------------------ |
| int fork()                             | create a process, return child’s PID                         |
| int exit(int status)                   | Terminate the current process; status reported to wait(). No return |
| int wait(int *status)                  | Wait for a child to exit; exit status in *status; returns child PID.* |
| int kill(int pid)                      | terminate process PID, return 0, or -1 for error             |
| int getpid()                           | return the current process’s pid                             |
| int sleep(int n)                       | pause for n clock ticks                                      |
| int exec(char \*file, char* argv[])    | load a file and execute it with arguments; only returns if error |
| char \*sbrk(int n)                     | grow process’s memory by n bytes. return start of new   memory |
| int open(char \*file, int flags)       | open a file; flags indicate read/write; returns an fd(file descriptor) |
| int write(int fd, cahr \*buf, int n)   | write n bytes from buf to file descriptor fd; returns n.     |
| int read(int fd, cahr \*buf, int n)    | read n bytes into buf; returns number read; or 0 if end of file. |
| int close(int fd)                      | realease open file fd                                        |
| int dup(int fd)                        | return a new file descriptor referring to the same file as fd |
| int pip(int p[])                       | create a pipe, put read/write file descriptors in p[0] and p[1] |
| int chdir(char \*dir)                  | change the current directory                                 |
| int mkdir( char \*dir)                 | create a new directory                                       |
| int mknod(char \*file, int, int)       | create a device file                                         |
| int fstat(int fd, struct stat \*st)    | place info about an open file into \*st                      |
| int stat(char \*file, struct stat *st) | place info about a named file into \*st                      |
| int linke(char \*file1, char \*file2)  | create abother name (file2) for the file1                    |
| int unlink(char \*file)                | remove a file                                                |

The xv6 shell uses the above calls to run programs on behalf of users. The main structure of the shell is imple. The main loop reads a line of input from the user with getcmd. Then is calls fork, which creates a copy of the shell process. The parent calls wait, while the child runs the command. 



## 1.2 IO and file descriptors

A file descriptor is a small integer representing a kernel-managed object that a process may read from or write to. A process may obtain a file descriptor by opening a file, directory, or device, or by creating a pipe, or by duplicating an existing descriptor. For simplicity we



































# VIRTIO

文档名称：Virtual I/O Device(VIRTIO) Version 1.1

#### Abstract：

This document describes the specifications of the “virtio” family of device. These device are found in virtual environments, yet by design they look like physical devices to the guest within the virtual machine-and this document treats them as such. This similarity allows the guest to use standard drivers and discovery mechanisms.

The purpose of virtio and this specification is that virtual environments and guests should have a straightforward, efficient, standard and extensible mechanism for virtual device, rather thatn boutique per-environment or per-OS mechanisms.



## 1 introduction



this document describes the specifications of the “virtio” family of devices. These are found in virtul envionments, yet by design they look like physical devices of to the guest within the virtual machine and this document treats them as such. This similarity allows the guest to use standard drivers and discovery mechanisms.

The purpose of virtio and this specification is that virtual environments and guests should have a straightforward, efficient, standard and extensible mechanism for virtual devices, rather than boutique per-environment or per-OS mechanisms.

-   **Straightforward**: Virtio devices use normal bus mechanisms of interrupts and DMA which should be familiar to and device author. There is no exotic page-flipping or COW mechanim: it’s just a normal device.
-   **Efficient**: Virtio devices consist of rings of descirptors for both input and output, which are neatly laid out to avoid cache effects from both driver and device writing to the same cache lines.
-   **Standard**: Virtio make no assumptions about the environment in which it operates, beyond suporting the bus to which device is attached. In this specification, virtio devices are implemented on ther buses not inlcuded here.
-   **Extension**: Virtio devices contain feature bits which are acknowledged by the guest operating system during device setup. This allows forwards and backwards compatibililty: the device offers all the features it knows about, and the driver acknowledges those it understands and wished to use.



##### 1.4 struct specifications

Many device and driver in-memory structure layouts are documented using the c struct syntax. All structures are assumed to be without additional padding. To stress this, cases where common c compilers are known to insert extra padding within strucures are tagged using the GCU c \__attribute__((packed)) syntax.





