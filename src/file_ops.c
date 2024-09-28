#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "../include/task.h"
#include "../include/status.h"
#include "../include/file_ops.h"

const char *filename = "bin/tasks.dat";

int getLastID() {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Failed to open file");
        return 0;
    }

    Task task;
    int lastID = 0;

    while (fread(&task.id, sizeof(int), 1, fp)) {
        // Read the rest of the task to move the file pointer correctly
        fread(&task.status, sizeof(int), 1, fp);

        int descriptionLength;
        fread(&descriptionLength, sizeof(int), 1, fp);
        fseek(fp, descriptionLength, SEEK_CUR);

        int createdAtLength;
        fread(&createdAtLength, sizeof(int), 1, fp);
        fseek(fp, createdAtLength, SEEK_CUR);

        int updatedAtLength;
        fread(&updatedAtLength, sizeof(int), 1, fp);
        fseek(fp, updatedAtLength, SEEK_CUR);

        lastID = task.id;
    }

    fclose(fp);
    return lastID + 1;  // Return the next ID, which is the last ID + 1
}

char *getTodayDate() {
    time_t t;
    struct tm *tm_info;
    char *buffer = (char *)malloc(DATE_STRING_SIZE);
    
    if (!buffer) return NULL;

    time(&t);
    tm_info = localtime(&t);
    strftime(buffer, DATE_STRING_SIZE, "%Y-%m-%d", tm_info);
    
    return buffer;
}

void createTask(const char *description) {
    Task *newTask = create_task(getLastID(), description, 1, getTodayDate(), "");
    if (newTask) {
        insertTask(newTask);
        free_task(newTask);
    }
}

void insertTask(Task *newTask) {
    FILE *fp = fopen(filename, "ab");
    if (!fp) {
        perror("Failed to open file");
        return;
    }
    
    fwrite(&newTask->id, sizeof(int), 1, fp);
    fwrite(&newTask->status, sizeof(int), 1, fp);

    int descriptionLength = newTask->description ? strlen(newTask->description) + 1 : 0;
    fwrite(&descriptionLength, sizeof(int), 1, fp);
    if (descriptionLength > 0) fwrite(newTask->description, sizeof(char), descriptionLength, fp);

    int createdAtLength = newTask->createdAt ? strlen(newTask->createdAt) + 1 : 0;
    fwrite(&createdAtLength, sizeof(int), 1, fp);
    if (createdAtLength > 0) fwrite(newTask->createdAt, sizeof(char), createdAtLength, fp);

    int updatedAtLength = newTask->updatedAt ? strlen(newTask->updatedAt) + 1 : 0;
    fwrite(&updatedAtLength, sizeof(int), 1, fp);
    if (updatedAtLength > 0) fwrite(newTask->updatedAt, sizeof(char), updatedAtLength, fp);

    printf("Task created successfully.\n");

    fclose(fp);
}

void listTasksWhere(Status *status) {
    bool tasksFound = false;
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Failed to open file");
        return;
    }

    Task task;
    while (fread(&task.id, sizeof(int), 1, fp)) {
        tasksFound = true;
        fread(&task.status, sizeof(int), 1, fp);

        int descriptionLength;
        fread(&descriptionLength, sizeof(int), 1, fp);
        task.description = (char *)malloc(descriptionLength);
        fread(task.description, sizeof(char), descriptionLength, fp);

        int createdAtLength;
        fread(&createdAtLength, sizeof(int), 1, fp);
        task.createdAt = (char *)malloc(createdAtLength);
        fread(task.createdAt, sizeof(char), createdAtLength, fp);

        int updatedAtLength;
        fread(&updatedAtLength, sizeof(int), 1, fp);
        task.updatedAt = (char *)malloc(updatedAtLength);
        fread(task.updatedAt, sizeof(char), updatedAtLength, fp);

        if (!status || task.status == status->id) {
            printf("ID: %d, Description: %s, Status: %s, Created At: %s, Updated At: %s\n",
                   task.id, task.description, getStatus(task.status), task.createdAt, task.updatedAt);
        }

        free(task.description);
        free(task.createdAt);
        free(task.updatedAt);
    }

    if (!tasksFound) {
        printf("No tasks found.\n");
    }

    fclose(fp);
}

void deleteTask(int *id) {
    FILE *fp = fopen(filename, "rb");
    FILE *temp_fp = fopen("temp.dat", "wb");
    if (!fp || !temp_fp) {
        perror("Failed to open file");
        if (fp) fclose(fp);
        if (temp_fp) fclose(temp_fp);
        return;
    }

    int found = 0;
    Task task;

    while (fread(&task.id, sizeof(int), 1, fp)) {
        fread(&task.status, sizeof(int), 1, fp);

        int descriptionLength;
        fread(&descriptionLength, sizeof(int), 1, fp);
        task.description = (char *)malloc(descriptionLength);
        fread(task.description, sizeof(char), descriptionLength, fp);

        int createdAtLength;
        fread(&createdAtLength, sizeof(int), 1, fp);
        task.createdAt = (char *)malloc(createdAtLength);
        fread(task.createdAt, sizeof(char), createdAtLength, fp);

        int updatedAtLength;
        fread(&updatedAtLength, sizeof(int), 1, fp);
        task.updatedAt = (char *)malloc(updatedAtLength);
        fread(task.updatedAt, sizeof(char), updatedAtLength, fp);

        if (task.id != *id) {
            fwrite(&task.id, sizeof(int), 1, temp_fp);
            fwrite(&task.status, sizeof(int), 1, temp_fp);
            fwrite(&descriptionLength, sizeof(int), 1, temp_fp);
            fwrite(task.description, sizeof(char), descriptionLength, temp_fp);
            fwrite(&createdAtLength, sizeof(int), 1, temp_fp);
            fwrite(task.createdAt, sizeof(char), createdAtLength, temp_fp);
            fwrite(&updatedAtLength, sizeof(int), 1, temp_fp);
            fwrite(task.updatedAt, sizeof(char), updatedAtLength, temp_fp);
        } else {
            found = 1;
        }

        free(task.description);
        free(task.createdAt);
        free(task.updatedAt);
    }

    fclose(fp);
    fclose(temp_fp);

    if (found) {
        remove(filename);
        rename("temp.dat", filename);

        printf("Task with ID %d deleted successfully.\n", *id);
    } else {
        remove("temp.dat");
        fprintf(stderr, "Task with ID %d not found.\n", *id);
    }
}

