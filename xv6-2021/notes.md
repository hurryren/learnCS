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

When the RISC-V computer powers on, it initializers itself and runs a boot loader which is stored in read-only memory. The boot loader loads the xv6 kernel into memory. Then, in machine mode, the CPU executes xv6 starting at _entry. The RISC-V starts with paging hardware disable: virtual address map directly to physicall addresses.

The loader loads the xv6 kernel into memory at physical address 0x80000000. The reason is place the kernel at 0x80000000 rather than 0x0 is because the address range 0x0:0x80000000 contains IO device.

The instruction at _entry set up a stack so that xv6 can run C code. Xv6 declares space for an initial stack, stack0, in the file start.c. The code at _entry loads the stack pointer register sp with the address stack0+4096, the top of the stack, becase the stack on  RISC-V grown down. Now the kernel has a stack, _entry calls into C code at start.

The function start performs some configuration that is only allowed in machine mode, and then switches to supervisor mode. To enter supervisor mode, RISC-V provides the instruction sret. This instruction is most often used to return from a previous call from supervisor mode to machine mode. start is not returning from such a call, and instead set things up as if there had been one: it sets the previous privilege mode to supervisor in the register mstatus, it sets the return address to main by writing main’s address into the register mepc,  disable virtual address translation in supervisor mode by writing 0 into th page-table register satp, and delegates all interrupts and exceptions to supervisor mode.

Before jumping into supervisor mode, stack performs one more task: it programs the clock chip generate timer interrupts. With this housekeeping out of the way, start returns to supervisor mode by calling mret. This causes the program counter to change to main.

After main initializes several devices and subsystems. It creates the first process by calling userinit. The first process execute a small program written in RISC-V assembly, make makes the first system call in xv6. initcode.S loads the number for the exec system call, SYS_EXEC, into register a7, and then calls ecall to re-enter the kernel.

The kernel uses the number in register a7 in syscall to call the desired system call. The system call table maps SYS_EXEC to sys_exec, which the kernel invokes. As we saw in chapter 1, exec replaces the memory and registers of the current process with a new program.

Once the kernel has completed exec, it returns to user space in the init process. Init creates a new console device file if needed and the open it as file descriptors 0, 1 and 2. Then it start a shell on the console. The system is up.

### 2.7 system mode

The operating system must assume that a process’s user-level code will do its best to wreck the kernel or other processes. User code may try to dereferece pointers outside its allowed address space; it may attempt to execute any RISC-V instructions, even those not intended for user code; it may try to read and write RISC-V control register; it may try to directly access device hardware; and it may pass clever values to system calls in an attempt to trick the kernel into crashing or doing somthing stupid. The kernel’s goal to restrict each user processes so that all it can do ti read/write/execute its own user memory, use the 32 general-purpose RISC-V registers, and affect the kernel and other processes in the ways that system calls are intended to allow. The kernel must prevent any other actions. This is typically an absolute requirement in kernel design.

The expectations for the kernel’s own code are quite different. Kernel code is assumed to be written by well-meaning and careful programmers. Kernel code is expected to be bug-free, and certainly to contain nothing malicious. The assumption affects how we analyze kernel code. For example, there are many internal kernel functions that would cause serious problem if kernel code use them incorrectly. When examining any specific piece of kernel code, we will convince ourselves that it behaves correctly. We assume, however, that kernel code in general is correctly written, and follows all the rules about use of kernel’s own functions and data structures. At the hardware level, the RISC-V CPU, RAM, disk, etc. are assumed to operate as advertised in the documentation, with no hardware bugs.

Of course in real life things are not so straightforward. It’s difficult to prevent clever user code from making a system unusable by consuming kernel-protected resources disk space, CPU time, process table slots, etc. It’s usually impossible to write bug-free code or design bug-free hardware. If the writers of malicious user code are aware of kernel or hardware bugs, they will exploit them.

It is worthwhile to design safeguards into the kernel against the possibility that it has bugs: assertion, type checking, stack guard pages, etc.

Finally, the distinction between user and kernel code is something blurred: some privileged user-level processes may provide essential services and effectively be part of the operating system, and in some operating systems privilege user code can insert new code into the kernel.

### 2.8 real world

Most operating systems have adopted the process concept, and most processes look similar to xv6’s. Modern operaitng systems, however, support several threads within a process, to allow a single process to exploit multiple CPUs, Supporting multiple thread in a process involves quite a bit of machinery that xv6 doesn’t have, including potential interface changes, to control which aspects of a process thread share.



## chapter 3 pgae tables

Page tables are the most popular mechanism through the operating system provides each process with its own private address space and memory. Page tables determine what memory adresses mean, and what parts of physical memory can be accessed. They allow xv6 to isolate different process’s address spaces and mutiplex them onto a single physical memory. Page tables are a popular design because they provide a level of indirection that allow operating systems to perform many tricks. Xv6 performs a few tricks: mapping the same memory(a trampoling page) in several address spaces, and guarding kernel and user stack with an unmapped page.

### 3.1 paging hardware

As a reminder, RISC-V instructions(both user and kernel) manipulate virtual addresses. The machine’s RAM, or physical memory, is indexed with physical addresses. The RISC-V page table hardware connects these two kinds of addresses, by mapping each virtual address to a physicall address.

xv6 runs on Sv39 RISC-V, which means that only the bottom 39 bits of a 64-bit virtual address are used; the top 25 bits are not used. In this Sv39 configuration, a RISC-V page table is logically an array of  $$2^{27}$$(134217728) page table entries(PTEs). Each PTE contains a 44-bit physical page number(PPN) and some flags. The paging hardware translates a virtual address by using the top 27 bits of the 39 bits to index into the page table to find a PTE, and making a 56-bit physical address whose top 44 bits come from the PPN in the PTE and whose bottom 12 bits are copied from the original virtual address. A page table gives the operating system control over virtual-to-physical address translations at the granularity of aligned chunks of 4096 bytes. Such chunks is called page.

In Sv39 RISC-V, the top 25 bits of a virtual address are not used for translation. The physical address also has room for growth: there is room in the PTE fromat for the physical page number to grow by another 10 bits.  The designer of RISC-V chose these numbers based on technology predictions. $2^{39}$ is 512 GB, which should be enough address space for applications running on RISC-V computers.  

A RISC-V CPU translates a virtual address into a physical in three steps. A page table is stored in physical memory as a three level tree. The root of the tree is a 4096-byte page-table page that contains 512 PTEs, which contain the physical addresses for page-table pages in the next level of the tree. Each of those pages contains 512 PTEs for the final level in the tree.

If any of the three PTEs required to translate an address is not present, the pageing hardware raise a page-fault exception, leaving it up to the kernel to handle the exception.

The three-level structure allows a memory-efficient way of recording PTEs, compared to the single-level design. In the common case in which large ranges of virtual addresses have no mapping, the threee-level structure can omit entire page directories. For example ,if an application uses only a few pages starting at address zero, then the entries 1 through 511 of the top-level page directory are invalid, and the kernel does not have to allocate pages those for 511 intermediate page directories. Furthermore, the kernel also does not have to allocate pages for the bottom-level page directories for those 511 intermediate page directories. So, in this example, the three level design saves 511 pages for intermediate page directories and $511*512$ pages for bottom-level page directories.

