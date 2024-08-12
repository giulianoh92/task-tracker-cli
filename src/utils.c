#include <stdio.h>
#include "../include/utils.h"

void printHelp() {
    printf("Task Manager\n");
    printf("Usage:\n");
    printf("  add <description> \t\t - Add a new task\n");
    printf("  update <id> <description> \t - Update a task description\n");
    printf("  delete <id> \t\t\t - Delete a task\n");
    printf("  mark-in-progress <id> \t - Mark task as in-progress\n");
    printf("  mark-done <id> \t\t - Mark task as done\n");
    printf("  list \t\t\t\t - List all tasks\n");
    printf("  list <status> \t\t - List tasks by status (todo, in-progress, done)\n");
}
