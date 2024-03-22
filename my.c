#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void wordCount(char **args) {
    const char *option = args[1];
    const char *file_path = args[2];

    // Open the file for reading
    FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'\n", file_path);
        return;
    }

    int lines = 0;
    int words = 0;

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        lines++;

        // Count words in each line
        char *token = strtok(buffer, " \t\n");
        while (token != NULL) {
            words++;
            token = strtok(NULL, " \t\n");
        }
    }

    // Close the file
    fclose(file);

    // Print result based on option
    if (strcmp(option, "-l") == 0) {
        printf("Number of lines in '%s': %d\n", file_path, lines);
    } else if (strcmp(option, "-w") == 0) {
        printf("Number of words in '%s': %d\n", file_path, words);
    } else {
        fprintf(stderr, "Error: Invalid option '%s'\n", option);
        return;
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <option> <file_path>\n", argv[0]);
        return 1;
    }

    wordCount(argv);

    return 0;
}