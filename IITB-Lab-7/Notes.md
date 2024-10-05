# Lab 7 Notes

- Do not use the acquire/release functions implemented in spinlocks.c for the implementing the syscalls in part-1. The reason being that these functions switch off interrupts a thread is executing the critical section. This is fine for critical sections in the kernel code as the kernel code is not malacious. But if a user code is provided a syscall which switches off interrupts, it can hog on the processor forever.

- A lock does not have an owner. Although only 1 thread holds a lock, it doesnt mean that the thread is the owner of that lock. Even if a thread which has acquired a lock, if it tries to re-acquire it without releasing it, it will keep waiting. This fact has been used in toggle.c

- A good way for a parent to wait for all it's child processes that it spawns is to have a for loop in the end which calls wait n times where n is the number of children spawned