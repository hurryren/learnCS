# xv6: a simple, Unix-like teaching operating system





## Chapter 1 operating system interface

the job of an operating system is to share a computer among multiple programs and to provide a more useful set of services than the hardware along supports. An operating system manages and abstracts the low-level hardware, so that for example, a  word processor need not concern itself with which type of disk hardware is being used. An operating system shares the hardware among multiple programs so that they run at the same time. Finally, operating systems provide controlled ways for programs to interact, so that they can share data or work together.  

An operating system provides services to user programs through an interface. Designing a good interface turns out to be difficult. On the one hand, we would like the interface to be simple and narrow because that makes it easier to get the implementation. On the other hand, we may be tempted to offer many sophisticateds features to applications. The trick in resolving this tension is to design interfaces that rely on a few mechanisms that can be combined to provide much generality.

When a process needs to invoke a kernel service, it invokes a system call, one of the calls in the operating system’s interface. The system call enters the kernel; the kernel perrforms the service and return. Thus a process alternates between executing in user space and kernel space.

The kernel uses the hardware protrection mechanisms provided by a CPU to ensure that each process executing in user space can access only its own memory. The kernel executes with the hardware provileges required to implement these protections; user programs execute without those provileges. When a user program invokes a system call, the hardware raise the privilege level and starts executing a pre-arranged functions in the kernel.

The shell is an ordinary program that read commands from the user and executes them. The facts that the shell is a user program, and not part of the kernel, illustrates the power of the system call interface: there is nothing special about the shell. It also mean sthat the shell is easy to replace; as a result, moderne Unis systems hace a variety of shells to choose from, each with its own user interface and scripting features. The xv6 shell is a simple implementation of the essence of the Unix Bourne shell.

### 1.1  processes and memeory

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



### 1.2 IO and file descriptors

A file descriptor is a small integer representing a kernel-managed object that a process may read from or write to. A process may obtain a file descriptor by opening a file, directory, or device, or by creating a pipe, or by duplicating an existing descriptor. For simplicity we will often refer to the object a file descriptor refers to as a file; the file descriptor interface abstracts away the differences between files, pipes, and devices, making them all look like streams of bytes. We will refer to input and output as IO.

internally, the xv6 kernel use the file descriptor as an index into a per-process table, so that every process has a private space of file descriptors starting at zero. By convention, a process reads from file descriptor 0 (standard input), writes output to file descriptor 1 (standard output), and writes error messages to file descriptor(standard error). As we will see, the shell exploits the convention to implement IO redirection and pipelines. The shell ensure that it always has three file descriptor open, which are by default file descriptors for the console.



### 1.3 pipes

A pipe is a small kernel buffer exposed to processes as a pair of file descriptor, one for reading and one for writing. Writing data to one end of the pipe makes that data available for reading from the other end of the pipe. Pipes provide a way for processes to comumnicate.

Pipes have at least four advantages over temporary files in this situation. First, pipes automatically clean themselves up; with the file redirection, a shell would have to be careful to remove /tmp/xyz when done. Second, pipes can pass arbitrarity long streams of data, while file redirection requires enough free space on disk to store all the data. Third, pipes allow for parallel execution of pipeline stages, ehil the file approach requires the first program to finish before the second starts. Foruth, if you are implementing inter-process communication, pipes’ blocking reads and writes are more efficient than the non-blocking semantics of files.

### 1.4 file system

mknod creates a special file that refers to a device. Associated with a device file are the major and minor device numbers(the two arguments to mknod), whihc uniquely identify a kernel device. When a process later opens a device file, the kernel diverts read and write system calls to the kernel device implementation instead of passing them to the file system.

Unix provides file utilites callable from the shell as user-level programs, for example mkdir,ln, and rm. This design allows anyone to extend the command-line interface by adding new user-level programs. In hindsight this plan seems obvious, but other systems designed at the time of Unix often built such commands into the shell(and built the thell into the kernel).



### 1.5 real world

Unix’s combination of standard file descriptors, pipes, and convenient shell syntax for operations on them was a major advance in writing general-purpose reusable programs. The idea sparked a culture of software tools that was responsible for much of unix’s power and popularity, and the shell was the first so-called scripting language. the Unix system call interface persists today in systems like BSD, Linux, and macos.

