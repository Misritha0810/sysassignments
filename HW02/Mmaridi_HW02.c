#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include<errno.h>
#include<time.h>
#include <inttypes.h> 


/*
Name: Misritha Maridi
BlazerId: Mmaridi
Project #: HomeWork2
To compile: gcc Mmaridi_HW02.c
To run: ./a.out <dir_path>
        ./a.out -v <dir_path>
        ./a.out -v -s <pattern> <depth -L <size> <dir_path>
        ./a.out -s <pattern> <depth> <dir_path>
        ./a.out -v -s <pattern> <depth <dir_path>
        ./a.out -L <size> <dir_path>
        ./a.out -t d|f <dir_path>
        ./a.out -v -s <pattern> <depth> -L <size> -t d|f <dir_path>
*/


typedef struct {
    int sizeRestrictionFlag, substringPattern, fileCategoryVar, detailedAnalysisVar;
    long long sizeValue; 
    int subfolderLevel;
    char searchPattern[256], fileFormat;
} Config;

void displayDataFormatted(struct stat *statbuf);

void handle_error(const char *message) {
    fprintf(stderr, "Error: %s - %s\n", message, strerror(errno));
    exit(EXIT_FAILURE);
}

void FilePErmissionsFetcher(mode_t mode) {
    mode_t permissions[] = { S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH };
    char symbols[] = "rwxrwxrwx";
    for (int i = 0; i < sizeof(permissions) / sizeof(mode_t); i++) {
        putchar(mode & permissions[i] ? symbols[i] : '-');
    }
    int owner = (mode & S_IRUSR ? 4 : 0) + (mode & S_IWUSR ? 2 : 0) + (mode & S_IXUSR ? 1 : 0);
    int group = (mode & S_IRGRP ? 4 : 0) + (mode & S_IWGRP ? 2 : 0) + (mode & S_IXGRP ? 1 : 0);
    int others = (mode & S_IROTH ? 4 : 0) + (mode & S_IWOTH ? 2 : 0) + (mode & S_IXOTH ? 1 : 0);

    printf("(%o)", (owner * 64) + (group * 8) + others);
}


void traverseDirectory(const char *basePath, Config config, int currentDepth) {
    DIR *dir = opendir(basePath);
    if (!dir) {
        return;
    }

    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", basePath, dp->d_name);

        struct stat statbuf;
        if (stat(path, &statbuf) != 0) {
            continue;
        }
        if (config.fileCategoryVar) {
            if (config.fileFormat == 'd' && !S_ISDIR(statbuf.st_mode)) {
                continue; 
            }
        }
        if (config.substringPattern && strstr(dp->d_name, config.searchPattern) == NULL) {
            continue;
        }
        if (config.sizeRestrictionFlag && S_ISREG(statbuf.st_mode) && statbuf.st_size <= config.sizeValue) {
            continue;
        }
        for (int i = 0; i < currentDepth; i++) {
            printf("\t");
        }
        printf("%s", dp->d_name);
        if (S_ISDIR(statbuf.st_mode)) {
            printf("/");
        }
        if (config.detailedAnalysisVar) {
            printf(" ");
            displayDataFormatted(&statbuf);
        } else {
            printf("\n");
        }
        if (S_ISDIR(statbuf.st_mode)) {
            if (config.subfolderLevel == -1 || currentDepth < config.subfolderLevel) {
                traverseDirectory(path, config, currentDepth + 1);
            }
        }
    }

    closedir(dir);
}


void displayDataFormatted(struct stat *statbuf) {
    printf("Size: %" PRIdMAX " bytes", (intmax_t)statbuf->st_size);
    printf(", Permissions: ");
    FilePErmissionsFetcher(statbuf->st_mode);
    printf(", Last Modified: ");
    struct tm *tm = localtime(&statbuf->st_mtime);
    if (tm) {
        char timeBuf[50]; 
        if (strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", tm)) {
            printf("%s", timeBuf);
        } else {
            printf("Unknown"); 
        }
    } else {
        printf("Unknown"); 
    }
    printf("\n");
}



void parseArgs(int argc, char **argv, Config *config) {
    config->sizeRestrictionFlag = 0;
    config->substringPattern = 0;
    config->fileCategoryVar = 0;
    config->detailedAnalysisVar = 0;
    config->sizeValue = 0;
    config->subfolderLevel = -1;
    config->searchPattern[0] = '\0';
    config->fileFormat = '\0';

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'L':
                    if (i + 1 < argc) {
                        config->sizeRestrictionFlag = 1;
                        config->sizeValue = atoll(argv[++i]); 
                    break;
                case 's':
                    if (i + 1 < argc) {
                        config->substringPattern = 1;
                        strncpy(config->searchPattern, argv[++i], sizeof(config->searchPattern) - 1);
                        config->searchPattern[sizeof(config->searchPattern) - 1] = '\0';
                    }
                    break;
                case 't':
                    if (i + 1 < argc) {
                        config->fileCategoryVar = 1;
                        config->fileFormat = argv[++i][0];
                        // printf("Type flag set to: %c\n", config->fileFormat); 
                    } else {
                        printf("Expected fileFormat after -t but none found.\n");
                    }
                    break;
                case 'd':
                    if (i + 1 < argc) {
                        config->subfolderLevel = atoi(argv[++i]);
                    }
                    break;
                case 'v':
                    config->detailedAnalysisVar = 1;
                    break;
                default:
                    printf("Unknown option: %c\n", argv[i][1]);
                    return; 
            }
        }
    }
    }
}


int main(int argc, char **argv) {
    Config config = {0};
    parseArgs(argc, argv, &config);

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        traverseDirectory(cwd, config, 0);
    } else {
        perror("getcwd() error");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
