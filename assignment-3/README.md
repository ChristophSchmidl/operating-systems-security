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
	* Answer: I saved the entire output of the following command into the file *objdump_output.txt*.
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

	```
	void helper() {
    	asm("pop %rdi; pop %rsi; pop %rdx; ret");
	}
	```

	* i) What does the gadget do and what can it be used for?
		* Answer: This gadget pops three elements from the top of the stack and saves them into the registers %rdi, %rsi and %rdx and later on adjusting the current instruction pointer. These registers are used for passing integers or pointers and I assume that this gadget could be used to invoke another function which takes three parameters of either integers or pointers.

	* ii) Find the offset of the gadget in the executable.
		* Answer:

		The important part can be found by looking into the output of the objdump command that was used before:

		```
		00000000000007aa <helper>:
		 7aa:	55                   	push   %rbp
		 7ab:	48 89 e5             	mov    %rsp,%rbp
		 7ae:	5f                   	pop    %rdi
		 7af:	5e                   	pop    %rsi
		 7b0:	5a                   	pop    %rdx
		 7b1:	c3                   	retq   
		 7b2:	90                   	nop
		 7b3:	5d                   	pop    %rbp
		 7b4:	c3                   	retq  
		```

		The offset of the gadget is either 00000000000007aa or 00000000000007ae.

* c) Use **readelf** to find writable memory that can be used to store 8 bytes of payload. Give the memory address and explain why you picked this address.
	* Answer:

	```
	# readelf -S vuln
	There are 30 section headers, starting at offset 0x1a98:

	Section Headers:
	  [Nr] Name              Type             Address           Offset
	       Size              EntSize          Flags  Link  Info  Align
	  [ 0]                   NULL             0000000000000000  00000000
	       0000000000000000  0000000000000000           0     0     0
	  [ 1] .interp           PROGBITS         0000000000000238  00000238
	       000000000000001c  0000000000000000   A       0     0     1
	  [ 2] .note.ABI-tag     NOTE             0000000000000254  00000254
	       0000000000000020  0000000000000000   A       0     0     4
	  [ 3] .note.gnu.build-i NOTE             0000000000000274  00000274
	       0000000000000024  0000000000000000   A       0     0     4
	  [ 4] .gnu.hash         GNU_HASH         0000000000000298  00000298
	       0000000000000024  0000000000000000   A       5     0     8
	  [ 5] .dynsym           DYNSYM           00000000000002c0  000002c0
	       0000000000000120  0000000000000018   A       6     1     8
	  [ 6] .dynstr           STRTAB           00000000000003e0  000003e0
	       00000000000000a4  0000000000000000   A       0     0     1
	  [ 7] .gnu.version      VERSYM           0000000000000484  00000484
	       0000000000000018  0000000000000002   A       5     0     2
	  [ 8] .gnu.version_r    VERNEED          00000000000004a0  000004a0
	       0000000000000020  0000000000000000   A       6     1     8
	  [ 9] .rela.dyn         RELA             00000000000004c0  000004c0
	       00000000000000d8  0000000000000018   A       5     0     8
	  [10] .rela.plt         RELA             0000000000000598  00000598
	       0000000000000078  0000000000000018  AI       5    23     8
	  [11] .init             PROGBITS         0000000000000610  00000610
	       0000000000000017  0000000000000000  AX       0     0     4
	  [12] .plt              PROGBITS         0000000000000630  00000630
	       0000000000000060  0000000000000010  AX       0     0     16
	  [13] .plt.got          PROGBITS         0000000000000690  00000690
	       0000000000000008  0000000000000008  AX       0     0     8
	  [14] .text             PROGBITS         00000000000006a0  000006a0
	       0000000000000232  0000000000000000  AX       0     0     16
	  [15] .fini             PROGBITS         00000000000008d4  000008d4
	       0000000000000009  0000000000000000  AX       0     0     4
	  [16] .rodata           PROGBITS         00000000000008e0  000008e0
	       0000000000000024  0000000000000000   A       0     0     4
	  [17] .eh_frame_hdr     PROGBITS         0000000000000904  00000904
	       000000000000004c  0000000000000000   A       0     0     4
	  [18] .eh_frame         PROGBITS         0000000000000950  00000950
	       0000000000000148  0000000000000000   A       0     0     8
	  [19] .init_array       INIT_ARRAY       0000000000200de8  00000de8
	       0000000000000008  0000000000000008  WA       0     0     8
	  [20] .fini_array       FINI_ARRAY       0000000000200df0  00000df0
	       0000000000000008  0000000000000008  WA       0     0     8
	  [21] .dynamic          DYNAMIC          0000000000200df8  00000df8
	       00000000000001e0  0000000000000010  WA       6     0     8
	  [22] .got              PROGBITS         0000000000200fd8  00000fd8
	       0000000000000028  0000000000000008  WA       0     0     8
	  [23] .got.plt          PROGBITS         0000000000201000  00001000
	       0000000000000040  0000000000000008  WA       0     0     8
	  [24] .data             PROGBITS         0000000000201040  00001040
	       0000000000000010  0000000000000000  WA       0     0     8
	  [25] .bss              NOBITS           0000000000201050  00001050
	       0000000000000010  0000000000000000  WA       0     0     8
	  [26] .comment          PROGBITS         0000000000000000  00001050
	       0000000000000026  0000000000000001  MS       0     0     1
	  [27] .symtab           SYMTAB           0000000000000000  00001078
	       00000000000006a8  0000000000000018          28    44     8
	  [28] .strtab           STRTAB           0000000000000000  00001720
	       0000000000000271  0000000000000000           0     0     1
	  [29] .shstrtab         STRTAB           0000000000000000  00001991
	       0000000000000107  0000000000000000           0     0     1
	Key to Flags:
	  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
	  L (link order), O (extra OS processing required), G (group), T (TLS),
	  C (compressed), x (unknown), o (OS specific), E (exclude),
	  l (large), p (processor specific)

	```

	The following entry looks like a good candidate because it is both writable and allocatable (is that even a word?!):

	```
	[24] .data             PROGBITS         0000000000201040  00001040
        0000000000000010  0000000000000000  WA       0     0     8
    ```

    The actual address is therefore 0x201040.

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
