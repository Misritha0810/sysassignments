# program Description 

      To read commands from a text file, executes them with arguments using fork-exec, logs execution details to a seperate file including start time and endtime of. the command execution.

# Compilation Instructions
```bash
gcc -o lab7 lab7.c 
```

# running instructions 

```bash
    ./lab7 <filename>
```

# My Files
## lab7.c

This program reads commands from a text file given from input argument and executes each command along with its arguments using fork-exec, stores the execution details, and writes them to a output logger file.


   1. Opened the input file( test.txt ) which is given as a command-line argument and reads commands from file line by line using fgets.
   2. Executed each command along with its arguments using fork-exec where it is from referenced code.
   3. Stored the start time before executing each command and end time after the command execution completes.
   4. Store the command information with start time, and end time into a log file (output.log).

# Github link
https://github.com/Misritha0810/sysassignments.git