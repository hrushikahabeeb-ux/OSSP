# OSSP — Operating Systems Shell Project

A custom Linux shell (`my_shell`) built incrementally across the weekly labs of the
Operating Systems practical course. It demonstrates core OS concepts — process
creation (`fork`), program execution (`exec` family), synchronization (`wait`),
and file I/O via raw system calls (`open`/`read`/`write`/`close`).

## Project structure

- `src/` — source files
- `include/` — header files
- `obj/` — compiled object files (generated, git-ignored)
- `bin/` — final executables (generated, git-ignored)
- `docs/` — design notes and reports
- `tests/` — unit test programs
- `scripts/` — helper shell scripts
- `assets/` — screenshots/diagrams referenced in docs

## Build

```bash
make        # builds bin/my_shell
make run    # builds and runs it
make clean  # removes build artifacts
```

## Week 2 — Command executor

`src/main.c` is a standalone demo (compiled separately, see below) that shows how
the OS runs a command: it reads a command from the user, `fork()`s a child, the
child `exec()`s the command, and the parent `wait()`s for it, printing both PIDs.

```bash
gcc -Wall -Wextra -o bin/command_executor src/main.c
./bin/command_executor
```

See `docs/report.md` for the accompanying write-up on how the OS abstracts CPU,
memory, storage, and I/O (Week 2, Task 2).

## Requirements

- GCC
- A POSIX-compliant Linux environment (fork/exec/wait, and later, termios for
  raw keyboard input)
- Valgrind (for later weeks' memory-safety checks)