Each PTE contains flag bits that tell the paging hardware how the associated virtual address is allowed to be used. PTE_V indicatres whether the PTE is present: if is not set, a reference to the page cause an exception. PTE_R controls whether instructions are allowed to read the page. PTE_W controls whether instructions are allowed to write to the page. PTE_X controls whether  the CPU may interpret the content of the page as instructions and execute them. PTE_U controls whether instructions in user mode are allowed to access the the page; If PTE_U is not set, the PTE can be only used in supervisor mode. 

To tell the hardware to use a page table, the kernel must write the physical address of the root page-table page into the satp register. Each CPU has its own satp. A CPU will translate all addresses generated by subsequence instructions using the page table pointed to by its own satp. Each CPU has its own satp so that different CPUs can tun different processes, each with a private address space descriped by its own page table.

Typically a kernel maps all of physical memory into its page table so that it can read and write any location in physical memory using load/store instructions. Since the page directories are in physical memory, the kernel can program the content  of a PTE in a page by writing to the virtual address of the PTE using a standard store instruction.

### 3.2 kernel address space

QEMU simulates a computer that includes RAM (physical memory) starting at physical address 0x80000000 and continuing through at least 0x86400000, which xv6 calls PHYSTOP. The QEMU simulation also include IO device such as a disk interface. QEMU expose the device interfaces to software as memory-mapped control registers that sit below 0x80000000 i n the physical addresses. The kernel can interact with the devices by reading/writing these special physical addresses; such reads and writes communicate with the device hardware rather than with RAM.

The kernel gets at RAM and memory-mapped device register using direct mapping; that is, mapping the resources at virtual address that are equal to the physical address. For example, the Kernel itself is located at KERNELBASE=0x80000000 in both the virtual address space and in physical memory. The kernel can interact with the device by reading/writing these special physical addresses; such read and writes communicate with the device hardware rather than with RAM.

### 3.3 code: creating an address space

Most of the xv6 code for manipulating address spaces and page tables resides in vm.c. The central data structure is *pagetable_t*, which is really a pointer to a RISC-V root page-tbale page; a pagetable_t may be either the kernel page table, or one of the per-process page tables. The central functions are *walk*, which finds the PTE for a virtual address, and *mappages*, which installs PTEs for new mappings. Functions starting with *kvm* manipulate the kernel page table; functions starting with *uvm* manipulate a user virtual addresses provided as system call arguments; they are in *vm.c* because they need to explicitly translate those addresses in order to find the corresponding physical memory.

Early in the boot sequence, *main* calls *kvminit* to create the kernel’s page table using kvmmake. This call occurs before xv6 has enabled paging on the RISC-V, so addresses refer directly to physical memory. *Kvmmake* first allocates a page of physical memory to hold the root page-table page. Then it calls kvmmap to install the translations that the kernel needs, The translations include the kernel’s instructions and data, physical memory up to *PHYTOP*, and memory ranges which are actually devices. *Proc_mapstacks* allocates a kernel stack for each process. It call s *kvmmap* to map each stack at the virtual address generated by KSTACK, which leaves root for the invalid stack-guard pages.

kvmmap calls mappages, which install smapping into a page table for a range of virtual addresses to a corresponding range of physical addresses. It does this separately for each virtual address in the range, at page intervals. For each virtual address to be mapped, mappages calls walk to find the address of the PTE for that address. It then initializes the PTE to hold the relevant physicall page number, the desired permissions, and PTE_V to mark the PTE as valid.

walk  mimics the RISC-V pageing hardware as it looks up the PTE for a virtual address. walk descends the 3-level page table 9-bits at the time. It uses each level’s 9-bits of virtual address to find the PTE of either the nerxt-level page table or the final page. If the PTE is not valid, then the required page has not yet been allocated; if the alloc argument is set, walk allocates a new page-table page and puts its physical address in the PTE. It returns the address of the PTE in the lowest layer in the tree.

The above code depends on physical memory being direct-mapped into the kernel virtual address space. For example, as walk descend levels of the page table, it pulls the physical address of the next-level-down page table from ae PTE, and thene uses that address as a virtual address to fetch the PTE at the next level down.

*main* calls kvminithart to install the kernel page table. It writes the physical address of the root page-tbale page into the register satp. After this the CPU will translate addresses using the kernel page table. Since the kernel uses anidentity mapping, the now virtual address of the next instrction will map to the right physical memory address.

Each RISC-V CPU caches page table entries in a Translation Look-aside Buffer(TLB), and when xv6 changes a page tbale, it must tell the CPU to incvalidate corresponding cached TLB entries. If it did not , then at some point later the TLB might use an old cached mapping, pointing to a physcal page that in the meantime has been allocated to another proce4ssm and as a result, aprocess might be able to scribble on some other process’s memory. The RISC-V has an instruction *sfence.vma* that flushes the current CPU’s TLB. Xv6 executes *sfence.vma* in kvminithartt after reloading the satp register, and in the trampoline code that switches to a user page table before returning to user space.

To avoid flushing the complete TLB, RISC-V CPUs may support address spaces identifiers. The kernel can then flush just TLB entries for a particular address spaces.

### 3.4 Physical memory allocation

The kernel must allocate and free physical memory at run-time for page tables, user memory, kernel stacks, and pipe buffers.

xv6 uses the physical memory between the end of the kernel and PHYTOP for run-time allocation. It allocates and frees whole 4096-byte pages at a time. It keeps track of which pages are free by threading a linked list through the pages themselves. Allocation consists of removing a page from the linked list; freeing consists of adding the freed page to the list.

### 3.5 physical memory allocator

The allocator resides in kalloc.c. The allocator’s data structure is a free list of physical memory pages that are available for allocation. Each free page’s list element is a struct run. Where does the allocator get the memory to hold that data structure? It store each free page’s run structure in the free page itself, since there’s nothing else stored there. The free list is protected by a spin lock. The list and the lock are wrapped in a struct to make cleat that the lock protect the fields in the struct. for now, ignore the lock and calls to acqueire and release;

The function main calls kinit to initialize the allocator. Kinit initilizes the free list to hold every page between the end of the kernel and PHYSTOP.xv6 ought to determine how much physical memory is availabel by parding configuration information provided by the hardware. Instead xv6 assumes that the machine has 128 megabytes of RAM. kinit calls freerange to add memory to the free list via per-page calls to kfree. A PTE can only refer to a physical address that is aligned on a 4096-byte boundary (is a multiple of 4096), so freerange uses PGROUNUP to ensure that it frees only aligned physical addresses. The allocator starts with no memoryl; these calls to kfree give it some to manage.

The allocator sometimes treats addresses as integers in order to perform arithmetic on them, and somtimes uses addresses as pointers to read and write memory;this dual use of addresses is the main reason that the allocator code is full of C tyep casts. The other reason is that freeing and allocating inherently change the tyep of the memory.

