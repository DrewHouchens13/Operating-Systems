NAME OF PROJECT:
================
Project 3 - POSIX Thread Programming


MEMBERS:
========
Drew Houchens


STATEMENT:
==========
I have neither given nor received unauthorized assistance on this work.


VIRTUAL MACHINE (VM) INFORMATION:
=================================
NAME OF VIRTUAL MACHINE (VM): (fill in your VM name)
USERNAME: (fill in your username)
PASSWORD: (fill in your password)

Files are located at ~/Project3/ on the VM (or wherever you copy them).

Directory layout:

  Project3/
    task1_substring/        -- Task 1: parallel substring search
      substring_parallel.c  -- pthread parallel implementation
      strings.txt           -- sample input file
      Makefile
    task2_prodcon/          -- Task 2: producer-consumer
      prodcon.c             -- condition variable implementation
      message.txt           -- sample input message
      Makefile
    task3_listforming/      -- Task 3: optimized list-forming
      list-forming.c        -- original version (for benchmarking)
      my_list-forming.c     -- optimized version
      Makefile

To build and run each task, cd into the task directory and run:
  $ make
  $ ./<executable>  (see details below)

ROOT PASSWORD:
==============
(fill in root password)


PROBLEM DESCRIPTION AND REMEDIATION:
====================================

	PART 0: SET UP
	==============
	Ensured the VM has 4 virtual CPUs. Verified with:
	  $ cat /proc/cpuinfo
	Used the provided writeup template for the report.

	PART 1: PARALLEL SUBSTRING SEARCH (Task 1)
	===========================================
	Parallelized the sequential substring counter using pthreads.
	The search index space is partitioned across threads (not the
	string itself) so substrings spanning partition boundaries are
	not missed.  Each thread stores a local count; the main thread
	sums them after joining.

	Build and run:
	  $ cd task1_substring
	  $ make
	  $ ./substring_parallel <num_threads>

	Example:
	  $ ./substring_parallel 4
	  The number of substrings is: 4

	PART 2: PRODUCER-CONSUMER (Task 2)
	===================================
	Implemented a producer-consumer pair using a circular buffer of
	size 15 and condition variables.  The producer reads characters
	from message.txt; the consumer prints them in order.  Condition
	variables handle blocking when the buffer is full or empty.

	Build and run:
	  $ cd task2_prodcon
	  $ make
	  $ ./prodcon

	PART 3: OPTIMIZED LIST-FORMING (Task 3)
	========================================
	Optimized list-forming.c with three changes:
	  1. Local list batching (K nodes built lock-free, one lock to splice)
	  2. pthread_mutex_lock instead of trylock (block instead of spin)
	  3. Removed CPU pinning (reduces cache-line bouncing)

	Build and run:
	  $ cd task3_listforming
	  $ make
	  $ ./list-forming <num_threads>       # original
	  $ ./my_list-forming <num_threads>    # optimized

	Benchmark results (K=200, runtime in microseconds):

	  num_threads | Original (us) | Optimized (us) | Speedup
	  ------------|---------------|----------------|--------
	            4 |          1236 |            815 |   1.52x
	           20 |          3843 |           3336 |   1.15x
	          100 |         18102 |          11936 |   1.52x

	The optimized version is consistently faster. The gap widens as
	thread count increases because the original acquires the lock K
	times per thread (heavy contention), while the optimized version
	acquires it only once per thread (local batching).
