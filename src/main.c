#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/command.h"

int main(int argc, char *argv[]) {
    char input[256];
    char *args[10];
    int argCount;

    while (1) {
        printf("task-tracker> ");
        if (!fgets(input, sizeof(input), stdin)) {
            break; // Exit on EOF (Ctrl+D)
        }

        // Remove newline character
        input[strcspn(input, "\n")] = 0;

        // Tokenize input while respecting quoted strings
        argCount = 0;
        char *start = input;
        while (*start != '\0' && argCount < 10) {
            // Skip leading spaces
            while (*start == ' ') {
                start++;
            }

            if (*start == '"') {
                // Handle quoted string
                start++;
                char *end = strchr(start, '"');
                if (end != NULL) {
                    *end = '\0'; // Remove closing quote
                    args[argCount] = start;
                    start = end + 1;
                } else {
                    // Handle case where closing quote is missing
                    fprintf(stderr, "Error: Missing closing quote\n");
                    break;
                }
            } else {
                // Handle unquoted string
                char *end = strchr(start, ' ');
                if (end != NULL) {
                    *end = '\0';
                    args[argCount] = start;
                    start = end + 1;
                } else {
                    args[argCount] = start;
                    start += strlen(start);
                }
            }

            argCount++;
        }

        if (argCount == 0) {
            continue; // Skip empty input
        }

        // Parse and execute command
        CommandType cmdType = parseCommandLineArguments(argCount, args);
        if (cmdType == CMD_EXIT) {
            break; 
        }
        executeCommand(cmdType, argCount, args);
    }

    return 0;
}