The function kfree begins by setting every byte in the memory being freed to the value 1. This will cause code that uses memory after freeing it to read garbage instead of the olf valid contents; hopfully that will cause such code to break faster. Then kfree prepends the page to the free list: it casts pa to a pointer to struct run, records the old start of the free list in r->next, and sets the free list equal to r.kalloc removes and returns the first element in the free list.

### 3.6 process address space

each process has a separate page table, and when xv6 switches between processes, it also changes page tables. a process’s user memory starts at virtual address zero and can grow up to MAXVA, allowing a process to address in principle 256Gigabytes of memory.

When a process ask xv6 for more user memory, xv6 first uses kalloc to allocate physical pages. It then adds PTEs to the process’s page table that point to the new physical pages. Xv6 sets the PTE-W,PTE_X, PTE_R, PTE_U, and PTE_V flags in these PTEs. Most processes do not use the entire user address space; xv6 leaves PTE_V clear in unused PTEs.

We see here a few nice examples of use of page tables. First, different processes’ page tabls translate user addresses to different pages of physical memory, so that each process has private user memory. Second, each process sees its memory can be non-contiguous. Third, the kernel maps a page with trampoline code at the top of the user address space, thus a single page of physical memory shows up in all address spaces.

The stack is a single paghhe, and is shown with the initial contents as created by exec. Strings containing the command-line arguments, as well as an array of pointers to them, are at the very top of the stack. Just under that are values that allows a program to start at main as if the function main had just been called.

To detect a user stack overflowing the allocated stack memory, xv6 places an inaccessible guard page right below the stack by clearing the PTE_U flag. If the user stack overflows and the process tries to use an address below the stack, the hardware will generate a page-faule exception because the guard page is inaccessible to a program running in user mode. A real-world operaing system might instead automatically allocate more memory for the user stack when it overflows.

### 3.7 sbrk

sbrk is the system call for process to shrink or grow its memory. The system call is implemented by the function growproc. growproc calls uvmalloc or uvmdealloc, depending on whether n is positive or negative. uvmalloc allocates physical memroy with kalloc, and adds PTEs to the user page table with mappages. uvmdealloca calls uvmunmap, which uses walk to find PTEs and kfree to free the physical memory they refer to.

xv6 uses a process’s page table not just to tell the hardware how to map user virtual addresses, but also as the only record of which physical memory pages are allocated to that process. That is the reason why freeing user memory requires examination of the user page table.

### 3.8 exec

exec is the system call that creates the user part of an address space. It initializes the user part of an address space from a file stored in the file system. Exec opens the named binary path using namei. Then, it reads the ELF header. Xv6 applications are described in the widely-used ELF format. An ELF binary consists of an ELF header, struct elfhdr, followed by a sequence of program section headers, struct proghdr. Each proghdr describes a section of the application that must be loades into memory; xv6 programs have only one program section header, but other systrems might have separate sectinos for instructins and data.

The first step is a quick check that the file probably contains an ECL binary. An ELF binary starts with the four-byte magic number 0x7f, e,l,f, or ELF_MAGIC. If the ELF header has the right magic numebr, exec assumes that the binary is well-formed.

Exec allocates a new page tbale with no user mappings with proc_pagetable, allocates memory for each ELF segment with uvmalloc, and loads each segments into memory with loadseg. loadseg yses walkaddr to find the physical address of the allocated memory at which to write each page of the ELF segment, and readi to read from the file.

The program section headers filesz may be less than the memsz, indicating that the gap between them should be filled with zeros rather than read from the file.

It is easy for a kernel developer to omit a crucial check, and read-world kernels hacve a long history of missing checks whose absence can be exploited by user programs to obtain kernel privileges. It is likely that xv6 does not do a complete job of validating user-level data supplied to the kernel, which a malicious user program might be able to exploit to circumvent xv6’s isolation.

### 3.9 real world

Like most operating systems, xv6 uses the paging hardware for memory protection and mapping. Most operating systems make far more sophisticated use of pageing than xv by combining pageing and page-fault exceptions.

Xv6 is simplified by the kernel’s use of a direct map between virtual and physical addresses, and by its assumption that there is physical RAM at address 0x8000000, where the kernel expects to be loaded. This works with QEMU, but on real headware it turns out to be bad idea; real hardware places RAM and devices at unpredictable physical addresses, so that there might be no RAM at 0x8000000, where xv6 expect to be able to store the kernel, More serious kernel designs exploit the page table to turn arbitrary hardware physical memory layouts into predictable kernel virtual address layouts.

RISC-V supports protection at the level of physical addresses, but xv6 doesn’t use that feature.

On machines with lots of memory it mights make sense to use RISC-V support for suprt pages. small pages make sense when physical memory is small, to allow allocation and page-out to disk with fine granularity. For example, if a program uses only 8 kilobytes of memory, giving it a whole 4-megabyte super-page of physical memory is wastful. Larger pages make sense on machines with lots of RAM, and may reduce overhead for page-table manipulation.

The xv6 kernel’s l;ack of a malloc-like allocator that provide memory for small objects prevents the kernel from using sophisticated data structures that would require dynamic allocation.

memory allocation is a perenial hot topic, the basic problems being efficient use of limited memory and preparing for unknown future requests.Today people care more about speed than space efficiency. In addition, a more elaborate kernel would likely allocate many different size of small blocks, rather thatn just 4096-byte blocks; a real kernel allocator would need to hadle small allocation as well as large ones.

## chapter 4 traps and system calls

There are three kinds of event which cause the CPU to set aside ordinary execution of instructions and force a transfer of control to special code that handles the event. One situation is a system call, when a user program executes the *ecall* instruction (user or kernel) does somthing illegal, such as divide by zero or use an ivalid virtual address. The third situation is a device interrupt, when a device signals that it needs attention, for example when the disk hardware finishes a read or write request.

This book uses trap as a generic term for these situations. Typically whatever code was executeing at the time of the trap will later need to resume, and shouldn’t need to be aware that anything special happeded. That is, we often want traps to be transparent; this is particulay important for device interrupts, which the interrupted code typically doesn’t expect. The usual sequence is that a trap forces a transfer of control into the kernel; the kernel saves registers and other state so that execution can be resumed; the kernel executes appropriate handler code(e.g., a system call implementation or device driver); the kernel restores the saved state and returns from the trap; and the original code resumes where it left off.

xv6 handles all traps in the kernel; traps are not delivered to user code. Handling traps in the kernel is natural for system calls. It make s sense for interrupts since isolation demands that only the kernel be allowed to use devices, and because the kernel is a convenient mechanism with which to share device among multiple process. It also makes sense for exception since xv6 responds to al exceptions from user space by killing the offending program.

Xv6 trap handling proceeds in four stages: hardware actins taken by the RISC-V CPU, some assembly instruction that prepare the way for kernel c code, a C function that decides what to do with the trap, and the system call or device-driver service routine. While commonality among the three trap types suggests that a kernel could handle all traps with a single code path, it turns out to be convenient to have separate code for three distinct cases: traps from user space, traps from kernel space, and timer interrupts. Kernel code (assembler or C) that processes a trap is often called a handler; the first handler instructions are usually written in assembler(rather than C) and are sometimes called a vector.

### 4.1 RISC-V trap machinery

