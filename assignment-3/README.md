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
		# objdump --disassemble ./vuln | grep read@plt
		0000000000400560 <read@plt>:
  		400682:	e8 d9 fe ff ff       	callq  400560 <read@plt>

  		# objdump --disassemble ./vuln | grep system@plt
		0000000000400540 <system@plt>:
  		4006bb:	e8 80 fe ff ff       	callq  400540 <system@plt>
		```

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
