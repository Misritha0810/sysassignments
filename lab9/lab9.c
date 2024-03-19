#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

pid_t child_process_id = -1;

void handleSignal(int signalNumber) {
    // Handle signals based on signal number
    switch (signalNumber) {
        case SIGINT:
            // Forward SIGINT signal to the child process
            if (child_process_id > 0) {
                printf("Parent: Received SIGINT (Ctrl+C), forwarding to child PID %d\n", child_process_id);
                kill(child_process_id, SIGINT);
            }
            break;
        case SIGTSTP:
            // Forward SIGTSTP signal to the child process
            if (child_process_id > 0) {
                printf("Parent: Received SIGTSTP (Ctrl+Z), forwarding to child PID %d\n", child_process_id);
                kill(child_process_id, SIGTSTP);
            }
            break;
        case SIGQUIT:
            // Handling SIGQUIT signal (Control-\)
            printf("\nParent: Received SIGQUIT (Ctrl+\\), parent process PID %d will now terminate.\n", getpid());
            // If the child process is still running, terminate it first
            if (child_process_id > 0) {
                printf("Parent: Terminating child process PID %d\n", child_process_id);
                kill(child_process_id, SIGTERM);
            }
            exit(EXIT_SUCCESS);
            break;
        default:
            break;
    }
}

int main(int argc, char **argv) {
    int status;

    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Setting up signal handlers in the parent process
    signal(SIGINT, handleSignal);   // Handling Control-C
    signal(SIGTSTP, handleSignal);  // Handling Control-Z
    signal(SIGQUIT, handleSignal);  // Handling Control-\

    child_process_id = fork();
    if (child_process_id == 0) { // Child process
        printf("Child process started with PID %d, executing command: %s\n", getpid(), argv[1]);
        execvp(argv[1], &argv[1]);
        // If execvp returns, it failed
        printf("If you see this statement, execvp failed ;-(\n");
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (child_process_id > 0) { // Parent process
        printf("Parent process PID %d, waiting for child process PID %d to terminate\n", getpid(), child_process_id);

        // The parent process waits for the child process to terminate
        waitpid(child_process_id, &status, 0); 

        if (WIFEXITED(status)) {
            printf("Child process PID %d exited with status = %d\n", child_process_id, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process PID %d terminated by signal %d\n", child_process_id, WTERMSIG(status));
        }
    } else { // Error occurred during fork()
        perror("fork");
        exit(EXIT_FAILURE);
    }

    printf("Parent process PID %d: Exiting program.\n", getpid());
    return 0;
}