void deleteAllTasks() {
    remove(filename);
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open file");
        return;
    }
    fclose(fp);
    printf("All tasks deleted successfully.\n");
}

void updateTask(int *id, char *description) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Failed to open file");
        return;
    }

    FILE *temp_fp = fopen("temp.dat", "wb");
    if (!temp_fp) {
        perror("Failed to open temporary file");
        fclose(fp);
        return;
    }

    Task task;
    while (fread(&task.id, sizeof(int), 1, fp)) {
        fread(&task.status, sizeof(int), 1, fp);

        int descriptionLength;
        fread(&descriptionLength, sizeof(int), 1, fp);
        task.description = (char *)malloc(descriptionLength);
        fread(task.description, sizeof(char), descriptionLength, fp);

        int createdAtLength;
        fread(&createdAtLength, sizeof(int), 1, fp);
        task.createdAt = (char *)malloc(createdAtLength);
        fread(task.createdAt, sizeof(char), createdAtLength, fp);

        int updatedAtLength;
        fread(&updatedAtLength, sizeof(int), 1, fp);
        task.updatedAt = (char *)malloc(updatedAtLength);
        fread(task.updatedAt, sizeof(char), updatedAtLength, fp);

        if (task.id == *id) {
            free(task.description);
            task.description = strdup(description);
            free(task.updatedAt);
            task.updatedAt = getTodayDate();
        }

        fwrite(&task.id, sizeof(int), 1, temp_fp);
        fwrite(&task.status, sizeof(int), 1, temp_fp);
        descriptionLength = strlen(task.description) + 1;
        fwrite(&descriptionLength, sizeof(int), 1, temp_fp);
        fwrite(task.description, sizeof(char), descriptionLength, temp_fp);
        fwrite(&createdAtLength, sizeof(int), 1, temp_fp);
        fwrite(task.createdAt, sizeof(char), createdAtLength, temp_fp);
        updatedAtLength = strlen(task.updatedAt) + 1;
        fwrite(&updatedAtLength, sizeof(int), 1, temp_fp);
        fwrite(task.updatedAt, sizeof(char), updatedAtLength, temp_fp);

        free(task.description);
        free(task.createdAt);
        free(task.updatedAt);
    }

    fclose(fp);
    fclose(temp_fp);

    remove(filename);
    rename("temp.dat", filename);

    printf("Task with ID %d updated successfully.\n", *id);
}

void setTaskTo(int *id, Status *status) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Failed to open file");
        return;
    }

    FILE *temp_fp = fopen("temp.dat", "wb");
    if (!temp_fp) {
        perror("Failed to open temporary file");
        fclose(fp);
        return;
    }

    Task task;
    while (fread(&task.id, sizeof(int), 1, fp)) {
        fread(&task.status, sizeof(int), 1, fp);

        int descriptionLength;
        fread(&descriptionLength, sizeof(int), 1, fp);
        task.description = (char *)malloc(descriptionLength);
        fread(task.description, sizeof(char), descriptionLength, fp);

        int createdAtLength;
        fread(&createdAtLength, sizeof(int), 1, fp);
        task.createdAt = (char *)malloc(createdAtLength);
        fread(task.createdAt, sizeof(char), createdAtLength, fp);

        int updatedAtLength;
        fread(&updatedAtLength, sizeof(int), 1, fp);
        task.updatedAt = (char *)malloc(updatedAtLength);
        fread(task.updatedAt, sizeof(char), updatedAtLength, fp);

        if (task.id == *id) {
            task.status = status->id;
            free(task.updatedAt);
            task.updatedAt = getTodayDate();
        }

        fwrite(&task.id, sizeof(int), 1, temp_fp);
        fwrite(&task.status, sizeof(int), 1, temp_fp);
        fwrite(&descriptionLength, sizeof(int), 1, temp_fp);
        fwrite(task.description, sizeof(char), descriptionLength, temp_fp);
        fwrite(&createdAtLength, sizeof(int), 1, temp_fp);
        fwrite(task.createdAt, sizeof(char), createdAtLength, temp_fp);
        fwrite(&updatedAtLength, sizeof(int), 1, temp_fp);
        fwrite(task.updatedAt, sizeof(char), updatedAtLength, temp_fp);

        free(task.description);
        free(task.createdAt);
        free(task.updatedAt);
    }

    fclose(fp);
    fclose(temp_fp);

    remove(filename);
    rename("temp.dat", filename);

    printf("Task with ID %d set to %s successfully.\n", *id, status->status);
}

