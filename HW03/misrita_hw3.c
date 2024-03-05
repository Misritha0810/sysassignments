#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <inttypes.h>
#include <sys/wait.h>

/*
Name: Misritha Maridi
BlazerId: Mmaridi
Project #: Homework3
To compile: gcc -o search this_file_name.c
To run examples:
    ./search -L 1024 -e "ls -l"
    ./search -s jpg 3 -E "tar cvf jpg.tar"
    ./search -L 1024 -s jpg 3 -e "wc -l"

    ./search -e "wc -l"
    ./search -E "tar cvf test.tar"
    ./search -L 1024 -e "wc -l"
    ./search -L 1024 -E "tar cvf test.tar"
*/

typedef struct {
    int sizeRestrictionFlag, substringPattern, fileCategoryVar, detailedAnalysisVar, executeEachFlag, executeAllFlag;
    long long sizeValue;
    int subfolderLevel;
    char searchPattern[256], fileFormat, executeCommand[1024];
    char directoryPath[1024];

} Config;

void FilePermissionsFetcher(mode_t mode) {
    mode_t permissions[] = { S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH };
    char symbols[] = "rwxrwxrwx";
    for (int i = 0; i < sizeof(permissions) / sizeof(mode_t); i++) {
        putchar(mode & permissions[i] ? symbols[i] : '-');
    }
    printf("\n");
}

void displayDataFormatted(struct stat *statbuf) {
    printf("Size: %" PRIdMAX " bytes", (intmax_t)statbuf->st_size);
    printf(", Permissions: ");
    FilePermissionsFetcher(statbuf->st_mode);
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

void executeUnixCommand(char *command, char *filePath) {
    char cmd[1024];
    if (filePath != NULL) {
        // Ensure we do not exceed buffer size
        snprintf(cmd, sizeof(cmd), "%s %s", command, filePath);
    } else {
        strncpy(cmd, command, sizeof(cmd));
    }

    pid_t pid = fork();
    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        fprintf(stderr, "Failed to execute command: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        fprintf(stderr, "Fork failed: %s\n", strerror(errno));
    } else {
        wait(NULL);
    }
}

void traverseDirectory(const char *basePath, Config config, int currentDepth, char **fileList, int *fileCount) {
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
        if (config.fileCategoryVar && ((config.fileFormat == 'd' && !S_ISDIR(statbuf.st_mode)) || (config.fileFormat == 'f' && !S_ISREG(statbuf.st_mode)))) {
            continue;
        }
        if (config.substringPattern && strstr(dp->d_name, config.searchPattern) == NULL) {
            continue;
        }
        if (config.sizeRestrictionFlag && statbuf.st_size < config.sizeValue) {
            continue;
        }

        if (config.executeEachFlag || config.executeAllFlag) {
            if (*fileCount < 1024) { 
                fileList[*fileCount] = strdup(path);
                (*fileCount)++;
            }
        } else {
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
            }
            printf("\n");
        }

        if (S_ISDIR(statbuf.st_mode)) {
            if (config.subfolderLevel == -1 || currentDepth < config.subfolderLevel) {
                traverseDirectory(path, config, currentDepth + 1, fileList, fileCount);
            }
        }
    }

    closedir(dir);
}

void executeCommandForFiles(Config config, char **fileList, int fileCount) {
    if (config.executeEachFlag) {
        for (int i = 0; i < fileCount; i++) {
            executeUnixCommand(config.executeCommand, fileList[i]);
        }
    } else if (config.executeAllFlag) {
        char *allFilesCommand = malloc(strlen(config.executeCommand) + fileCount * 1024);
        strcpy(allFilesCommand, config.executeCommand);
        for (int i = 0; i < fileCount; i++) {
            strcat(allFilesCommand, " ");
            strcat(allFilesCommand, fileList[i]);
        }
        executeUnixCommand(allFilesCommand, NULL);
        free(allFilesCommand);
    }
}

void parseArgs(int argc, char **argv, Config *config) {
    // Initialize config with defaults
    memset(config, 0, sizeof(Config));
    config->subfolderLevel = -1;

    if (getcwd(config->directoryPath, sizeof(config->directoryPath)) == NULL) {
        perror("Failed to get current working directory");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'L':
                    config->sizeRestrictionFlag = 1;
                    config->sizeValue = atoll(argv[++i]);
                    break;
                case 's':
                    config->substringPattern = 1;
                    strncpy(config->searchPattern, argv[++i], sizeof(config->searchPattern) - 1);
                    config->searchPattern[sizeof(config->searchPattern) - 1] = '\0';
                    break;
                case 't':
                    config->fileCategoryVar = 1;
                    config->fileFormat = argv[++i][0];
                    break;
                case 'v':
                    config->detailedAnalysisVar = 1;
                    break;
                case 'd':
                    config->subfolderLevel = atoi(argv[++i]);
                    break;
                case 'e':
                    config->executeEachFlag = 1;
                    strncpy(config->executeCommand, argv[++i], sizeof(config->executeCommand) - 1);
                    break;
                case 'E':
                    config->executeAllFlag = 1;
                    strncpy(config->executeCommand, argv[++i], sizeof(config->executeCommand) - 1);
                    break;
                default:
                    fprintf(stderr, "Unknown option: %c\n", argv[i][1]);
                    exit(EXIT_FAILURE);
            }
        }else {
            strncpy(config->directoryPath, argv[i], sizeof(config->directoryPath) - 1);
            config->directoryPath[sizeof(config->directoryPath) - 1] = '\0';
        }

    }
    
}

int main(int argc, char **argv) {
    Config config;
    parseArgs(argc, argv, &config);

    char *fileList[1024];
    int fileCount = 0; 

    traverseDirectory(config.directoryPath, config, 0, fileList, &fileCount);
    if (config.executeEachFlag || config.executeAllFlag) {
        executeCommandForFiles(config, fileList, fileCount);
    }

    for (int i = 0; i < fileCount; i++) {
        free(fileList[i]);
    }

    return EXIT_SUCCESS;
}
