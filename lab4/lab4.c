#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 2048

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    char *file1 = argv[1];
    char *file2 = argv[2];

    if (strcmp(file1, file2) == 0) {
        fprintf(stderr, "Error: Both Input filenames are the same.\n");
        return 1;
    }

    int file_description_information1 = open(file1, O_WRONLY | O_APPEND);
    if (file_description_information1 == -1) {
        perror("Error opening file1");
        return 1;
    }

    int file_description_information2 = open(file2, O_RDONLY);
    if (file_description_information2 == -1) {
        perror("Error opening file2");
        close(file_description_information1);
        return 1;
    }

    char memory_buffer[BUFFER_SIZE];
    ssize_t file_data_read;

    while ((file_data_read = read(file_description_information2, memory_buffer, sizeof(memory_buffer))) > 0) {
        if (write(file_description_information1, memory_buffer, file_data_read) != file_data_read) {
            perror("Error writing to file1");
            close(file_description_information1);
            close(file_description_information2);
            return 1;
        }
    }

    if (file_data_read == -1) {
        perror("Error reading from file2");
        close(file_description_information1);
        close(file_description_information2);
        return 1;
    }

    close(file_description_information1);
    close(file_description_information2);

    printf("Contents of %s have been concatenated with %s successfully.\n", file2, file1);

    return 0;
}
