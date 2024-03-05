## Search Program Overview

The search program gives a better approach to directory exploration and file information retrieval. It is written in a way users an array of customizable options for displaying directory contents. Key features of this program include the ability to search for files and directories using patterns, apply filters based on file size, specify file or directory types for listing, and set limits on traversal depth.

The program now supports the -e and -E commands as well with unix command as argument. The purpose of this commands is to run unix command on the filtered files from the search program edither individually or at once or all together.

## Functions 

`displayDataFormatted(struct stat *statbuf)`: This function prints size in bytes, permissions, and last modification date of the given file or directory based on its stat structure.

`FilePermissionsFetcher(mode_t mode)`: This function generates and prints a string representation of the file permissions (e.g., "rwxr-xr--") and the octal value for the given mode argument.

`traverseDirectory(const char *basePath, Config config, int currentDepth)`: This function recursively traverses directories starting from basePath, applying filters and printing details based on the Config structure, up to a specified depth.

`parseArgs(int argc, char **argv, Config *config)`: This function parses the command-line arguments provided to the program and populates a Config structure with the appropriate flags and values extracted from these arguments.

`executeUnixCommand`: This function executes a Unix command specified by command with an optional file path specified by filePath. It uses fork to create a child process and execl to execute the command. If successful, the child process executes the command and terminates. Otherwise, it prints an error message to stderr and exits with failure status. The parent process waits for the child process to complete before continuing execution.

`executeCommandForFiles`: This function executes Unix commands on files based on the provided configuration. If config.executeEachFlag is set, it executes specified command individually on each file in the `fileList`. If 
config.executeAllFlag is set, it constructs a single command string concatenating all file paths and executes the command once for all files. The function dynamically allocates memory to accommodate the concatenated command string and frees it after execution.

`main(int argc, char **argv)`: This is the base or main function initializes the program, processes command-line arguments to configure the traversal, then starts traversing from the current working directory or specified path and finally handles any errors.



## Compilation and Execution Instructions

To compile the Code, follow these steps:

    Open a terminal or command prompt.
    Change to the directory where you have saved the source code file Mmaridi_HW03.c.
    Execute the following command to compile the source code:
    
    ```
    gcc  Mmaridi -o Mmaridi_HW03.c
    ```

To Execute the Code , run below instruction

    ```
        ./Mmaridi
    ```

## Usage Guide

This Program offers a range of command-line options to print directories in a hierarchy mode and filter the files as per given arguments.


### Options

-`v`: Activates detailed mode, providing comprehensive details for each item, including file and directory attributes.
-`s <pattern> <depth>`: Applies a filter to list down the listings to only those items matching the specified pattern upto given depth level .
-`L <size>`: Shows only those files exceeding a certain size threshold, with the size specified in bytes.
-`t d|f`: Displays the output to exclusively include either directories (d) or files (f), based on the chosen option.
-`-e <command>`: Executes the specified command for each matched file or directory for filtered data.
-`-E <command>`: Executes the specified command once for all matched files or directories for filtered data.


### Run Commands

    ./Mmaridi 
    ./Mmaridi -v
    ./Mmaridi -L <size>
    ./Mmaridi -s <pattern> <level>
    ./Mmaridi -t d
    ./Mmaridi -t f
    ./Mmaridi -v -L <size>
    ./Mmaridi -v -s <pattern> <level>
    ./Mmaridi -v -t d
    ./Mmaridi -v -t f
    ./Mmaridi -L <size> -s <pattern> <level>
    ./Mmaridi -L <size> -t d
    ./Mmaridi -L <size> -t f

    ./Mmaridi -e <unixcommand>
    ./Mmaridi -E <unixcommand>
    ./Mmaridi -v -e <unixcommand>
    ./Mmaridi -v -E <unixcommand>
    ./Mmaridi -L <size> -e <unixcommand>
    ./Mmaridi -L <size> -E <unixcommand>


    These are some of the commands which you can try and test, however you can mix and match different commands and see filtered outputs in the command line.

    
## Exit Status

- `EXIT_SUCCESS` (0): It means The program executed successfully.
- `EXIT_FAILURE` (1): It means The program encountered an error.

## Author Details

    Name: Misritha Maridi
    BlazerId: Mmaridi
    Project #: Homework 3

## Github link

    https://github.com/Misritha0810/sysassignments.git


## Note :

As we have extended homework2 , I have extended the README.md file to be of sma ewith additional info on the changes made.