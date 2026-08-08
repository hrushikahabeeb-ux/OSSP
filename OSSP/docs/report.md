# Hardware Resources and OS Abstraction

**Practical Week 2 — Task 2**

## 1. Objective

Using the Linux terminal commands `uname`, `lscpu`, `ps`, and `top`, investigate
the relationship between the underlying hardware and the services the operating
system provides on top of it, and explain how the OS abstracts the CPU, memory,
storage, and I/O devices for user programs.

## 2. Commands and what they reveal

### 2.1 `uname -a`

Reports the kernel name, hostname, kernel release/version, and machine hardware
architecture (e.g. `x86_64`). This is the OS identifying the exact hardware
platform and kernel build it is mediating access to — every abstraction the OS
offers above this point is built for that specific architecture and kernel ABI.

### 2.2 `lscpu`

Reports the physical CPU topology: number of cores/threads, sockets, cache
sizes (L1/L2/L3), clock speed, and instruction set extensions. This is the raw
hardware inventory that the OS scheduler works with — it doesn't change how a
program is written, but it determines how many processes/threads the scheduler
can genuinely run in parallel versus time-slice.

### 2.3 `ps`

Lists the operating system's process table: PID, parent PID, state, controlling
terminal, and CPU/memory usage per process. Each entry is the OS's internal
bookkeeping for one running program — proof that the "process" a user sees is
an OS-level construct layered over the CPU's raw instruction stream.

### 2.4 `top`

Shows the same process information as `ps`, but live and ranked by resource
consumption, alongside system-wide CPU and memory utilization. It exposes the
scheduler's decisions in real time: which processes are getting CPU slices,
how memory is partitioned across running programs, and how load shifts as
processes are created and destroyed (visible directly if `command_executor`
from Task 1 is run in one terminal while `top` runs in another).

## 3. How the OS abstracts hardware

| Resource | Raw hardware reality | OS abstraction presented to programs |
|---|---|---|
| **CPU** | A fixed number of physical cores/threads executing one instruction stream each | **Processes and threads** — the illusion of many programs running "simultaneously" via preemptive scheduling and context switching |
| **Memory** | One physical address space (RAM) shared by everything | **Virtual memory** — each process gets its own private address space, mapped to physical frames (or swap) by the MMU and page tables, so processes can't see or corrupt each other's memory |
| **Storage** | Blocks/sectors on a physical disk or SSD, addressed by raw device offsets | **Filesystems** — a hierarchical namespace of files and directories, accessed through `open()`/`read()`/`write()`/`close()`, hiding block layout, wear-levelling, and device-specific protocols |
| **I/O devices** | Heterogeneous hardware (keyboard, terminal, network card, etc.), each with its own controller and register interface | **Device files and system calls** (e.g. everything is a file descriptor under `/dev`) — a uniform read/write interface regardless of the physical device behind it |

## 4. Synthesis

`uname` and `lscpu` describe the fixed hardware the OS has to work with; `ps`
and `top` show the OS's process abstraction in action on top of that hardware.
Together they illustrate the core theme of this lab: the OS's job is to convert
a small set of physical resources (a handful of CPU cores, one memory bus, one
or more storage devices) into abstractions — processes, virtual memory, files,
and device I/O — that let many independent programs run concurrently and
safely without needing to know about each other or about the physical hardware
underneath.

This is exactly what Task 1's `command_executor` demonstrates in miniature:
`fork()` asks the OS for a new process abstraction, `exec()` asks it to load a
new program image into that process's virtual address space, and `wait()`
asks it to report back on that abstraction's lifecycle — all without the
calling program touching any hardware register directly.