Each risc-v CPU has a set of control registers that the kernel writes to tell the CPU how to handle traps, and that the kernel can read to find out about a trap that has occured. The RISC-V documents contain the full story[1].riscv.h(kernel/riscv.h) contains definition that sv6 uses. Here’s an outline of the most important registers:

-   *stvec*: The kernel writes the address of its trap handler here; the RISC-V jumps to the address in stevc to handle trap.
-   *sepc*: When a trap occurs, RISC-V saves the programs counter here(since the pc is then overwritten with the value in stvec). The sret (return from trap) instruction copies sepc to pc. The kernel can write sepc to control where sret goes.
-   *scause*: RISC-V puts number here that describes the reason for the trap.
-   *sscratch*: The kernel placves a value here that comes in handy at the very start of a trap handler.
-   *sstatus*: The SIE bit in sstatus controls whether device interrupts are enabled. If the kernel clears SIE, the risc-v will defer device interrupts until the kernel sets SIE. The SPP bit indicates whether a trap came from user mode or supervisor mode, and controls to what mode sret returns.

The above registers relate to traps handled in supervisor mode, and they can not be read or written in user mode. There is similar set of control registers for traps handled in machine mode; xv6 uses them only for the special case of timer interrupts.

Each cpu on a multi-core chip has its own set of these registers, and more than one CPU may be handling a trap at any given time.

When it needs to force a trap, the RISC-V hardware does the follwing for all trap types(other than timer interrupts):

1.   If the trap is a device interrupt, and the sstatus SIE bit is clear, don’t do any of the following.
2.   Disable interrupts by clearing the SIE bit in sstatus.
3.   Copy the pc to sepc.
4.   save the current mode(user or supervisor) in the spp bit in sstatus.
5.   set scause to reflect the trap’s cause.
6.   set the mode to supervisor.
7.   copy stvec to the pc
8.   start executing at the new pc.

Note that the CPU does not switcch to the kernel page table, does not switch to a stack in the kernel, and does not save any registers other than the pc. kernel software must perform these tasks. One reason that the CPU does minimal work during a trap is to provide flexibility to software; for example, some operating systems omit a page table switch in some situations to increate trap performance.

It is worth thinking about whether any of the steps listed above could be omitted, paerhaps in search of faster traps. Though there are situations in which a simpler sequence can work, many of the steps would be dangerous to omit in general. For example, suppose that the CPU did not switch program counters. Then a trap from user space could switch to supervisor mode while still running user instructions. Those user instructions could break user/kernel siolation, for example by modifying the satp register to point to a page table that allowed accessing all of physical memory. It is thus importtant that the CPU switch to a kernel-specified instrction address, namely stvec.

### 4.2 traps from user space

xv6 handles traps differently depending on whether it is executing in the kernel or in user code.

A trap may occur while executing in user space if the user program makes a system call(ecall instruction), or does something illegal, or if a device interrupts. The high-level path of a trap from user space is uservec, then usertrap; and when returning, usertrapret and then userret.

A major constraint on the design of xv6’s trap handling is the fact that the risc-v hardware does not switch page tables when it forces a trap. This means that the trap handler address in stvec must have avalid mapping in the user page table, since that  is the page table in force when the trap handling code starts executing. Furthermore, xv6’s trap handling code needs to switch to the kernel page table; in order to be able to continue executing after that switch, the kernel page table must also hacve a mapping for the handler pointed to by stvec.

xv6 satisfies these requirements using a trampoline page. The trampoline page contains uservec, the xv6 trap handline code that stvec points to. The trampoline page is mapped in every process’s page tbal at address TRAMPOLINE, which is at the end of the virtual address space so that it will be above memroy that programs use for themselves. The trampline page is also mapped at address TRAMPOLINE in the kernel page tabel. Because the trampoline page is mamped in the user page tbale, with the PTE_U flag, traps can start executing there in supervisor mode. Because the trampoline page is mapped at the same address in the kernel address space, the trap handler can continue to execute after it switches to the kernel page table.

The code for the uservec trap handler is in trampoline.s. When uservec starts, all 32 registers contain values owned by the interrupted user code. These 32 values need to be saved somewhere in memory, so that they can be restored when the trap returns to user space. Storing to memroy requires use of a register to hold the address, but at this point there are no general-purpose registers availabel! Luckily RISC-V provides a helping hand in the form of the sscratch register. The cerrw instruction at the start of uservec swaps the contents of a0 and sscratch. Now the user code’s a0 is saved in sscratch; uservec has one register(10) to play with; and a0 contains the value the kernel previously placed in sscratch.

uservec’s next task is to save the 32 user registers. Before entering user space, the kernel set sscratch to point to a per-process trapframe structure that has space to save the 32 user register. Because satp stil refers to the user page table, uservec needs the trapframe to be mapped in the user address space. When creating each process, xv6 allocates a page for the process’s trapframe, and arranges for it always to be mapped at user virtual address TRAPFRAME, though at its physical address so the kernel can use it through the kernel page table.

Thus after swapping a0 and sscratch, a0 holds a pointer to the current process’s trapframe. uservec nows saves all user registers there, including the user’s a0, read from sscratch.

The trapframe contains the address of the current process’s kernel stack, the current CPU’s hartid, the address of the usertrap function, and the address of the kernel page table. uservec retrieves these values, switches satp to the kernel page table, and calls usertrap.

The job of usertrap is to determine the cause of the trap, process it, and return. It first changes stvec so that a trap while in the kernel will be handled by kernelvec rather than uservec. It saves the sepc register (the saved user program counter), because usertrap might call yield to switch to another process’s kernel thread, and that process might return to user space, in the process of which it will modify sepc. If the trap is a system call, usertrap calls syscall to handle it; if a device interrrupt, devintr; otherwise it is an exception, and the kernel kills the faulting process. The system call path adds four to the saced user program counter because RISC-V, in the case of a system call ,leaves the program pointer pointing to the ecall instruction but user code needs to resume executing at the subsequent instruction. on the way out, usertrap checks if the process has been keiiled or should yield the CPY (if this trap is a timer interrupt).

The first step in returning to user space is the call usertrapret. This function sets up the risc-v control registers to prepare for a future trap from user space. This involues changing stvec to refer to uservec, preparing the trapframe fields that uservec relied on, and setting sepc to the previously saved user program counter. At the end, usertrapret calls userret opn the trampoline page that is mapped in both user and kernel page tbales; the reason is that assembly code in userret will switch page tables.

usertrapret’s call to userret passes trapframe in a0 and a pointer to the process’s user page tbale in a1. userret switches satp to the process’s user page table. Recall that the user page table maps both the trampoline page and trapframe, but nothing else from the kernel. The fact that the trampoline page is mapped at the same virtual address in user and kernel page tbales is waht allows uservec to keep executing after changing satp. userret copies the trapframe’s saved user a0 to sscratch in preparation for a later swap with trapframe. from this point on, the only data userret can use is the register contents and the content of the trapframe. Next userret restores saved user registers from the trapframe, does a dinal swap of a0 and sscratch to restore the user a0 and save trapframe for the next trap, and executes sret to return to user space.

### 4.3 callinig system calls

