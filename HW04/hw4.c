#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define MAX_JOBS 100

typedef struct {
    pid_t pid;
    char command[256];
    char status[20];  
    char outfile[50];
    char errfile[50];
    int jobid;
    time_t startTime;
    time_t endTime;
} Job;

Job jobQueue[MAX_JOBS];
int jobCount = 0;
int currentJobs = 0;
int maxJobs;

typedef void (*CommandFunc)(char *);

typedef struct {
    char *commandName;
    CommandFunc func;
} CommandHandler;

void submit(char *command);
void showjobs();
void submithistory();
void executeJob(int jobIndex);
void checkJobs();
void updateJobStatus(int jobIndex, int status);
char* formatTime(time_t rawtime);
void tryExecuteWaitingJobs();
void unknownCommand(char *command);

CommandHandler commandHandlers[] = {
    {"submit ", submit},
    {"showjobs", showjobs},
    {"submithistory", submithistory},
    {NULL, NULL}
};

void executeCommand(char *input) {
    for (int i = 0; commandHandlers[i].commandName != NULL; i++) {
        int cmdLen = strlen(commandHandlers[i].commandName);
        if (strncmp(input, commandHandlers[i].commandName, cmdLen) == 0) {
            commandHandlers[i].func(input + cmdLen);
            return;
        }
    }
    unknownCommand(input);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_parallel_jobs>\n", argv[0]);
        return 1;
    }

    maxJobs = atoi(argv[1]);
    memset(jobQueue, 0, sizeof(jobQueue));

    char input[256];
    printf("Enter command> ");
    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;
        checkJobs();
        executeCommand(input);
        printf("Enter command> ");
        tryExecuteWaitingJobs();
    }
    return 0;
}

void submit(char *command) {
    if (jobCount >= MAX_JOBS) {
        printf("Job queue is full, cannot submit more jobs.\n");
        return;
    }
    int index = jobCount++;
    snprintf(jobQueue[index].command, sizeof(jobQueue[index].command), "%s", command);
    snprintf(jobQueue[index].status, sizeof(jobQueue[index].status), "Waiting");
    snprintf(jobQueue[index].outfile, sizeof(jobQueue[index].outfile), "job%d_out.out", index);
    snprintf(jobQueue[index].errfile, sizeof(jobQueue[index].errfile), "job%d_err.err", index);
    jobQueue[index].jobid = index + 1;
    jobQueue[index].startTime = time(NULL);
    if (currentJobs < maxJobs) {
        executeJob(index);
        printf("Job %d submitted and is now running.\n", index + 1);
    } else {
        printf("Job %d submitted but is waiting for execution.\n", index + 1);
    }
}

void executeJob(int jobIndex) {
    if (strcmp(jobQueue[jobIndex].status, "Waiting") == 0 && currentJobs < maxJobs) {
        pid_t pid = fork();
        if (pid == 0) { // Child process
            freopen(jobQueue[jobIndex].outfile, "w", stdout);
            freopen(jobQueue[jobIndex].errfile, "w", stderr);
            execlp("/bin/sh", "sh", "-c", jobQueue[jobIndex].command, NULL);
            exit(EXIT_FAILURE); // If execlp fails
        } else if (pid > 0) { // Parent process
            jobQueue[jobIndex].pid = pid;
            snprintf(jobQueue[jobIndex].status, sizeof(jobQueue[jobIndex].status), "Running");
            currentJobs++;
        } else {
            fprintf(stderr, "Failed to fork.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void checkJobs() {
    int status;
    for (int i = 0; i < jobCount; i++) {
        if (jobQueue[i].pid != 0 && strcmp(jobQueue[i].status, "Running") == 0 && waitpid(jobQueue[i].pid, &status, WNOHANG) > 0) {
            updateJobStatus(i, status);
        }
    }
}

void updateJobStatus(int jobIndex, int status) {
    jobQueue[jobIndex].endTime = time(NULL);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        snprintf(jobQueue[jobIndex].status, sizeof(jobQueue[jobIndex].status), "Success");
    } else {
        snprintf(jobQueue[jobIndex].status, sizeof(jobQueue[jobIndex].status), "Failed");
    }
    currentJobs--;
}

void showjobs() {
    for (int i = 0; i < jobCount; i++) {
        if (strcmp(jobQueue[i].status, "Running") == 0 || strcmp(jobQueue[i].status, "Waiting") == 0) {
            printf("Jobid: %d, Command: '%s', Status: %s\n", jobQueue[i].jobid, jobQueue[i].command, jobQueue[i].status);
        }
    }
}

void submithistory() {
    printf("Job History:\n");
    for (int i = 0; i < jobCount; i++) {
        printf("Jobid: %d, Command: '%s', Start: %s, End: %s, Status: %s\n",
               jobQueue[i].jobid, jobQueue[i].command, formatTime(jobQueue[i].startTime), formatTime(jobQueue[i].endTime), jobQueue[i].status);
    }
}

char* formatTime(time_t rawtime) {
    static char buffer[30];
    struct tm *timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}

void tryExecuteWaitingJobs() {
    for (int i = 0; i < jobCount; i++) {
        if (strcmp(jobQueue[i].status, "Waiting") == 0) {
            executeJob(i);
        }
    }
}

void unknownCommand(char *command) {
    printf("Unknown command: '%s'\n", command);
}
