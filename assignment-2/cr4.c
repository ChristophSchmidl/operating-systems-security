#include <stdio.h>

void main() {
	unsigned long long result;
	/* unsigned long result; (for 32-bit OS) */
	__asm__("movq %%cr4, %%rax\n" : "=a"(result));
	/*__asm__("mov %%cr4, %%eax\n" : "=a"(result)); (for 32-bit OS)*/
	printf("Value of CR4 = %llx\n", result);
}