chapter 2 ended with initcode.s invoking the exec system call. let’s look at how the user call makes its way to the exec system call’s implementation in the kernel.

initcode.s palces the arguments for exec in registers a0 and a1, and puts the system call number in a7. system call numbers match the entries in the system calls array, a table of function pointers. The ecall instruction traps into the kernel and causes uservec, usertrap, and then syscall to execute, as we saw above.

syscall retrieves the system call number from the saved a7 in the trapframe and uses it to index into syscalls. For the first system call, a7 contains sys_exec, resulting in a call to the system call implementation function sys_exec.

When sys_exec returns, syscall records its return value in p->trapframe->a0. This will cause the original user-space call to exec() to return that value, since the C calling convention on risc-v places return values in a0. system calls conventionally return negative numbers to indicate errors, and zero or positive numbers for success. If the system call number is invalid, syscall prints an error and returns -1.

### 4.4 system call arguments

system call implementations int the kernel need to find the arguments passed by user code. Because user code calls system call wrapper functions, the arguments are initially where the risc-v c calling convention palces them: in registers. The kernel trap code saves user registers to the current process’s trap frame, where kernel code can find them. The kernel kernel functions argint agaddr, and argfd retrieve the n’th system call argument from the trap frame as an integer, pointer, or a file descriptor. They all call argraw to retrieve the appropriate saved user register.

Some system calls pass pointers as arguments, and the kernel must use those pointers to read or write user memory. The exec system call, for example, passes the kernel an array of pointers referring to string arguments in user space. These pointers pose two chanllenges. First, the user program may be buggy or malicious, and may pass the kernel an invalid pointer or a pointer intended to trick the kernel into accessing kernel memory instead of user memory. Second, the xv6 kernel page tbale mappigns are not the same as the user page tbale mappings, so the kernel cannot use ordinary instructions to load ot store from user-supplied addresses.

The kernel implements functions that safely transfer data to and from user-supplied addresses fetchstr is an example. File system calls such as exec use fetchstr to retrieve string file-name argmuments from user space. fetchstr calls copyinstr to do the hard work.

copyinstr copies up to max bytes to dst from virtual address scrva in the user page tbale pagetable. since pagetbale is not the current page tbale, copyinstr uses walkaddr to look up srcva in pagetabel, yielding physical address pa0. the kernel maps each physical ram address to the corresponding kernel virtual address , so copyinstr can directly copy string bytes form pa0 to dst. walkaddr checks that the user-spplied virtual address is part of the process’s user address space, so programs cannot trick the kernel into reading other memory. A similar function, copyout, copied data from the kernel to a user-supplied address.

### 4.5 traps from kernel space

xv6 configures the CPU trap registers somewaht differently depending on  whether user or kernel code is executing. When the kernel is executing on a CPU, the kernel points stvec to the assembly code at kernelvec. Since xv6 is already in the kernel, kernelvec can rely on satp being set to the kernel page table, and on the stack pointer referring to a valid kernel stack. kernelvec pushes all 32 registers onto the stack, from which it will later restore them so that the interrupted kernel code can resume without disurbance.

kernelvec saves the registers on the stack of the interrupted thread, which makes sense because the register values belong to that thread. This is particularly important if the trap causes a switch to a different thread-in that case the trap will actually return from the stack of the new thread, leaving the inerrupted thread’s saved registers safely on its stack.

kernelvec jumps to kerneltrap after saving registers. Kerneltrap is prepared for two types opf traps: device interrupts and exceptions. It calls devints to check for and handle the former. If the trap is not a device interrupt, it must be an exception, and that is always a fatal error if it occurs in the xv6 kernel; the kernel calls panic and stops executing.

if kerneltrap was called due to a timer interrupt, and a process’s kernel thread is running, kerneltrap calls yield to give other threads a chance to run. At some point one of those thrads will yield, and let our thread its kerneltrap resume again.

When kerneltrap’s work is done, it needs to return to whatever code was interrupted by the trap. Because a yield may have disturbed sepc and the preivous mode in sstatus kerneltrap saves them when it starts. It now restores those control registers and returns to kernelvec. Kernelvec pops the saved registers form the stack and executes sret, which copies sepc to pc and resumes the interrupted kernel code.

It is worth thinking through how the trap return happens is kerneltrap called yield due to a timer interrupt.

xv6 sets a pcu’s stvec to kernelvec when that cpu enters the kernel from user space; you can see this in usertrap. There is a window of time when the kernel has started executing but svec is still set to uservec, and it is crucial that no device interrupts occur during that window. Luckily the risc-v always disables interrupts when it start to take a trap, and xv6 does not enabel them againe until after it sets sevec.

### 4.6 pagefault exception

xv6 response to exceptions is quite boring: if an exception happes in user space, the kernel kills the faulting process. If an exception happeds in the kernel, the kernel panics. Real operating systems often respond in much more interesting ways.

As an example, many kernels use page faults to implement cop-on-write fork. fork causes the child’s initial memory content to be the same as the parent’s at the time of the fork. xv6 implements fork with uvmcopy, which allocates physical memory for the child and copies the parent’s memory into it. It would be more efficient if the child and parent could share the parent’s physical memory. A straightforward implemetation of this would not work, however, since it would cause the parent and child to distrpt each other’s execution with their writes to the shared stack and heap.

parent and child can safely share physical memory by appropriate use of page-tbale permissions and page faults. The CPU raises a page-fault exception when a virtual address is used that has no mapping in the page table, or has a mapping whose pte_v flas is clear, or a mapping whose permission bits forbid the operation being attempted. RISC-V distinguishes three kinds of page faults(when a store instruction can not translate its virtual address), and instruction page faults(when the address in the program counter does not translate). The scause register indicates the type of the page fault and the stval register contains the address that could not be translated.

The basic plan in COW fork is for the parent and child to initially share all physical pages, but for each to map them read-only(with the PTE-W flag clear). Parent and child can read from the shared physical memory. If either writes a given page, the RISC-V CPU raises a page-fault exception. The kernel’s trap handler responds by allocating a new page of physical memory and copying into it the physical page that the faulted address maps to. The kernel changes the relevant PTE in the faulting process’s page tbale to point to the copy and to allow writes as well as reads, and then resumes the faulting process at the instruction that caused the fault. Because the PTE allows writes, the re-executed instruction will now execute without a fault. Copy-on-write requires book-keeping to help decide when physical pages can be freed, since each page can be referenced by a varying number of page tables depending on the history of forks, page faults, execs, and exits. This book-keeping allows an important optimization: if a process incurs a store page fault and the physical page is only referred to from that process’s page tbale, no copy is needed.

copy-on-write makes fork faster, since fork need not copy memroy. Some of the memory will have to be copied later, when written, but it is often the cases that most of the memory never has to be copied. A common example is fork followed by exec: a few pages may be written after the fork, but then the child’s exec releases the bulk of the memory inherited from the parent. Copy-on-write fork eliminates the need to ever copy this memory. Furthermore cow fork is transparent: no modifications to applications are necessary for them to benefit.

