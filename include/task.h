#ifndef TASK_H
#define TASK_H

#define DATE_STRING_SIZE 20

typedef struct Task {
    int id;
    char *description;
    int status;
    char *createdAt;
    char *updatedAt;
} Task;

Task *create_task(int id, const char *description, int status, const char *createdAt, const char *updatedAt);
void free_task(Task *task);

#endif // TASK_H