The unix system call interface has been standardized through the portable operating system interface(POSIX) standard. XV6 is no POSIX compliant: it is missing many system call(including basic ones such as lseek), and many of the system calls it does provide differ from the standard. Our main gols for xv6 are simplicity and clarity while providing a simple UNIX-like standard. Our main gols for xv6 are simplicity and clarity while provide differ from the standard. Several pepole have extended xv6 with a few more system calls and a simple C library in order to run basic Unix programs. Modern kernels, however, provide many morew system calls, and many more kinds of kernel service, than xv6. For example, they support networking, windowing systems, user-level threadsd, drivers for many devices, and so on. Modern kernels evolve continuously and rapidly, and offer many features beyond POSIX.

unix unified access to multiple types of resources (file, directories, and devices) with a single set of file-name and file-descriptor interfaces. This idea can be extended to more kinds of resources; a good example is Plan9, which applied the resources are files concept to networks, graphics , and more. However, most unix-derived operating systems have not followed this route.

The file system and file descriptors have been powerful abstractions. Even so, there are other models for operating system interface. Multics, a predecessor of unix, abstracted file storage in a way that made it look like memory, producing a very different flavor of interface. the complexity of the multics design had a direct influence on the designers of unix, who tried to build somthing simpler.

xv6 does not provide a notion of users or of protecting one user from another; in unix terms all xv6 processes run as root.

This book exampines hwo xv6 implements ist unix-like interface, but the ideas and concepts apply to more than just Unix. Any operating system must multiplex processes onto the underlying hardware, isolate processes from each other, and provide mechanisms for controlled inter-process communication. after studying xv6, you should be able to look at other, more complex operating system and see the concepts underlying xv6 in those systems as well.





## chapter 2 operating system orgnization

a key requirement for an operating system is tyo support serveral activities at once. for example, using the system call interface descibed in chapter 1 a process can start new processes fork. The operating system must time-share the resources of the computer among these processes. For example, even if there are more processes than there are hardware CPUs, the operating system mush ensure that all of the processes get a chance to execute. The operating system must also arrange for isolation between the processes. That is, if one process has a bug and malfunctions, it shouldn’t affect processes that fon’t depend on the buffy process. Complete isolation, however, is too strong, since it should be possible for processee to intentionally interact; pipelines are an example. Thus an operaing system must fulfill three requirements: multiplexing, isolation, and interaction.

This chapter provides an overview of how operating systems are organized to acheieve these three requirements. It turns out there are many ways to do so, but this text focuses on mainstream designs centered around a monolithic kernel, whihc is used by many unix operating systems. This chapter also provides an overview of an xv6 process, which is the unit of isolation in xv6, and the creating of the first process when xv6 starts.

xv6 runs on a multi-core risc-v microprocessor, and much of its low-level functionality(for example, its process implemtation) is specific to RISC-V. RISC-V is a 64-bit CPU, and xv6 is written is LP64 c, which means long and pointers in the c programmming language are 64bits, but int is 32bit. This book assumes the reader has done a bit of machine-level programming on some architecture, and will introduce RISC-V specifi ideas as they come up.

The CPU in a complete computer is surrounded by support hardware, much of it in the form of IO interfaces. XV6 is writen for the support hardware simulated by qemu’s -machine virt option. This includes ram, a rom containing boot code, a serial connection to the user’ key-board/screen, and a disk for storage.

### 2.1 abstracting physical resources

the first question one might ask when encountering an operating system is why have it at all? That is, one could implement the system calls  as a library, with whihc applications link. In this plan, each application could even have its own library railored ot its needs. Applications could directly interact with hardware resources and use those reources in the best way for the application. Some operating systems for embeded devices or real-time systems are organized in this way.

the downside of this library approach is that, if there is more than one application running, the applications must be well-behaved. For example, each application must periodically give up the CPU so that other applications can run. such a cooperative time-sharing scheme may be OK if all applications trust each other and have no bugs. It is more typical for applications to not trust each other, and to have bugs, so one often want stronger isolation than a cooperative schemem provides.

To achieve strong isolation it’s helpful to forbid applictions from directly accessing sensitive hardware resources, and instead to abstract the resources into services. For example, unix appliction interact with storage only through the file system’s open read, write and close system calls, instead of reading and writing the disk directly. This provides the application with the convenience of pathnames, and it allows the operating system to manage the disk.

