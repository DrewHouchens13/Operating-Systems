================================================================================
                        Operating Systems - Project 4
                        Memory Management (Kernel Modules)
================================================================================

Project Members:
----------------
Drew Houchens

Honor Statement:
----------------
I have neither given nor received unauthorized assistance on this work.

================================================================================
                            VM Information
================================================================================

NAME OF VIRTUAL MACHINE (VM):     HouchensVM
USERNAME:                         Drew
PASSWORD:                         Broncos4me05-

Code path on VM:                   /home/drew/Operating-Systems/Project4
                                  

Files are located under Project4/ with two subdirectories, one per kernel
module. Build each module in its own folder on the Ubuntu VM (kernel headers
required).

================================================================================
                          Build Instructions
================================================================================

Prerequisites on Ubuntu:
  - Same kernel headers as the running kernel:
      sudo apt update
      sudo apt install build-essential linux-headers-$(uname -r)

Build va_space:
  $ cd va_space
  $ make

Build va_status:
  $ cd ../va_status
  $ make

Clean (optional):
  $ make clean


================================================================================
                          Project Description
================================================================================

This project implements two loadable kernel modules that inspect Linux memory
management structures: summing virtual memory areas (VMAs) for a process, and
walking page tables to see whether a virtual address is backed by a present
PTE (in RAM) or not (e.g. swapped / unmapped).

Files Included:
---------------
va_space/
  - va_space.c    Part 1: Total virtual address space size for a PID (sum of
                  VMA ranges via maple-tree VMA iterator).
  - Makefile      Kernel module build file.

va_status/
  - va_status.c   Part 2: Page-table walk (PGD -> P4D -> PUD -> PMD -> PTE)
                  for a PID and hex virtual address; uses pte_present().
  - Makefile      Kernel module build file.

README.txt       This submission document.
LEARNING.md      Personal notes (optional / not required for Canvas backup).

================================================================================
                    Part 1: va_space Module Usage
================================================================================

Find a process PID (example: bash):
  $ pgrep bash

Load the module (replace <PID>):
  $ sudo insmod va_space/va_space.ko pid=<PID>

View kernel log (timestamps may appear out of order; reload module if needed):
  $ sudo dmesg -T | tail -30

Compare with pmap (optional):
  $ pmap <PID>
  Note: pmap total may be slightly larger than the module sum due to the
  vsyscall mapping; see course notes.

Unload:
  $ sudo rmmod va_space

================================================================================
                    Part 2: va_status Module Usage
================================================================================

Obtain a virtual address from pmap for the same PID:
  $ pmap -x <PID>
  Pick an address from the listing (hex range start).

Load the module. Pass the address as a string; use single quotes outside and
double quotes inside so the shell passes the hex string to the module:

  $ sudo insmod va_status/va_status.ko pid=<PID> vaddr='"0x55a1cafe1000"'

Examples without 0x prefix also work if digits are valid hex:
  $ sudo insmod va_status/va_status.ko pid=<PID> vaddr='"55a1cafe1000"'

Read results:
  $ sudo dmesg -T | tail -30

Unload:
  $ sudo rmmod va_status

================================================================================
                       Implementation Approach
================================================================================

va_space:
  - module_param(pid, int, ...).
  - Resolve task with pid_task(find_vpid(pid), PIDTYPE_PID) under RCU; hold a
    reference with get_task_struct/put_task_struct.
  - mmap_read_lock(mm); iterate VMAs with VMA_ITERATOR / for_each_vma (maple
    tree interface on modern kernels); sum (vm_end - vm_start); mmap_read_unlock.
  - printk total bytes and KB.

va_status:
  - module_param(pid, int, ...) and module_param(vaddr, charp, ...).
  - kstrtoul(vaddr, 16, &address).
  - Same task/mm lookup as va_space; mmap_read_lock(mm).
  - Walk pgd_offset -> p4d_offset -> pud_offset -> pmd_offset; handle leaf
    PMD (huge / THP) with pmd_leaf() before pte_offset_kernel.
  - Read PTE with READ_ONCE; pte_none vs pte_present vs swapped/unmapped.

================================================================================
                        AI Assistance Statement
================================================================================

I used Anthropic's Sonnet 4.6 model for assistance building my approach to this project. 

================================================================================
                          Notes for Grading
================================================================================

- Makefiles are named Makefile (capital M).
- Module names match assignment: va_space, va_status.
- Build tested on Ubuntu Linux VM with matching linux-headers package.

================================================================================
