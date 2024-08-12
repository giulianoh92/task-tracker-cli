#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/task.h"

Task *create_task(int id, const char *description, int status, const char *createdAt, const char *updatedAt) {
    Task *task = (Task *)malloc(sizeof(Task));
    if (!task) return NULL;

    task->id = id;
    task->description = strdup(description);
    task->status = status;
    task->createdAt = strdup(createdAt);
    task->updatedAt = strdup(updatedAt);
    
    return task;
}

void free_task(Task *task) {
    if (task) {
        free(task->description);
        free(task->createdAt);
        free(task->updatedAt);
        free(task);
    }
}