similarly, unix transparently switches hardware CPUs among processes, saving and restoring register state as necessary, so that applications don’t have to be aware of time sharing. This transparency allows the operating system to share CPUs even is some applications are in infinite loops.

As another example, unix processes use exec to build up their memory image, instead of directly interacting with physical memory. This allows the operating system to decide where to place a ptocess in memory. If memory is tight, the operating system might even store some of a process’s data on disk. Exec also provides users with the convenience of a file system to store executable program images.

Many forms of interaction among unix processes occur via file descriptors. Not only do file descriptors abstract away many details, they are also defined in a way that simplifies interaction. For example, if one application in a pipeline fails, the kernel generates an end-of-file signal for the next process in the pipeline.

The system-call interface is carefully designed to provide both programmer convenience and the possibility of strong isolation. the Unix interface is not the only way to abstract resources, but it has proven to be a very goog one.

### 2.2 user mode, supervisor mode, and system calls

Strong isolation requires a hard houndary between applications and the operating system. If the application makes a mistake, we don’t want the operating system to fail or other applications to fail. Instead, the operating system should be able to clean up the failed application and continuerunning other applications. To achieve strong isolation the operatomg system must arrange that application cannot modify (or even read) the operating system’s data structures and  instructions and that applications can not access other processes’s memeory.

CPUs provide hardware support for strong isolation. For example RISC-5 has three modes in which the CPU can execute instructions: machine mode, supervisor mode, and user mode. Instructions executing in machine mode have full privilege; a CPU starts in machine mode. Machine mode is mostly intended for configuring a computer. Xv6 executes a few lines in machine mode and then changes to supervisor mode.

In supervisor mode the CPU is allowed to execute privileged instructions: for example, enabling and disableing interrupts, reading and writing the register that holds the address of a pagetable, etc. If an application in user mode attempts to execute a privileged instruction, then the CPU doesn;t execute the isntruction, but switches to supervisor mode so that supervisor-mode code can terminate the application, because it did something it should not be doing. An application can execute only user-mode instructions and is said to be running in user space, while the software in supervisor mode can also execute privileged instructions and is said to be running in kernel space. The sofeware running in kernel space is called the kernel.

An application that wants to be invoke a kernel function must transition to the kernel; an application cannot invoke a kernel function directly. CPU provide a apecial instruction that switches the CPU from user mode to supervisor mode and enters the kernel at an entry point specified by the kernel(RISC-V provides the ecall instrction for this purpose.) Once the CPU has switched to supervisor mode, the kernel can then validate the arguments of the system call(e.g., check if the address passed to the system call is part of the application’s memory), decide whether the application is allowed to perform the requested operation(e.g., check if the application is allowed to write the specified file), and then deny it or execute is. It is important that the kernel control the entry point for transitions to supervisor mode; if the application could decide the kernel entry point, a malicious application could, for example, enter the kernel at a point where the validation of argument is skipped.



### 2.3 kernel organization

A key design question is what part of the operating system should run in supervisor mode. One possibility is that the entire operating system resides in the kernel, so that the implementations of all system calls run in supervisor mode. This organization is called a monolithic kernel.

In this organization the entire operating system runs with full hardware privilege. This organization is convenient because the OS designer doesn’t have to decide which part of the operating system doesn’t need full hardware privilege. Furthermore, it is easier for different parts of the opearating system to cooperate. For example, an operating system might have a buffer cache that can be shared both by the file system and the virtual memory sytem.

A downsife of the monolithic organizationis that the interfaces between different parts of the operating system are comple(as we will see in the rest of this text), and therefore it is easy for an operating system developer to make a mistake. In a monolithic kernel, a mistake is fatal, because an error in supervisor mode will often cause the kernel to fail. If the kernel fails, the computer stops working, and thus all applications fail too. The computer must reboot to start again.

To reduce the risk of mistakes in the kernel, OS designers can minimize the amount of operating system code that runs in supervisor mode, and execute the bulk of the operating system system in user mode. This kernel organization is called a microkernel.

In a microkernel, the kernel interface consists of a few low-level functions for starting applications, sending messages, accessing device hardware, etc. This organizaion allows the kernel to be relatively simple, as most of the operating system resides in user-level servers.

