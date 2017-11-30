#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Christoph Schmidl");
MODULE_DESCRIPTION("Try to access the cr4 register in kernel mode");

static int __init hello_init(void)
{
	unsigned long long result;
	/* unsigned long result; (for 32-bit OS) */
	__asm__("movq %%cr4, %%rax\n" : "=a"(result));
	/*__asm__("mov %%cr4, %%eax\n" : "=a"(result)); (for 32-bit OS)*/
    printk(KERN_INFO "Value of CR4 = %llx\n", result);
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);