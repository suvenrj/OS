# Notes on Lab-2 Learnings

## SysCalls

### fork

- Returns 0 to the child processs and the process_id of the child to the parent process

- When a child process is created, the full memory snapshot of the parent process is copied and the same is created for the child process. The stack and heap are both copied

- Modern OSs follow Copy on Write (CoW). The copying of parent pages for child are only done when a write operation occurs on the page

### wait

- Halts a process until all it's child processes exit

### waitpid

- Halts a process until the child with the given pid exits

## Additional Pointers

- The syscall functions that we use in user programs are simply C library functions. The code for all syscalls lies in the kernel.

- The library syscall functions simply pass a special syscall instruction to the processor with a syscall number.

- The processor, on receiving the special instruction changes the mode from user to kernel and the appropriate syscall handler in the kernel is called