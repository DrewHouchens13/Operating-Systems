/*
 * print_self.c
 * Kernel module that prints information about the current process
 * and walks the parent chain back to init (PID 1).
 *
 * Author: Drew Houchens
 * Course: Operating Systems
 * Project 2: Kernel Modules and Processes
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

static const char *state_to_string(unsigned int state)
{
	switch (state) {
	case TASK_RUNNING:
		return "TASK_RUNNING";
	case TASK_INTERRUPTIBLE:
		return "TASK_INTERRUPTIBLE";
	case TASK_UNINTERRUPTIBLE:
		return "TASK_UNINTERRUPTIBLE";
	case __TASK_STOPPED:
		return "TASK_STOPPED";
	case __TASK_TRACED:
		return "TASK_TRACED";
	default:
		return "UNKNOWN";
	}
}

int init_module(void)
{
	struct task_struct *task;

	printk(KERN_INFO "print_self: Module loaded\n");
	printk(KERN_INFO "print_self: Walking process tree from current to init\n");
	printk(KERN_INFO "print_self: %-20s %-10s %s\n", "NAME", "PID", "STATE");
	printk(KERN_INFO "print_self: %-20s %-10s %s\n", "----", "---", "-----");

	task = current;
	while (task->pid != 0) {
		printk(KERN_INFO "print_self: %-20s %-10d %s (%u)\n",
		       task->comm,
		       task->pid,
		       state_to_string(task->__state),
		       task->__state);
		task = task->parent;
	}

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "print_self: Module removed\n");
}

module_init(init_module);
module_exit(cleanup_module);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Prints current process info and parent chain to init");
MODULE_AUTHOR("Drew Houchens");
