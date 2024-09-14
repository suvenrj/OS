# Notes on Lab-3 Learnings

## Signals

- Signals enable User-to-process (ex. pressing Ctrl+C), Hardware-to-process (ex. Segmentation Fault) and Porcess-to-process communication via OS as an intermediary

- Signals don't carry any payload, hence can only signify events

- Signal Handlers are user-defined functions in the user codespace that define the process's response to a given signal type

- When an OS receives a signal for a process, it checks whether that process has a registered signal handler for that signal type. If it does, then the processor returns back to user mode and runs that signal handler, else the default signal handler in the kernel space is run