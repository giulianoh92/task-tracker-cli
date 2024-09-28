#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../include/command.h"

#define HISTORY_FILE ".task_tracker_history"

int main(int argc, char *argv[]) {
    char *input;
    char *args[10];
    int argCount;

    // Load history from file
    read_history(HISTORY_FILE);

    while (1) {
        input = readline("task-tracker> ");
        if (!input) {
            break; // Exit on EOF (Ctrl+D)
        }

        // Add input to history
        add_history(input);

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
            free(input);
            continue; // Skip empty input
        }

        // Parse and execute command
        CommandType cmdType = parseCommandLineArguments(argCount, args);
        if (cmdType == CMD_EXIT) {
            free(input);
            break; 
        }
        executeCommand(cmdType, argCount, args);
        free(input);
    }

    // Save history to file
    write_history(HISTORY_FILE);

    return 0;
}