The combination of page tables and page faults opens up a wide range of interesting possibilities in addition to cow fork. Another widely-used feature is called lazy allocation, which has two parts, First, when an application asks for more memory by calling sbrk, the kernel notes the increase in size, but does not allocate physical memory and does not create PTEs for the new range of virtual addresses, Second , on a page fault on one those new addresses, the kernel allocates a page of physical memory and maps it into the page tbale. Like COW fork, the kernel can implements lazy allocation transparently to applications.

Since applications often ask for more memory than they need, lazy allocation is a win: the kernel does not have to do any work at all for pages that the application never uses. Furthermoew, if the application is asking to grow the address space by a lot, then sbrk without lazy allocation is expensive: if an application asks for a gigabyte of memory, the kernel has to allocate and zero 262,144 4096-byte pages. Lazy allocation allows this cost to be spread over time. On the other hand, lazy allocation incurs the extra overhead of page faults, which involve a kernel/user transition. Operating systems can reduce this cost by allocating a batch of consecutive pages per page fault instead of on page and by specializing the kernl extry/exit for such page-faults.

Yet another widely-used feature that exploits page faults is demand paging. In exec, xv6 loads all text and data of an application eagerly into memory. Since applications can be large and reading from disk is expensive, this startup cost may be noticeable to users: when the user starts a large application from the shell, it may take a long time before user sees a response. To improve response time, a modern kernel creates the page tbale for the user address space, but marks the PTEs for the pages invalid. On a page fault, the kernel reads the content of the page from disk and map it into transparently to applications.

The programs running on a computer may need more memory than the computer has RAM. To cope gracefully, the operating system may implement paging to disk. The idea is to store only a fraction of user pages in RAM, and to store the rest on disk in a paging area. The kernel marks PTEs that correspond to memory stored in the paging area(and thus not in RAM) as invalid. If an application tries to use one of pages that has been paged out to disk,the application will incur a page fault, and the page must be paged in: the kernel trap handler will allocate a page of physical RAM, read the page from disk into the RAM, and modifu the relevant PTE to point to the RAM.

What happens if a page needs to be paged in, but there is no free physical RAM? in that case, the kernel must first free a physical page by paging it out ot evicting it to the paging area on disk, and marking the PTEs referring to that physical page as invalid. Eviction is expensive, so paging performs best if it is infrequent: if applications use only a subset of their memory pages and the union of the subsets fits in RAM. This property is often referred to as having good locality of reference. As with many virtual memory techniques, kernel usually implement paging to disk in a way that is transparent to applications.

Computers often operate with little or no free physical memory, regardless of how much RAM the hardware provides. For example, cloud providers multiplex many customers on a single machine to use their hardware cost-effectively. As another example, users run many applications on smart phones in a small amount of physical memory. In such setting allocating a page may require first evicting an existing page. Thus, when free physical memory is scarce, allocation is expensive.

Lazy allocation and demand paging are particularly advantagous when free memory is scarce. Eagerly allocating memory in sbrk or exec incurs the extra cost of eviction to make memory availabel. Furthermore, there is a risk that the eager work is wasted, because before the application uses the page, the operating system may have evictted it.

Other features that combine paging and page-fault exceptions include automatically extending stacks and memory-ampped files.

### 4.7 real world

The trampoline and trapframe may seem excessively complex. A driving force is that the RISC-V intentionally does as little as it can when forcing a trap, to allow the possibility of very fast trap handling, which turns out to be important. As a result, the first few instructionsd of the kernel trap handler effectively have to execute in the user environment: the user page tbale, and user register contents. And the trap handler is initially ignorant of useful facts such as identity of the process that’s running or the address of the kernel page table. A solution is possible because risc-v provides protected places in which the kernel can stash away information before entering user space: the sscratch register, and user page table entries that point to kernel memory but are protected by lack of PTE_U. Xv6’s trampoline and trapframe exploit these risc-v features.

The need for special trampoline pages could be eliminated if kernel memory were mapped into every process’s user page tbale(with appropriate PTE permission flags). That would also eliminate the need for a page tbale switch when trapping from user space into the kernl. That in turn would allow system call implementations in the kernel to take advantage of the current process’s user memory being mapped, allowing kernel code to directly dereference user pointers. Many operating systems have used these ideas to increase efficiency. Xv6 avoids them in order to reduce the chances of security bugs in the kernel due to inadvertent use of user pointers, and to reduce some complexity that would be required to ensure that user and kernel virtual addresses do not overlap.

production operating systems implement copy-on-write fork, lazy allocation, demand paging, paging to disk, memroy-mapped files, etc.

## chapter 5 interrupts and device drivers

a driver is the code in an operating system that manages a particular device: it configures the device hardware, tells the device to perform operations, handles the resulting interrupts, and interacts with processes that may be waiting for IO from the device. Driver code can be tricky because a driver executes concurrently with the device that it manages. In addition, the driver must understand the device’s hardware interface, which can be complex and poorly documented.

devices that need attention from the operating system can usually be configured to generate interrupts, which are one type of trap. The kernel trap handling code recognizes when a device has raised an interrupt and calls the driver’s interrupt handler; in xv6, this dipatch happeds in devintr.

Many device drivers execute code in tow contexts: a top hald that runs in a process’s kernel thread, and a bottom half that executes at interrupt time. The top half is called via system calls such as read and write that want the device to perform IO. This code may ask the hardware to start an operation; Then the code waits for the operation to complete. eventually the device completes the operation and raises an interrupt. The driver’s interrupt handler, acting as the bottom half, figures out what operation has completed, wakes up a waiting process if appropriate, and tells the hardware to start work on any waiting next operation.

### 5.1 console input

The condole driver is a simple illustration of driver structure. The console driver accepts characters typed by a human, via the uart serial-port hardware attached to the risc-v. the console driver accumulates a line of input at a time, processing special input characters such as backspace and control-u. User processes, such as the shell use the read system call to fetch lines of input from the console. When you type input to xv6 in QEMU,you keystrokes are delivered to xv6 by way of qemu’s simulated uart hardware.

the uart hardware that the driver talks to is a 16550 chip emulated by qemu. On a real computer, a 16550 would manage an rs232 serial link connecting to a terminal or other computer. When running qemu, it is connected to your keyboard an display.

the uart hardware appears to software as a set of memory-mapped control registers. That is, there are some physical addresses that risc-v hardware connects to the uart device, so that loads and stores interact with the device hardware rather than RAM. The memory-mapped addresses for the uart start at 0x10000000, or uart0. There are a handful of uart control registers, each the width of a byte. Their offfsets from uart0 are defined in uart.c. For example, the lsr register contain bits that indicate whether inpu characters are waiting to be read by the software. These charaters are availabel for reading from the RHR register. Each time one is read, the UART hardware deletes it from an internal fifo of waiting characters, and clears the ready bit in lsr when the fifo is empty. The Uart transmit hardware is largely independent of the receive hardware; if software writes a byte to the THR, the uart transimit that byte.

xv6’s main calls consoleinit to initialize the uart hardware. This code configures the uart to generate a receive interrupt when the uart receives each byte of input, and a transmit complete interrupt each time the uart finishes sending a byte of output.

