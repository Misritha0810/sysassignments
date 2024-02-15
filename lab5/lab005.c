l#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

char *filetype(unsigned char type) {
    char *str;
    switch(type) {
        case DT_BLK: str = "block device"; break;
        case DT_CHR: str = "character device"; break;
        case DT_DIR: str = "directory"; break;
        case DT_FIFO: str = "named pipe (FIFO)"; break;
        case DT_LNK: str = "symbolic link"; break;
        case DT_REG: str = "regular file"; break;
        case DT_SOCK: str = "UNIX domain socket"; break;
        case DT_UNKNOWN: str = "unknown file type"; break;
        default: str = "UNKNOWN";
    }
    return str;
}

void traverseDirectory(char *dirName) {
    struct dirent *dirent;
    DIR *dirp;

    dirp = opendir(dirName);
    if (dirp == NULL) {
        fprintf(stderr, "Error opening directory '%s'\n", dirName);
        exit(EXIT_FAILURE);
    }

    int count = 1;
    while ((dirent = readdir(dirp)) != NULL) {
        char filePath[1024];
        snprintf(filePath, sizeof(filePath), "%s/%s", dirName, dirent->d_name);
        printf("[%d] %s (%s)\n", count, dirent->d_name, filetype(dirent->d_type));

        if (dirent->d_type == DT_DIR && strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {
            traverseDirectory(filePath);
        }
        count++;
    }

    closedir(dirp);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    traverseDirectory(argv[1]);

    return 0;
}
