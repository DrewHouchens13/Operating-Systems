/*
 * va_status.c
 * Kernel module that walks the page tables for a process and reports whether
 * a given virtual address is resident in memory or not (e.g. swapped).
 *
 * Usage:
 *   sudo insmod va_status.ko pid=<PID> vaddr='"<hex>"'
 * Example:
 *   sudo insmod va_status.ko pid=32349 vaddr='"0x00400000"'
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
#include <asm/pgtable.h>

static int pid = -1;
static char *vaddr;

module_param(pid, int, 0644);
MODULE_PARM_DESC(pid, "PID of the process whose virtual address to inspect");

module_param(vaddr, charp, 0644);
MODULE_PARM_DESC(vaddr, "Virtual address as a hex string (use shell: vaddr='\"0xADDR\"')");

static int __init va_status_init(void)
{
	struct task_struct *task;
	struct mm_struct *mm;
	unsigned long address = 0;
	int err;
	char comm[TASK_COMM_LEN];
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *ptep;
	pte_t pteval;

	printk(KERN_INFO "va_status: From current process %s [%d], loading module\n",
	       current->comm, current->pid);

	if (pid < 0) {
		printk(KERN_ERR "va_status: Invalid pid. Usage: insmod va_status.ko pid=<PID> vaddr='\"<hex>\"'\n");
		return -EINVAL;
	}

	if (!vaddr || !*vaddr) {
		printk(KERN_ERR "va_status: Missing vaddr. Usage: insmod va_status.ko pid=<PID> vaddr='\"<hex>\"'\n");
		return -EINVAL;
	}

	err = kstrtoul(vaddr, 16, &address);
	if (err) {
		printk(KERN_ERR "va_status: Could not parse vaddr \"%s\" as hex (err=%d)\n",
		       vaddr, err);
		return -EINVAL;
	}

	printk(KERN_INFO "va_status: Inspecting PID %d, virtual address 0x%lx (%lu)\n",
	       pid, address, address);

	rcu_read_lock();
	task = pid_task(find_vpid(pid), PIDTYPE_PID);
	if (!task) {
		rcu_read_unlock();
		printk(KERN_ERR "va_status: No process found with PID %d\n", pid);
		return -ESRCH;
	}

	get_task_struct(task);
	rcu_read_unlock();

	get_task_comm(comm, task);

	mm = task->mm;
	if (!mm) {
		put_task_struct(task);
		printk(KERN_ERR "va_status: Process %d has no mm (kernel thread?)\n", pid);
		return -EINVAL;
	}

	printk(KERN_INFO "va_status: Found process %s [%d]\n", comm, task->pid);

	mmap_read_lock(mm);

	pgd = pgd_offset(mm, address);
	if (pgd_none(*pgd)
#ifdef pgd_bad
	    || unlikely(pgd_bad(*pgd))
#endif
			) {
		mmap_read_unlock(mm);
		put_task_struct(task);
		printk(KERN_WARNING "va_status: PGD entry missing or bad for address 0x%lx\n",
		       address);
		return -EFAULT;
	}

	p4d = p4d_offset(pgd, address);
	if (p4d_none(*p4d)
#ifdef p4d_bad
	    || unlikely(p4d_bad(*p4d))
#endif
			) {
		mmap_read_unlock(mm);
		put_task_struct(task);
		printk(KERN_WARNING "va_status: P4D entry missing or bad for address 0x%lx\n",
		       address);
		return -EFAULT;
	}

	pud = pud_offset(p4d, address);
	if (pud_none(*pud)
#ifdef pud_bad
	    || unlikely(pud_bad(*pud))
#endif
			) {
		mmap_read_unlock(mm);
		put_task_struct(task);
		printk(KERN_WARNING "va_status: PUD entry missing or bad for address 0x%lx\n",
		       address);
		return -EFAULT;
	}

	pmd = pmd_offset(pud, address);
	if (pmd_none(*pmd)) {
		mmap_read_unlock(mm);
		put_task_struct(task);
		printk(KERN_WARNING "va_status: PMD entry missing for address 0x%lx\n",
		       address);
		return -EFAULT;
	}

	/*
	 * Huge pages (including THP) install a leaf mapping at PMD level;
	 * pte_offset_kernel() does not apply.
	 */
	if (pmd_leaf(*pmd)) {
		int resident = pmd_present(*pmd);

		mmap_read_unlock(mm);
		put_task_struct(task);
		if (resident)
			printk(KERN_INFO "va_status: Address 0x%lx for %s [%d]: "
			       "in memory (leaf / huge PMD)\n",
			       address, comm, pid);
		else
			printk(KERN_INFO "va_status: Address 0x%lx for %s [%d]: "
			       "not resident at PMD (on disk or unmapped)\n",
			       address, comm, pid);
		return 0;
	}

	ptep = pte_offset_kernel(pmd, address);
	pteval = READ_ONCE(*ptep);

	mmap_read_unlock(mm);
	put_task_struct(task);

	if (pte_none(pteval))
		printk(KERN_INFO "va_status: Address 0x%lx for %s [%d]: "
		       "no PTE (not mapped in this page table)\n",
		       address, comm, pid);
	else if (pte_present(pteval))
		printk(KERN_INFO "va_status: Address 0x%lx for %s [%d]: "
		       "in memory (PTE present)\n",
		       address, comm, pid);
	else
		printk(KERN_INFO "va_status: Address 0x%lx for %s [%d]: "
		       "on disk or not resident (PTE not present)\n",
		       address, comm, pid);

	return 0;
}

static void __exit va_status_exit(void)
{
	printk(KERN_INFO "va_status: From current process %s [%d], cleaning up va_status.\n",
	       current->comm, current->pid);
}

module_init(va_status_init);
module_exit(va_status_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Reports whether a virtual address is resident using a page table walk");
MODULE_AUTHOR("Drew Houchens");
