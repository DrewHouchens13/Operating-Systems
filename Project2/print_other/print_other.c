/*
 * print_other.c
 * Kernel module that takes a PID as a parameter and prints information
 * about that process, walking the parent chain back to init (PID 1).
 *
 * Usage: sudo insmod print_other.ko pid=<PID>
 *
 * Author: Drew Houchens
 * Course: Operating Systems
 * Project 2: Kernel Modules and Processes
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/pid.h>
#include <linux/moduleparam.h>

static int pid = -1;
module_param(pid, int, 0644);
MODULE_PARM_DESC(pid, "PID of the process to inspect");

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

static int __init print_other_init(void)
{
	struct task_struct *task;

	printk(KERN_INFO "print_other: Module loaded with pid=%d\n", pid);

	if (pid < 0) {
		printk(KERN_ERR "print_other: No valid PID provided. "
		       "Usage: insmod print_other.ko pid=<PID>\n");
		return -EINVAL;
	}

	rcu_read_lock();
	task = pid_task(find_vpid(pid), PIDTYPE_PID);
	if (task == NULL) {
		rcu_read_unlock();
		printk(KERN_ERR "print_other: No process found with PID %d\n",
		       pid);
		return -ESRCH;
	}

	printk(KERN_INFO "print_other: Walking process tree from PID %d to init\n",
	       pid);
	printk(KERN_INFO "print_other: %-20s %-10s %s\n", "NAME", "PID", "STATE");
	printk(KERN_INFO "print_other: %-20s %-10s %s\n", "----", "---", "-----");

	while (task->pid != 0) {
		printk(KERN_INFO "print_other: %-20s %-10d %s (%u)\n",
		       task->comm,
		       task->pid,
		       state_to_string(task->__state),
		       task->__state);
		task = task->parent;
	}

	rcu_read_unlock();
	return 0;
}

static void __exit print_other_exit(void)
{
	printk(KERN_INFO "print_other: Module removed\n");
}

module_init(print_other_init);
module_exit(print_other_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Prints process info for a given PID and its parent chain");
MODULE_AUTHOR("Drew Houchens");
