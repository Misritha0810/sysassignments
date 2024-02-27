#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024
#define LOG_FILE "output.log"

void store_log_details(const char *cmd, const char *start_time, const char *end_time) {
    FILE *log = fopen(LOG_FILE, "a");
    if (log == NULL) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }

    fprintf(log, "%s\t%s\t%s\n", cmd, start_time, end_time);
    fclose(log);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_input_data>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file_input_data = fopen(argv[1], "r");
    if (file_input_data == NULL) {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    char cmd[MAX_COMMAND_LENGTH];
    while (fgets(cmd, sizeof(cmd), file_input_data) != NULL) {
        size_t len_of_cmd = strlen(cmd);
        if (len_of_cmd > 0 && cmd[len_of_cmd - 1] == '\n')
            cmd[len_of_cmd - 1] = '\0';

        printf("Executing cmd: %s\n", cmd); 

        struct timeval start, end;
        gettimeofday(&start, NULL);
        pid_t process_id = fork();
        if (process_id < 0) {
            perror("fork");
            return EXIT_FAILURE;
        } else if (process_id == 0) { 
            char *args[10]; 
            int arg_count = 0;
            char *token_of_cmd = strtok(cmd, " ");
            while (token_of_cmd != NULL && arg_count < 10) {
                args[arg_count++] = token_of_cmd;
                token_of_cmd = strtok(NULL, " ");
            }
            args[arg_count] = NULL;
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else { 
            int cmd_exec_status;
            waitpid(process_id, &cmd_exec_status, 0);
            gettimeofday(&end, NULL);
            char start_time[20], end_time[20];
            strftime(start_time, sizeof(start_time), "%c", localtime(&start.tv_sec));
            strftime(end_time, sizeof(end_time), "%c", localtime(&end.tv_sec));
            store_log_details(cmd, start_time, end_time);
        }
    }
    fclose(file_input_data);
    return EXIT_SUCCESS;
}
