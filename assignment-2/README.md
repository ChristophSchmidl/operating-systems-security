# Assignment 2

## 1 - Compile and load your own Linux kernel module


Login to your (Kali) Linux system as a **root** user and compile the program **cr4.c**

```
#include <stdio.h>

void main() {
	unsigned long long result;
	/* unsigned long result; (for 32-bit OS) */
	__asm__("movq %%cr4, %%rax\n" : "=a"(result));
	/*__asm__("mov %%cr4, %%eax\n" : "=a"(result)); (for 32-bit OS)*/
	printf("Value of CR4 = %llx\n", result);
	}
```

With the command line: `# gcc -o cr4 cr4.c`. 
Notice that executing will result in an exception: `# ./cr4` Segmentation fault.
Using a debugger, we can quickly pinpoint what the problem is. Start the debugger in assembly mode: `# gdb -ex "layout asm" ./cr4`
and execute it using the following **GDB** instruction: `# run`:

```
   ┌───────────────────────────────────────────────────────────────────────────┐
   │0x55555555464a <main>           push   %rbp                                │
   │0x55555555464b <main+1>         mov    %rsp,%rbp                           │
   │0x55555555464e <main+4>         sub    $0x10,%rsp                          │
  >│0x555555554652 <main+8>         mov    %cr4,%rax                           │
   │0x555555554655 <main+11>        mov    %rax,-0x8(%rbp)                     │
   │0x555555554659 <main+15>        mov    -0x8(%rbp),%rax                     │
   │0x55555555465d <main+19>        mov    %rax,%rsi                           │
   │0x555555554660 <main+22>        lea    0x9d(%rip),%rdi        # 0x555555554│
   │0x555555554667 <main+29>        mov    $0x0,%eax                           │
   │0x55555555466c <main+34>        callq  0x555555554520 <printf@plt>         │
   │0x555555554671 <main+39>        nop                                        │
   │0x555555554672 <main+40>        leaveq                                     │
   │0x555555554673 <main+41>        retq                                       │
   └───────────────────────────────────────────────────────────────────────────┘

(gdb) run
Starting program: /root/operating_systems_security/assignment-2/cr4

Program received signal SIGSEGV, Segmentation fault.
0x0000555555554652 in main ()

```

* Crash course regarding inline assembly in C: [Link](https://0xax.gitbooks.io/linux-insides/content/Theory/asm.html)
* [x64 Cheat Sheet](https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf)
* [Inline Assembly](https://0xax.gitbooks.io/linux-insides/content/Theory/asm.html)
* [GCC-Inline-Assembly-HOWTO](http://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html)
* [A Tiny Guide to GCC Inline Assembly](http://ericw.ca/notes/a-tiny-guide-to-gcc-inline-assembly.html)
* [Extended Asm](https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html)






* a) Figure out where the register **CR4** is used for and report back why you think it should be be accessible in user [mode](http://en.wikipedia.org/wiki/Control_register).

	* Answer: CR4 belongs to so called "Control registers" which can mainly be found in x86 series. There are 8 registers in total, namely CR0-CR7. Wikipedia describes the CR4 register as follows: *Used in protected mode to control operations such as virtual-8086 support, enabling I/O breakpoints, page size extension and machine check exceptions.*[Source](https://en.wikipedia.org/wiki/Control_register#CR4)
	The CR4 register provides 19 different bits which enables/disables different functionality which might be dangerous to access in user mode. CR4 contains bits which handle different settings regarding page size extensions and the ability to share address translations between address spaces. This functionality refers to bit 7 and I assume that would make it possible for different processes to share address spaces and therefore would be a security flaw.
	

* b) Figure out which exact assembly instruction of **cr4.c** triggers the segmentation fault and briefly write down what it tries to do.
	
	* Answer: You can use asm, __asm or __asm__ for inline assmbly in C. Some of them are used over the other one to avoid namespace conflicts. But in the end they all make inline assembly possible. movq (assuming you're talking about x86) is a move of a quadword (64-bit value). GCC inline assembly uses %0, %1, %2, etc. to refer to input and output operands. That means you need to use two %% for real registers. If there is a colon : after the string, then it is an extended asm, and %% escapes the percent. movq %%cr4, %%rax moves the contents of the cr4 register into the rax register. Modern x86_64 processors have 64-bit registers that can be used in backward-compatible fashion as 32-bit registers, 16-bit registers and even 8-bit registers, for example: rax (64 bits), eax (32 bits), ax (16 bits), ah (8 bits), al (8 bits). By convention, %rax​ is used to store a function’s return value, if it exists and is no more than 64 bits long. (Larger return types like structs are returned using the stack.) The part after the first colon is called the Outputoperand and is a special parameter in extended Asm. The "=a" part is called a constraint. Output constraints must begin with either ‘=’ (a variable overwriting an existing value) or ‘+’ (when reading and writing). ‘d’, ‘a’ and ‘f’ are defined on the 68000/68020 to stand for data, address and floating point registers. The segmentation fault is triggered by this instruction `0x555555554652 <main+8>         mov    %cr4,%rax` which refers to this line in the inline assembly: `__asm__("movq %%cr4, %%rax\n" : "=a"(result));` This line tries to move the contents of the cr4 register into the rax register and later on is trying to save the address of the rax register into the result variable. Because the cr4 register is not accessable in user mode, a segmentation fault occurs.

* c) Follow the *How to Write Your Own Linux Kernel Module with a Simple Example* guide hosted at this [website](http://www.thegeekstuff.com/2013/07/write-linux-kernel-module/) and try to reproduce their results. You should be able to see your kernel module output with the following command: `dmesg | tail -10`.

	* Answer:

* d) If your kernel module is working correctly, try to adjust the kernel module to read out the exact same **CR4** register. Hand in the source-code of your kernel module together with a Makefile to build it and report back which value the **CR4** in your (Kali) Linux system has.

	* Answer:


## 2 - Return to libc

* a) Run the attack and obtain a root shell (you might want to try first without suid-root to not allow too much disaster if things go wrong). Now automate this attack in a bash script. The bash script should be robust, i.e., it should handle the case that offsets in libc are different. You can test this by running the script on a different machine, e.g., on *lilo.science.ru.nl*. **Submit this script**
	
	* Answer:

* b) The attack does not work against the "original" version of the program in Lynn's tutorial with a buffer size of 64. Use gdb to find out why not. In particular, answer the following questions:
	
	* Does the attacked program jump (return) to the `pop %rdi, retq` gadget? If not, why not?

		* Answer:

	* Does the attacked program put the right address into `rdi`? If not, why not?
	
		* Answer:

	* Does the attacked program jump (return) to *system*? If not, why not?
	
		* Answer:

	* Does the attacked program issue the correct syscall? If so, which one? If not, why not?
	
		* Answer:

	* Summarize and explain why the attack does not work.
	
		* Answer:


Note: It is of course perfectly fine to compile the program with the `-g` flag.
Note: Addresses (for example, of *name*) are probably slightly different when running the program in gdb.

* c) Can you think about a way to make the attack work with a buffer of size 64? (Hint: Where else can you find the string `/bin/sh` or similar?)

	* Answer:

* d) **Bonus task:** Make the attack work against a buffer of size 64 and against a buffer of size 4.

	* Answer:							