In the real world, both monolithic kernels and microkernels are popular. Many Unix kernels are monolithic. For example, linux has a monolithic kernel, although some OS functions run as user-level servers(e.g., the windowing system). Linux delivers high performance to OS-intensive applictions,m partially because the subsystems of the kernel can be tightly integrated.

There is much debate among developers of operating systems which organization is better, and there is no conlusive evidence one way or the other. Furthermore, it depends much on what better means, faster performance, smaller code size, reliability of the kernel, reliability of the complete operating system(inclusing user-level service).

From this book’s perspective, microkernel and monolithic operating system share many key ideas. They implement system calls, they use page tables, they handle interrupts, they support processes, they use lock for concurrenty control, they implement a file system, etc. This book focuses on these core ideas.

Xv6 is implemented as a monolithic kernel, like most unix operating systems. Thus, the xv6 kernel interface corresponds to the operating system interface, and the kernel implements the complete operating system. Since xv6 does not provide many services, its kernel is smaller than some microkernel, but concepture xv6 is monolithic.

### 2.5 process overview

The unit of isolation in xv6(as in other unix operating system) is process. The process abstrucatio prevents one process form wrecking or spying on another process’s memory, CPU, file descriptors, etc. It also prevents a process from wrecking the kernel itself, so that a process can not subvert the kernel isolation mechanisms. The kernel must implement the process abstraction with care because a buggy or malicous application may trick the kernel or hardware into doing something bad. The mechanisms used by the kernel to implement processes include the user/supervisor mode flag, address spaces, and time-slicing of threads.

To help enforce isolation, the process abstraction provides the illusion to a program that it has its own private machine.  A process provides a program with what appears to be a private memory system, or address space, which other processes cannot read or write. A process also provide the program with waht appears to be its own CPU to execute the program’s instructions.

Xv6 uses page tables(which are implemented by hardware) to give each process its own address space. The RISC-V page table translates a virtual address (the address that an RISC-V instruction manipulates) to a physical address(an address that the CPU chip send to main memory).

Xv6 maintains a separate page table for each process that defines that process’s address space. An address space includes the process’s user memory starting at virtual address zero. Instructions come first, followed by global variables, then the stack, and finally a heap area (for malloc) that the process can expand as needed. There are a number of factors that limit the maximum size of a process’s address space: pointers on the RISC-V are 64-bits wide; the hardware only use the low 39bits when looking up virtual addresses in page tables; and xv6 only use 38 of those 39 bits. Thus the maximum address is $$2^{38}-1$$=0X3fffffffff,which is MAXVA. At the top of the address space xv6 reserves a page for a trampolilne and a page mapping the process’s trapframe. Xv6 uses these two pages to transition into the kernel and back; the tranpoline page contains the code to transition in and out  of the kernel and mapping the trapframe is necessary to save/restore the state of the user process.

The xv6 kernel maintains many pieces of state for each process, which it gathers into  astruct proc. A process’s most important pieces of kernel state are its page table, its kernel stack, and ist run state. 

Each process has a thread of execution that execute the process’s instructions. A thread can be suspended and later resumed. To switch transparently between processes, the kernel suspends the current running thread and resumes another process’s thread. Much of the state of a thread(local variables, function call return addresses) is stored on the thread’s stacks. Each stack has two stacks: a user stack and a kernel stack. when the process is executing user instrctions, only its user stack is in use,and its kernel stack is empty. When the process enters the kernel(for a system call or interrupt), the kernel code executes on the process’s kernel stack; while a process is in the kernel, its user stack still contains saved data, but isn’t actively used. A process’s thread alternates between actively using its user stack and its kernel stack.

The kernel stack is separate(and protected from user code) so that the kernel can execute even is a process has wrecked its user stack.

A process can make a system call by executing the RISC-V ecall instruction. This instruction raise the hardware provilege level and changes the program counter to a kernel-defined entry point. The code at the entry point switches to a kernel stack and executes the kernel instructions that implement the system call. When the system call completes, the kernel switches back to the user stack and returns to user space by calling the sret instruction, which lowers the hardware privilege level and resumes execuing user instructions just after the system call instruction. A process’s thread can block in the kernel to wait for IO, and resume where it left off when the IO has finished.

### 2.6 starting xv6, the first process and system call

To make xv6 more concrete, we will outline how the kernel starts and runs the first process. The subsequent chapters will describe the mechanisms that show up in this overview in more detail.





























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




