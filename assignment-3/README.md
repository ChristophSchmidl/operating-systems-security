# Assignment 3

## 1 - Bypassing ASLR (Address Space Layout Randomization)

In this exercise, you will learn how to exploit a buffer overflow, bypassing ASLR on 64-bit Linux binaries. You are strongly encouraged to read this [article](https://blog.techorganic.com/2016/03/18/64-bit-linux-stack-smashing-tutorial-part-3/) before proceeding with the assignment.

Reading list:

* [64-bit Linux stack smashing tutorial: Part 1](https://blog.techorganic.com/2015/04/10/64-bit-linux-stack-smashing-tutorial-part-1/)
* [64-bit Linux stack smashing tutorial: Part 2](https://blog.techorganic.com/2015/04/21/64-bit-linux-stack-smashing-tutorial-part-2/)
* [64-bit Linux stack smashing tutorial: Part 3](https://blog.techorganic.com/2016/03/18/64-bit-linux-stack-smashing-tutorial-part-3/)
* [PLT and GOT - the key to code sharing and dynamic libraries](https://www.technovelty.org/linux/plt-and-got-the-key-to-code-sharing-and-dynamic-libraries.html)


Login your (Kali) Linux system and download the source [code](https://www.cs.ru.nl/~vmoonsamy/teaching/ossec2016/a3_sourcecode.zip).

Compile *vuln.c* using the following command (**Hint**: add *-no-pie* on Ubuntu 16.10 and above to avoid the executable becoming ASLR'ed):

`# gcc -fno-stack-protector -no-pie vuln.c -o vuln`

Enable ASLR:

`# echo 2 > /proc/sys/kernel/randomize_va_space`


**Objectives**

* a) Find the address of **read@plt** and **system@plt** by disassembling *vuln* using **objdump**.
	* Answer:
		```
		# objdump -d -f ./vuln 

		...

		Disassembly of section .plt:

		0000000000000630 <.plt>:
		 630:	ff 35 d2 09 20 00    	pushq  0x2009d2(%rip)        # 201008 <_GLOBAL_OFFSET_TABLE_+0x8>
		 636:	ff 25 d4 09 20 00    	jmpq   *0x2009d4(%rip)        # 201010 <_GLOBAL_OFFSET_TABLE_+0x10>
		 63c:	0f 1f 40 00          	nopl   0x0(%rax)

		0000000000000640 <write@plt>:
		 640:	ff 25 d2 09 20 00    	jmpq   *0x2009d2(%rip)        # 201018 <write@GLIBC_2.2.5>
		 646:	68 00 00 00 00       	pushq  $0x0
		 64b:	e9 e0 ff ff ff       	jmpq   630 <.plt>

		0000000000000650 <setbuf@plt>:
		 650:	ff 25 ca 09 20 00    	jmpq   *0x2009ca(%rip)        # 201020 <setbuf@GLIBC_2.2.5>
		 656:	68 01 00 00 00       	pushq  $0x1
		 65b:	e9 d0 ff ff ff       	jmpq   630 <.plt>

		0000000000000660 <system@plt>:
		 660:	ff 25 c2 09 20 00    	jmpq   *0x2009c2(%rip)        # 201028 <system@GLIBC_2.2.5>
		 666:	68 02 00 00 00       	pushq  $0x2
		 66b:	e9 c0 ff ff ff       	jmpq   630 <.plt>

		0000000000000670 <printf@plt>:
		 670:	ff 25 ba 09 20 00    	jmpq   *0x2009ba(%rip)        # 201030 <printf@GLIBC_2.2.5>
		 676:	68 03 00 00 00       	pushq  $0x3
		 67b:	e9 b0 ff ff ff       	jmpq   630 <.plt>

		0000000000000680 <read@plt>:
		 680:	ff 25 b2 09 20 00    	jmpq   *0x2009b2(%rip)        # 201038 <read@GLIBC_2.2.5>
		 686:	68 04 00 00 00       	pushq  $0x4
		 68b:	e9 a0 ff ff ff       	jmpq   630 <.plt>

		 ...

		```

		 In the read@plt section we can see that the value to be returned is loaded from an offset of 0x2009b2 from the current %rip (instruction pointer register for 64bit. 32bit = eip). Therefore, the address is 0x201038.
		 In the system@plt section we can see that the value to be returned is loaded from an offset of 0x2009c2 from the current $rip (instruction pointer register for 64bit. 32bit = eip). Therefore, the address is 0x201028

* b) In *vuln.c*, a ROP gadget is provided in the function helper.
	* i) What does the gadget do and what can it be used for?
		* Answer:

	* ii) Find the offset of the gadget in the executable.
		* Answer:

* c) Use **readelf** to find writable memory that can be used to store 8 bytes of payload. Give the memory address and explain why you picked this address.
	* Answer:

* d) Locate *payload.py* in the source code folder you downloaded. The exploit is split into two stages - Stage 1 and Stage 2. Explain for each stage what is being done. Run the volnerable program as a server: `# socat TCP-LISTEN:3333,reuseaddr,fork EXEC:./vuln`. This way you can send input using a socket on port 3333.
	* Answer:

* e) Fill in the memory addresses you found before in *exploit.py* and run it in a separate terminal. Confirm that you can successfully get a shell by providing a screenshot (this should be the case if you entered the correct addresses) and explain in detail why.
	* Answer:


## 2 - Self-replicating code

* Write a (small) C programm that prints its own source-code.
	* Answer:
		```
		#include <stdio.h>
		int main(){char*c="#include <stdio.h>%cint main(){char*c=%c%s%c;printf(c,10,34,c,34,10);return 0;}%c";printf(c,10,34,c,34,10);return 0;}
		```



## 3 - Covert channels

An operating system tries to avoid information leakage between processes which are executed by different users. However, it is not always capable of identifying suspicious behaviour, especially if the processes use generic information leakage channels such as:

* Existence of a file
* File attributes
* CPU usage
* Temperature sensor
* "Disk full" errors

**Objectives**

* a) Write two (simple) programs that communicate messages to each other using a covert information leakage channel that is not (inherently) identified by the operating system as communication channel between processes. Hand in the source-code and explain how it should be installed/used.
	* Anwser:

* b) Execute the programs under two different (**non**-root) users and let it communicate to one another. Then, open the log files of your (Kali) Linux system and see if the communication leaves any visible trace (such as unusual and suspicious errors). Explain why your method is undetectable or how it could be optimized to avoid detection by operating systems that perform more advanced monitoring.
	* Answer:	
