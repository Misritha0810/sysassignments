#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    char command[256];

    while (1) {
        printf("Enter command>> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "quit") == 0) {
            printf("Exiting program...bye!\n");
            break;
        }

        FILE *fp = popen(command, "r");
        if (fp == NULL) {
            perror("Error executing command");
            continue;
        }

        char output[256];
        while (fgets(output, sizeof(output), fp) != NULL) {
            printf("%s", output);
        }

        pclose(fp);
    }

    return 0;
}
