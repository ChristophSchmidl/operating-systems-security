# Assignment 4

## 1 - Play around with the setuid (suid) bit

In Week 1 lecture, you were introduced to the **setuid** bit. In this exercise, you will learn how to carry out *privilege escalation* using suid.

Login to your (Kali) Linux system as a **non**-root user and download the program **showdate** from [https://www.cs.ru.nl/~vmoonsamy/teaching/ossec2016/showdate](https://www.cs.ru.nl/~vmoonsamy/teaching/ossec2016/showdate) (for 64-bit OS).

Then, change the owner
`$ sudo chown root:root showdate`
set the suid bit and make it executable
`$sudo chmod u+s,a+x showdate`
Execute the program and verify it prints the date correctly
`$ ./showdate`
*Fri Nov 18 xx:xx:xx EST 2016*

Install the tool **strace**
`$ sudo apt-get isntall strace`
and run it to see system calls used by **showdate**
`$ strace -f ./showdate`



**Objectives**

* a) Find out what the program does internally. What system calls does it use?
	* Answer: strace is printing its output to stderr and not to stdout. Therefore you have to redirect its output to later pipe it to grep.

	```
	root@kali:~/operating_systems_security/assignment-4# strace -f ./showdate 2>&1 | grep -oP "\w+(?=\()" | sort -u
	access
	arch_prctl
	brk
	clone
	close
	execve
	exit_group
	fstat
	geteuid
	getpid
	getppid
	lseek
	mmap
	mprotect
	munmap
	open
	read
	rt_sigaction
	rt_sigprocmask
	rt_sigreturn
	setgid
	setuid
	stat
	wait4
	WEXITSTATUS
	WIFEXITED
	write
	```
	

* b) Assume the role of a non-privileged attacker. Use the program *showdate* to obtain a root shell. You can verify if you succeeded by looking at the output of **id**, it should be something like: `$ /usr/bin/id` *uid=0(root) gid=0(root) groups=0(root),27(sudo),1001(test1)* Hand in the exact console commands you used to get this working.
	* Answer:

* c) Explain what a developer could do to overcome this issue. What explicit actions should a developer take when writing software that is intended to be used with *setuid-root* to avoid these typed of problems?
	* Answer:	




## 2 - Trust models

In this exercise we consider a reference monitor which uses Mandatory Access Control (MAC) to implement the Bell-LaPadula and the Biba model. The Bell-LaPadula model uses levels **unclassified** <= **confidential** <= **secret** <= **top secret**. The Biba model uses levels **untrusted** <= **user** <= **application** <= **system** <= **trusted**. The following objects with corresponding secrecy and trust levels are used in this exercise:

* /home/peter/database (confidential, user),
* /etc/password (confidential, trusted)
* /etc/shadow (top secret, trusted)
* /usr/bin/someprog (unclassified, application)
* /usr/lib/somelib.so (unclassified, system)
* Network socket to 203.0.113.42, port 80 (unclassified, untrusted)

**Objectives**


* a) For each of the following steps determine whether the reference monitor will allow the action. If not, explain why not (if there are multiple reasons, state all).
	* i) User peter logs in with clearance (secret, application) and tries to run */usr/bin/someprog*.

		* Answer: I assume that both the Bell-LaPadula and the Biba model are using weak tranquility as shown in the slides. **Weak tranquility** = Security level of a process never changes in a way that it violates the security policy. Typically start with low level, and increase as the process reads higher-level information. As peter logs in with clearance (secret, application), */usr/bin/someprog* receives the levels (unclassified, trusted). Therefore the action is **allowed**. 

	* ii) The process dynamically loads (reads) */usr/lib/somelib.so*

		* Answer: As the process dynamically loads */usr/lib/somelib.so*, */usr/bin/someprog* receives the levels (unclassified, system). Therefore the action is **allowed**. 

	* iii) The process reads */home/peter/database*.

		* Answer: Because */home/peter/database* has the level (confidential, user) and */usr/bin/someprog* cannot read below the confidentiality level *application* (No read-down), this action is **not allowed**.

	* iv) The process writes data to the network socket.
		* Answer: Because the network socket has the level (unclassified, untrusted) and */usr/bin/someprog* is running with (unclassified, system), this action is **allowed**. The Biba rule forbids write-up but not write-down.

	* v) The process reads */etc/password*.
		* Answer: As the process is reading */etc/password* with level (confidential, trusted), the process is receiving the level (confidential, system) and is therefore **allowed** to execute this action. The Bida rule forbids read-down but not read-up.

	* vi) The process writes */etc/shadow*.
		* Answer: As the process is trying to write to */etc/shadow* with level (confidential, trusted), the process still has the level (confidential, system). The Bida rule forbids write-up, therefore this action is **not allowed**.

* b) The process from part a) now creates a new file */home/peter/out*. What are the permitted pairs of trust and secrecy level for this output file?

	* Answer: At the end of action vi the process spawned from executing */usr/bin/someprog* has the level of (confidential, system). As the Bell-LaPadula model forbids write-down but write-up is not a problem, the output file could have have any confidentiality level of confidential, secret or top secret but not unclassified. The Biba model forbids write-up but write-down is not a problem, therefore the integrity level could be anything between system, application, user or untrusted but not trusted. Any combination from those two sets is therefore a possible combination for the file */home/peter/out* like (top secret, system). 