The xv6 shell reads from the console by way of a file descriptor opened by init.c. calls to the read system call make their way through the kernel to consoleread. consoleread waits for input to arrive(via interrupts) anb be buffered in cons.buf, copies the input to user space, and returns to the user process. If the user has not tyuped a full line yet, any reading processes will wait in the sleep call.

when the user types a character, the uart hardware asks the risc-v to raise an interrupt, which activates xv6’s trap handler to discover that the interrpt is from an external device. Then it asks a hardware unit called the PLIC to tell it which device interrupted. If it was the uart, devintr calls uartintr.

Uartintr reads any waiting input characters from the uart hardware and hands them to consoleintr; it does not wait for characters, since future input will raise a new interrupt. The job of consoleintr is to accumulate input characters in conss.buf until a whole line arrives. consoleintr treats backspace and a few other characters specially. When a newline arrives, consoleintr wakes up a waiting consoleread.

Once woken, consoleread will observe a full line in cons.buf, copy it to user space, and return to user space.

### 5.2 console output

A write system call on a file descriptor connected to the console eventually arrives at uartputs. The device driver maintains an output buffer so that writing processes do not have to wait for the uart to finish sending; instead, uartputs appends each character to the buffer, calls uartstart to start the device transmitting, and returns. The only situation in which uartputc waits is if the buffer is already full.

each time the uart finishes sending a byte, it generates an interrupt. uartintr calls uartstart, whihc checks that the device really has finished sending, and hands the device the next buffered output character. Thus if a process writes multiple bytes to the console, typically the first byte will be sent by uartputsc’s call to uartstart, and the remaining buffered bytes will be sent by uartstart calls from uartintr as transmit complete interrupts arrive.

A general pattern to note is the decoupline of device activity from process activity via buffering and interrupts. The console driver can process input even when no process is waiting to read it; a subsequent read will see the input. Similarly, processes can send output without having to wait for the device. This decoupline can increase performance by allowing processes to execute concurrently with device io, and is particularly important when the device is slow or needs immediate attention. This idea is sometimes called IO concurrency.

### 5.3 concurrency in drivers

You may have noticed calls to acquire in consoleread and in consoleintr. These calls acquire a lock, which protects the console driver’s data structures from concurrent access. There are three concurrency dangers here: two processes on different cpus might call consoleread at the same time; the hardware might ask a cpu to deliver a console interrupt while that cpu is already executing inside consoleread; and the hardware might deliver a console interrupt on a different cpu while consoleread is executing. These dangers may result in race conditions or deadlocks.

Another way in which concurrency requires care in drivers is that one process may be waiting for input from a device, but the interrupt signaling arrival of the input may arrive when a different process is running. Thus interrupt handlers are not allowed to think about the process or code that they have interrupted. For example, and interrupt handler cannot safely call copyout with the current process’s page table. Interrupt handlers typically do relatively little work, and wake up top-half code to do the rest.

### 5.4 timer interrupts

xv6 uses timer intrerupts to maintain its clock and to enable it to switch among compute-bound processes; the yield calls in usertrap and kerneltrap cause this switching. Timer interrupts come from clock hardware attached to each RISC-V CPU. xv6 programs this clock hardware to interrupt each cpu periodically.

RISC-V requires that timer interrupts be taken in machine mode, not supervisor mode. RISC-V machine mode executes without paging, and with a separate set of control registers, so it is not practical to run ordinary xv6 kernel code in machine mode. As a result, xv6 handles timer interrupts completely separately from the trap mechanism laid out above.

code executed in machine mode in start.c, before main, sets up receive timer interrupts. part of the job is to program the cline hardware to generate an interrupt after a certain delay. Another part is to set up da scratch area, analogous to the trapframe, to help the timer interrupt handler save registers and the address of the clint registers. Finally, start sets mtvec to timervec and enables timer interrupts.

a timer interrupt can occur at any point when user or kernel code is executing; there is no way for the kernel to disable timer interrupts during critical operations. Thus the timer interrupt handler must do its job in a way guaranteed not to disturb interrupted kernel code. The basic strategy is for the handler to ask the risc-v to raise a software interrupt and imnmediately return. The risc-v delivers software interrupts  to the kernel with the ordinary trap mechanism, and allows the kernel to disable them. The code to handle the software interrupt generated by a timer interrupt can be seen in devintr.

The machine mode timer interrupt handler is timervec. It saves a few registers in the scratch area prepared by start, tells the clint when to generate the next timer interrupt, asks the risc-v to raise a sofeware interrupt, restores registers, and returns.

### 5.5 real world

xv6 allows device and timer interrupts while executing in the kernel,as well as when executing user programs. Timer interupts force a thread switch from the timer interrupt handler, even when executing in the kernel. The ability to time-slice the cpu fairly among kernel threads it useful if kernel thread sometimes spend a lot of time computing, without returning to user space. However, the need for kernel code to be mindful that it might be suspended and later resume on a different cpu is the source of some complexity in xv6. The kernel could be made somewhat simpler if device and timer interrupts only occurred while executing user code.

## chapter 6 locking

most kernels, including xv6, interleave the execution of multiple activities. One source of interleaving is multiprocessor hardware: computers with multiple CPUs executing independently, such as xv6’s risc-v. These multiple CPUs share physical RAM, and xv6 exploits the sharing to maintain data structures that all CPUs read and write. THis sharing raises the possibility of one CPU reading a data structure while another CPU is mid-way through updating is, or even multiple CPUs updating the same data simulataneously; without careful design such parallel access is likely to yield incorrect results or a broken data structure. Even on a uniprocessor, the kernel may switch the CPU among a number of threads, causing their execution to be interleaved. Finally, a device interrupt handler that modifies the same data as some interruptible code could damage the data if the interrupt occurs at just the wrong time. The word concurrency refers to situations in which multiple instructin streams are interleaved, due to multiprocessor parallelism, thread switching, or interrupts.

Kernels are full of concurrently-accessed data. For example, two CPUs could simultaneously call kalloc, thereby concurrently popping from the head of the free list. Kernel designers like to allow for lots of concurrency, since it can yield increased performance though parallelism, and increased responsivenss. However, as a result kernel designers spend a lot of effort convincing themselves of correctness despite such concurrency. There are many ways to arrive at correct code, some easier to reason about than others, Straties aimed at correctness under concurrency, and abstractions that support them, are called concurrency control techniques.

xv6 uses a number of concurrency control techniques, depending on the situation; many more are possible. This chapter focuses on a widely used technique: the lock. A lock provides mutual exclusion, ensuring that only one CPU at a time can hould the lock. If the programmer associates a lock with each shared data item, and the code always holds the associated lock when using an item, then the item will be used by only one CPU at a time. In this situation, we say that the lock protects the data item. although locks are an easy-to-understand concurrency control mechanism, the downside of locks is that they can kill performance, because they serialize oncurrent operations.

The rest of this chapter explains why xv6 needs locks, how xv6 implements them, and how it uses them.

### 6.1 race conditions

as an example of why we need locks, consider two processes calling wait on two different CPUs. wait frees the child’s memory. Thus on each CPU, the kernel will call kfree to free the children’s pages. the kernel allocator manitains a linked list: kalloc() pops a page of memory from a list of free pages, and kfree() pushs a page onto the free list. For best performance, we might hope that the kfrees of the two parent processes would execute in parallel without either having to wait for the other, but this would not be correct given xv6’s kfree implementation.

