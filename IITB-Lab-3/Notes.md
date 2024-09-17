# Notes on Lab-3 Learnings

## Signals

- Signals enable User-to-process (ex. pressing Ctrl+C), Hardware-to-process (ex. Segmentation Fault) and Porcess-to-process communication via OS as an intermediary

- Signals don't carry any payload, hence can only signify events

- Signal Handlers are user-defined functions in the user codespace that define the process's response to a given signal type

- When an OS receives a signal for a process, it checks whether that process has a registered signal handler for that signal type. If it does, then the processor returns back to user mode and runs that signal handler, else the default signal handler in the kernel space is run

- If a process sleeps due to sleep() syscall, it can be interrupted by a signal and the process will start executing from the signal handler

## Turtle Shell

- execv system call, if successful, will replace all the existing memory (code/data) of the process with the code of the process which has been passed as the param in the exec. The data memory (stack/heap) starts afresh.

- Use strcmp in C to compare strings by value. Simple assignment comparison will only compare their addresses

- Any variable not initialized using malloc is on the heap. So, no need to use free on them

- A child process whose exit status is not collected by their parent using wait/waitpid syscalls stays as a zombie process entry in the process table. Only when the parent process reaps (acknowledges) the exit of the child process, the child process's entry is removed from the process table