/*
 * va_space.c
 * Kernel module that reports total virtual address space size (sum of VMA
 * ranges) for a process identified by PID.
 *
 * Usage: sudo insmod va_space.ko pid=<PID>
 *
 * Author: Drew Houchens
 * Course: Operating Systems
 * Project 4: Memory Management
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/mm.h>
#include <linux/moduleparam.h>
#include <linux/rcupdate.h>

static int pid = -1;
module_param(pid, int, 0644);
MODULE_PARM_DESC(pid, "PID of the process whose virtual address space size to compute");

static int __init va_space_init(void)
{
	struct task_struct *task;
	struct mm_struct *mm;
	struct vma_iterator iter;
	struct vm_area_struct *vma;
	unsigned long total_bytes = 0;
	unsigned long kb;
	char comm[TASK_COMM_LEN];

	printk(KERN_INFO "va_space: From current process %s [%d], loading module\n",
	       current->comm, current->pid);

	if (pid < 0) {
		printk(KERN_ERR "va_space: Invalid pid. Usage: insmod va_space.ko pid=<PID>\n");
		return -EINVAL;
	}

	rcu_read_lock();
	task = pid_task(find_vpid(pid), PIDTYPE_PID);
	if (!task) {
		rcu_read_unlock();
		printk(KERN_ERR "va_space: No process found with PID %d\n", pid);
		return -ESRCH;
	}

	get_task_struct(task);
	rcu_read_unlock();

	mm = task->mm;
	if (!mm) {
		put_task_struct(task);
		printk(KERN_ERR "va_space: Process %d has no mm (kernel thread?)\n", pid);
		return -EINVAL;
	}

	get_task_comm(comm, task);
	printk(KERN_INFO "va_space: Found process %s [%d]\n", comm, task->pid);

	mmap_read_lock(mm);

	VMA_ITERATOR(iter, mm, 0);
	for_each_vma(iter, vma)
		total_bytes += vma->vm_end - vma->vm_start;

	mmap_read_unlock(mm);

	kb = total_bytes / 1024UL;

	printk(KERN_INFO "va_space: total virtual memory size of %s [%d]: %lu bytes (%lu KB)\n",
	       comm, pid, total_bytes, kb);

	put_task_struct(task);

	return 0;
}

static void __exit va_space_exit(void)
{
	printk(KERN_INFO "va_space: From current process %s [%d], cleaning up va_space.\n",
	       current->comm, current->pid);
}

module_init(va_space_init);
module_exit(va_space_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Reports total virtual address space size for a PID using VMA iteration");
MODULE_AUTHOR("Drew Houchens");
