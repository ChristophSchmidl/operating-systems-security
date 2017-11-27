# Assignment 2

## 1 - Compile and load your own Linux kernel module


* a) Figure out where the register **CR4** is used for and report back why you think it should be be accessible in user [mode](http://en.wikipedia.org/wiki/Control_register).

	* Answer:

* b) Figure out which exact assembly instruction of **cr4.c** triggers the segmentation fault and briefly write down what it tries to do.
	
	* Answer: 

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