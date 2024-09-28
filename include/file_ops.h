#ifndef FILE_OPS_H
#define FILE_OPS_H

#include "task.h"
#include "status.h"

void createTask(const char *description);
void insertTask(Task *newTask);
void listTasksWhere(Status *status);
void deleteTask(int *id);
void deleteAllTasks();
void updateTask(int *id, char *description);
void setTaskTo(int *id, Status *status);
int getLastID();
char *getTodayDate();

#endif // FILE_OPS_H
