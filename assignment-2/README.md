# Assignment 2

## 1 - Compile and load your own Linux kernel module


* a) Figure out where the register **CR4** is used for and report back why you think it should be be accessible in user mode.

	* Answer:

* b) Figure out which exact assembly instruction of **cr4.c** triggers the segmentation fault and briefly write down what it tries to do.
	
	* Answer: 

* c) Follow the *How to Write Your Own Linux Kernel Module with a Simple Example* guide hosted at this [website](http://www.thegeekstuff.com/2013/07/write-linux-kernel-module/) and try to reproduce their results. You should be able to see your kernel module output with the following command: `dmesg | tail -10`.

	* Answer:

* d) If your kernel module is working correctly, try to adjust the kernel module to read out the exact same **CR4** register. Hand in the source-code of your kernel module together with a Makefile to build it and report back which value the **CR4** in your (Kali) Linux system has.	

