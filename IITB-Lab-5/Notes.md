# Lab-5 notes

- The exit() syscall basically changes the state of a process to zombie and sends a wakeup call to the parent
- If the parent of the process uses a wait syscall and is therefore sleeping, it will get up. The parent then clears the process table entry of the child, clears it's memory etc

