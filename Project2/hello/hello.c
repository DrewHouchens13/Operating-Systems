/*
 * hello.c
 * A simple helloworld Linux kernel module.
 *
 * Author: Drew Houchens
 * Course: Operating Systems
 * Project 2: Kernel Modules and Processes
 */

#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void) {
	printk(KERN_INFO "Hello world!\n");
	return 0;
}

void cleanup_module(void) {
	printk(KERN_INFO "Goodbye world!\n");
}

module_init(init_module);
module_exit(cleanup_module);

MODULE_LICENSE("GPL");
