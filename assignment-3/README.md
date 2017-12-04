# Assignment 3

## 1 - ´Bypassing ASLR


**Objectives**

* a) Find the address of **read@plt** and **system@plt** by disassembling *vuln* using **objdump**.
	* Answer:

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

