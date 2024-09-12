#include <stdio.h>
#include "../include/utils.h"

void printHelp() {
    printf("Task Manager\n");
    printf("Usage:\n");
    printf("  add <description>              - Add a new task\n");
    printf("  update <id> <description>      - Update a task description\n");
    printf("  delete <id>                    - Delete a task\n");
    printf("  mark-in-progress <id>          - Mark task as in-progress\n");
    printf("  mark-done <id>                 - Mark task as done\n");
    printf("  list                           - List all tasks\n");
    printf("  list <status>                  - List tasks by status (todo, in-progress, done)\n");
    printf("  help                           - Show this help message\n");
    printf("  exit                           - Exit the program\n");
}