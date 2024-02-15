# program Description 

   To recursively traverse a directory hierarchy and lists all subdirectories and files with their respective types.

# Compilation Instructions
```bash
gcc -o lab5 lab5.c 
```

# running instructions 

```bash
    ./lab5
```

# My Files
## lab5.c

This program recursively traverse a directory hierarchy and lists all subdirectories and files with their respective types.

   It takes directory path as command line argument.
   Validate if the given directory is correct or not.
   The traverseDirectory function opens the directory specified by dirName.
   It iterates over each entry in the directory, printing its name and type.
   It correctly identifies the type of each entry in the directory hierarchy using the dirent->d_type field and prints the corresponding file type using the filetype() function.
   If the entry is a subdirectory (excluding "." and ".."), it recursively calls itself to traverse the subdirectory.
   


# Github link
https://github.com/Misritha0810/sysassignments.git