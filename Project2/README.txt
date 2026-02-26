================================================================================
                        Operating Systems - Project 2
                        Kernel Modules and Processes
================================================================================

Project Members:
----------------
Drew Houchens

Honor Statement:
----------------
We have neither given nor received unauthorized assistance on this work.

================================================================================
                            VM Information
================================================================================

VM Directory:       dhouchen/HouchensVM
Code Path in VM:    /home/drew/Operating-Systems/Project2
Account Password:   [FILL IN]
Root Password:      [FILL IN]

================================================================================
                          Build Instructions
================================================================================

1. Navigate to the Project2 directory containing all source subdirectories.

2. Run the setup script to install dependencies and build all modules:
   $ chmod +x vm_setup.sh
   $ ./vm_setup.sh

   Or build each module individually:
   $ cd hello && make && cd ..
   $ cd print_self && make && cd ..
   $ cd print_other && make && cd ..

3. Clean up compiled files (optional):
   $ cd hello && make clean && cd ..
   $ cd print_self && make clean && cd ..
   $ cd print_other && make clean && cd ..

================================================================================
                          Project Description
================================================================================

This project implements three Linux kernel modules that demonstrate how
to interact with kernel internals, specifically process management via
the task_struct structure.

Files Included:
---------------
hello/
  - hello.c         : Helloworld kernel module (Part 1)
  - Makefile         : Kernel build Makefile for hello module

print_self/
  - print_self.c     : Module that prints current process info and parent
                       chain back to init/systemd (Part 2)
  - Makefile         : Kernel build Makefile for print_self module

print_other/
  - print_other.c    : Module that takes a PID parameter and prints that
                       process's info and parent chain (Part 3)
  - Makefile         : Kernel build Makefile for print_other module

- README.txt         : This documentation file
- vm_setup.sh        : Helper script for automated build on Ubuntu VM

================================================================================
                    Part 1: Hello Module Usage
================================================================================

Load the module:
   $ sudo insmod hello/hello.ko

Check the kernel log for "Hello world!":
   $ sudo dmesg -T | tail

Verify the module is loaded:
   $ sudo lsmod | grep hello

Remove the module:
   $ sudo rmmod hello

Check the kernel log again for "Goodbye world!":
   $ sudo dmesg -T | tail

Note the different timestamps for the Hello and Goodbye messages.

================================================================================
                    Part 2: print_self Module Usage
================================================================================

Load the module:
   $ sudo insmod print_self/print_self.ko

Read the output showing the current process and its parent chain:
   $ sudo dmesg -T | tail -20

Remove the module:
   $ sudo rmmod print_self

The output shows each process from the current user-level process that
triggered insmod, walking up through parent processes until reaching
init/systemd (PID 1). For each process, the name, PID, and state are printed.

================================================================================
                    Part 3: print_other Module Usage
================================================================================

First, find a valid PID to inspect:
   $ pgrep bash

Load the module with a PID argument:
   $ sudo insmod print_other/print_other.ko pid=<PID>

Read the output:
   $ sudo dmesg -T | tail -20

Remove the module:
   $ sudo rmmod print_other

The output shows the specified process and its parent chain up to
init/systemd, with name, PID, and state for each.

================================================================================
                       Implementation Approach
================================================================================

Part 1 - hello:
  Standard kernel module using init_module() and cleanup_module() with
  printk(KERN_INFO ...) for output to the kernel ring buffer.

Part 2 - print_self:
  Uses the "current" macro from <linux/sched.h> to obtain a pointer to
  the task_struct of the running process. Iterates through the ->parent
  pointer chain until reaching PID 0 (the swapper/idle process, parent
  of init). The numeric process state (task->__state) is mapped to a
  human-readable string using a helper function.

Part 3 - print_other:
  Accepts a PID via module_param(). Uses pid_task(find_vpid(pid), PIDTYPE_PID)
  to look up the target process safely. The lookup is protected by
  rcu_read_lock()/rcu_read_unlock() to prevent the task_struct from being
  freed during traversal. Walks the same parent chain as print_self.

================================================================================
                        AI Assistance Statement
================================================================================

I received guiding assistance from Claude (Anthropic) in building kernel
module source files, Makefiles, and documentation. The core kernel module
logic was developed with my understanding of Linux kernel internals and
the task_struct structure.

================================================================================
                          Notes for Grading
================================================================================

- All modules compile using the kernel build system (no warnings expected)
- Makefiles use capital-M "Makefile" as required by kernel build scripts
- Directory names contain no spaces (uses underscores)
- vm_setup.sh handles DOS-to-Unix line ending conversion automatically
- Process state values are mapped to human-readable strings
- print_other uses RCU locking for safe task_struct access

================================================================================
