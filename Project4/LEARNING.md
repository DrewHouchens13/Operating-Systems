# Project 4 — Personal Learning Notes

This file is for your own reference (not required on Canvas unless your instructor asks).

## What to take away from this project

### Virtual memory and VMAs

- Each user process has an `mm_struct` describing its virtual address space.
- That space is **not** one contiguous range; it is a set of **VMAs** (`vm_area_struct`), each with `[vm_start, vm_end)`.
- Summing `(vm_end - vm_start)` over all VMAs gives **total mapped virtual size** for regions the kernel tracks — useful to relate to `pmap` / `/proc/<pid>/maps`.
- Modern kernels store VMAs in a **maple tree**, not the old `mmap` linked list; the iterator API (`VMA_ITERATOR` / `for_each_vma`) is the supported way to traverse them.

### Page tables

- A virtual address is translated through **multi-level page tables** (on x86_64 with 5-level support: PGD → P4D → PUD → PMD → PTE).
- Each level is a table of entries; **walking** the levels is exactly what the CPU’s MMU does on TLB miss (in hardware), while your module does it in software for inspection.
- **`pte_present()`** tells you whether the kernel considers the page **currently in RAM** (PTE present bit). If the PTE exists but is not “present,” the content may be **swap-backed** or otherwise not resident — the distinction matters for “in memory vs on disk.”
- **Huge / transparent huge pages** can install a **leaf at PMD** (`pmd_leaf`): then there is no normal PTE for that address; you must check the PMD instead of only `pte_offset_kernel`.

### Kernel module habits

- **`current`** during `insmod`/`rmmod` is the loader process, not your target PID — always resolve the target task by PID explicitly.
- **`dmesg` ordering** can look wrong under load; the assignment warns about this — use `dmesg -T` and expect possible reordering.
- **RCU + reference counting**: `pid_task` + `get_task_struct` / `put_task_struct` keeps the task struct from disappearing while you use it.
- **`mmap_read_lock(mm)`** protects the memory layout and page tables while you walk VMAs or page tables for another process.

---

## What transfers to industry

| Area | Why this project matters |
|------|---------------------------|
| **Performance / backend** | Page faults, residency, and huge pages affect tail latency (DBs, JVMs, game engines). Understanding “is this page hot in RAM?” is the same abstraction as tuning huge pages, mmap I/O, and memory-mapped stores. |
| **Containers / Kubernetes** | cgroup memory limits, OOM behavior, and “working set” reasoning assume **virtual vs resident** memory — same PTE/present story at a higher level. |
| **Cloud / virtualization** | Ballooning, overcommit, and dedup (e.g. KSM) interact with **what’s actually resident** vs merely mapped. |
| **Security** | Isolation between processes is implemented via separate page tables and permission bits in PTEs; exploits and mitigations (e.g. SMEP, KPTI) sit on top of this machinery. |
| **Debugging** | Tools like `pmap`, `/proc/<pid>/maps`, `perf`, and memory profilers ultimately reflect **VMA layout and PTE state** you learned to touch in-kernel. |

---

## Skills you can cite on a resume / interview

- Built **out-of-tree Linux kernel modules** with the kbuild `Makefile` pattern.
- Read kernel APIs for **`task_struct`**, **`mm_struct`**, **VMA iteration**, and **page table walk** macros.
- Used **RCU**, **mmap locking**, and **`module_param`** safely in kernel context.
- Related experimental results to **userspace tools** (`pmap`, `dmesg`) and course theory (virtual memory, multi-level paging).

---

## Suggested follow-ups (optional)

- Compare your VA sum to `pmap` and reconcile the **vsyscall** gap mentioned in the assignment.
- Try `va_status` on addresses from **stack vs heap vs file mapping** and observe differences when memory is **trimmed** or **swapped** (careful on systems with swap disabled).
- Skim **Documentation/vm** and Mel Gorman’s “Understanding the Linux Virtual Memory Manager” for deeper intuition.
