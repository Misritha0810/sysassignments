# program Description 

    This program waits for the child process to end, configures signal handlers in the parent process, forks a child process to carry out a command, transmits signals between parent and child processes, and gracefully handles termination signals for program cleanup.

# Compilation Instructions
```bash
gcc -o lab9 lab9.c 
```

# running instructions 

```bash
    ./lab9
```

# My Files
## lab9.c

The program sets up signal handlers, forks a child process, forwards signals, waits for termination, and handles signals for cleanup.

    Handles signals SIGINT, SIGTSTP, and SIGQUIT in the parent process.
    Forwards SIGINT and SIGTSTP to the child process if it's active.
    Gracefully terminates the parent process upon receiving SIGQUIT.
    Ensures proper cleanup and termination of child processes.
    Executes a specified command in the child process.

# Github link

https://github.com/Misritha0810/sysassignments.git