When we say that a lock protects data, we really mean that the lock protects some collection of invariants that apply to the data. Invariants are properties of data structure that are maintained across operations. Typically, an operations’s correct behavior depends on the invariants but must reestablist them before finishing. For example, in the linked list case, the invariant is that list points at the first element in the list and that each elements’s next field at the next element. The implementation of push violates this invariant temporarily. proper use of a lock ensures that only one cpu at a time can operate on the data structure in the critical section, so that no CPU will execute a data structure operation when the data structure’s invariants do not hold.

You can think of a lock as serializing concurrent critical sections so that they run one at a time, and thus preserve invariants. You can alos think of critical sections guarded by the same lock as being atomic with respect to each other so that each sees only the complete set of changes from earlier critical sections, and never sees partially-completed updates.

althrough correct use of locks can make incorrect code correct, locks limit performance. For example, if two processes call kfree concurrently, the locks will serialize the two calls, and we obtain no benefit from running them on differently, the locks will serialize the two calls, and we obtain no benefit from running them on different CPUs. We say that multiple processes conflict if they want the same lock at the same time, or that the lock experiences contention. A major chanllenge in kernel design is to avoid lock contention. Xv6 does little of that, but sophisticated kernels organize data structures and algorithms specifically to avoid lock contention. In the list example, a kernel may maintain a free list per CPU and only touch another CPU’s free list if the CPU’s list is empty and it must steal memory from another CPU. Other use cases may require more complicated designs.

The placement of locks is also important for performance. For example, it would be correct to move acquire earlier in push: it is fine to move the call to acquire up to before line 13. This may reduce performance because then the calls to malloc are also serialized. The section using locks below provides some guidelines for where to insert acquire and release invocations.

### 6.2 locks

xv6 has two types pf locks: spinlocks and sleep-locks. We’ll start with spinlocks. xv6 represents a spinlock as a struct spinlock. The important field in the structure is locked, a word that is zero when the lock is aviilable and non-zero when it is held. Logically, xv6 should acquire a lock by executing code like:

```c
void
acquire(struct spinlock *lk)  /* does not word */
{
    for(;;) {
        if(lk->locked == 0){
            lk->locked = 1;
            break;
        }
    }
}
```

Unfortunately, this implementation does not guarantee mutual exclusion on a multiprocessor. It could happed that two CPUs simulataneously reach line 25, see that lk->locked is zero, and then both grab the lock by executing like 26. at this point, two different CPUs hold the lock, which violates the mutual exclusion property. What we need is a way to make lines 25 and 26 execute as an atomic step.

Because locks are widely used, multi-core processors usually provide instructions that implement an atomic version of line 25 and 26. On the risc-v this instructin is amoswap r,a. amoswap reads the value at the memory address a, writes the contents of register r to that address, and puts the value it read into r. That is, it swaps the contents of the register and the memory address. It performs this sequence atomically, using special hardware to prevent any other CPU from using the memory address between the read and the write.

Xv6’s acquire uses the portable C library call __sync_lock_test_and_set, shich boils down to the smoswap instruction; the return value is the old contents of lk->locked. The acqueire function wraps the swap in a loop, retrying until it has acquired the lock. Each iteration swaps one into lk->locked and checks the previous value; If the previous value is zero, then we have acquired the lock, and the swap will have set lk->locked to one. if the previous value is one, then some other CPU holds the lock, and the fact that we aromically swapped on int lk->locked did not change its value.

Once the lock is acquired, acquire records, for debugging, the CPU that acquired the lock. the lk->cpu field is protected by the lock and must only be changed while holding the lcok.

The function release is the oppsite of acquire: it clears the lk->cpu field and then releases the lock. conceptually, the release just requires assigning zero to lk->locked. The C standard allows compilers to implement an assignment with multiple store instructions, so a C assignment might be non-stomic with respect to concurrent code. Instead, release used the C library function __sync_lock_release that performs an atomic assignment. This function also boils down to a risc-v amoswap instruction.

### 6.3 using lock

xv6 uses locks in many places to avoid race conditins. As decribed above, kalloc and free form a good example. 

A hard part about using locks is deciding how many locks to use and which data and invariants each lock should protect. There are a few basic principle. First, any time a variable can be written by one CPU at the same time that another CPU can read or write it, a lock should be used to keep the two operations from overlapping. Second, remember that locks protect invariants: if an invariant involves multiple memory locations, typically all of them need to be protected by a single lock to ensure the invariant is maintained.

The ruls above say when locks are necessary but say nothing about when locks are unnecessary, and it is important for efficiency not to lock too much, because locks reduce paralleism. If paralleism is not important for efficiency not to lock too much, because locks reduce paralleism. If parallelism is not important, then one could arrange to have only a single thread and not worry about locks. A simple kernel can do this on a multiprocessor by having a single lock that must be acquired on entering the kernel and release on exiting the kernel(through system calls such as pipe reads or wait would pose a probelm). Many uniprocessor operating systems have been converted to run on multiprocvessors using this approach, sometimes called a big kernel lock, but the approach sacrifices parallelism: only one CPU can execute in the kernel at a time. If the kernel does any heavy computation, it would be more efficient ot use a larger set of more fine-grained locks, so that the kernel could execute on multiple CPUs simultaneously.

As an example of coarse-grained locking, xv6’s kalloc.c allocator has a single free list protected by a single lock. If multiple processes on different CPUs try to allocate pages at the same time, each will have to wait for its turn by spinning in acquire. Spinning reduces performance, since it is not useful work. if contention for the lock wasted a signigicant fraction of CPU time, perhaps performance could be improved by changing the allocator design to have multiple free lists, each with its own lock, to allow truly parallel allocation.

As an example of fine-grained locking, xv6 has a separate lock for each fine, so that processes that manipulate different files can often procedd without waiting for each others’ locks. the file locking schewme could be made even more fine-grained if one wanted to allow processes to simulataneously write different ateas of the same file. Ultimately lock granularity decisions need to be driven by performance measurements as well as complexity consiferations.

as subsequent chapters explain each part of xv6, they will mention examples of xv6’s use of locks to deal with concurrency.

### 6.4 deadlock and lock ordering

xv6 has many lock-order chains of length two involving per-process locks(the lock in each struct proc) due to the way that sleep works. For example, consoleintr is the interrupt routine which handles typed characters. when a newline arrives, any process that is waiting for console input should be woken up. To do this, consoleintr holds cons.lock while calling wakeup, whihc acquires the waiting process’s lock in order to wake it up. I consequence, the global deadlock-avoiding lock order includes the ruls that cons.lock must be acquired before any process lock. The fine system code contains xv6’s longest lock chains. For example, creating a file requires simulataneously holding a lock on the directory, a lock on the new file’s inode, a lock on a disk block buffer, the disk driver’s vdisk_lock, and the calling process’s p->lock. To avoid deadlock, file-system code always acquires locks in the order mentioned in the previous sentence.



















































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





