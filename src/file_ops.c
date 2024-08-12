#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/task.h"
#include "../include/status.h"
#include "../include/file_ops.h"

const char *filename = "bin/tasks.dat";

int getLastID() {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return 1;
    }

    Task task;
    int lastID = 0;

    while (fread(&task.id, sizeof(int), 1, fp)) {
        fread(&task.status, sizeof(int), 1, fp);

        int descriptionLength;
        fread(&descriptionLength, sizeof(int), 1, fp);
        task.description = (descriptionLength > 0) ? (char *)malloc(descriptionLength) : NULL;
        if (task.description) fread(task.description, sizeof(char), descriptionLength, fp);

        int createdAtLength;
        fread(&createdAtLength, sizeof(int), 1, fp);
        task.createdAt = (createdAtLength > 0) ? (char *)malloc(createdAtLength) : NULL;
        if (task.createdAt) fread(task.createdAt, sizeof(char), createdAtLength, fp);

        int updatedAtLength;
        fread(&updatedAtLength, sizeof(int), 1, fp);
        task.updatedAt = (updatedAtLength > 0) ? (char *)malloc(updatedAtLength) : NULL;
        if (task.updatedAt) fread(task.updatedAt, sizeof(char), updatedAtLength, fp);

        // Update lastID with the current task's ID
        if (task.id > lastID) {
            lastID = task.id;
        }

        // Free allocated memory
        free(task.description);
        free(task.createdAt);
        free(task.updatedAt);
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
        fprintf(stderr, "Error opening file\n");
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

    fclose(fp);
}

void listTasksWhere(Status *status) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return;
    }

    Task task;
    while (fread(&task.id, sizeof(int), 1, fp)) {
        fread(&task.status, sizeof(int), 1, fp);

        int descriptionLength;
        fread(&descriptionLength, sizeof(int), 1, fp);
        task.description = (descriptionLength > 0) ? (char *)malloc(descriptionLength) : NULL;
        if (task.description) fread(task.description, sizeof(char), descriptionLength, fp);

        int createdAtLength;
        fread(&createdAtLength, sizeof(int), 1, fp);
        task.createdAt = (createdAtLength > 0) ? (char *)malloc(createdAtLength) : NULL;
        if (task.createdAt) fread(task.createdAt, sizeof(char), createdAtLength, fp);

        int updatedAtLength;
        fread(&updatedAtLength, sizeof(int), 1, fp);
        task.updatedAt = (updatedAtLength > 0) ? (char *)malloc(updatedAtLength) : NULL;
        if (task.updatedAt) fread(task.updatedAt, sizeof(char), updatedAtLength, fp);

        if (!status || task.status == status->id) {
            printf("ID: %d, Description: %s, Status: %s, Created At: %s, Updated At: %s\n",
                task.id, task.description ? task.description : "N/A",
                getStatus(task.status), task.createdAt ? task.createdAt : "N/A",
                task.updatedAt ? task.updatedAt : "N/A");
        }

        free(task.description);
        free(task.createdAt);
        free(task.updatedAt);
    }

    fclose(fp);
}

void deleteTask(int *id) {
    FILE *fp = fopen(filename, "rb");
    FILE *temp_fp = fopen("temp.dat", "wb");
    if (!fp || !temp_fp) {
        fprintf(stderr, "Error opening file\n");
        if (fp) fclose(fp);
        if (temp_fp) fclose(temp_fp);
        return;
    }

    int found = 0;

    while (!feof(fp)) {
        Task task;
        int descriptionLength, createdAtLength, updatedAtLength;

        if (fread(&task.id, sizeof(int), 1, fp) != 1) break;
        fread(&task.status, sizeof(int), 1, fp);

        if (fread(&descriptionLength, sizeof(int), 1, fp) != 1) break;
        task.description = (char *)malloc(descriptionLength);
        if (!task.description) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(fp);
            fclose(temp_fp);
            return;
        }
        fread(task.description, sizeof(char), descriptionLength, fp);

        if (fread(&createdAtLength, sizeof(int), 1, fp) != 1) break;
        task.createdAt = (char *)malloc(createdAtLength);
        if (!task.createdAt) {
            fprintf(stderr, "Memory allocation failed\n");
            free(task.description);
            fclose(fp);
            fclose(temp_fp);
            return;
        }
        fread(task.createdAt, sizeof(char), createdAtLength, fp);

        if (fread(&updatedAtLength, sizeof(int), 1, fp) != 1) break;
        task.updatedAt = (char *)malloc(updatedAtLength);
        if (!task.updatedAt) {
            fprintf(stderr, "Memory allocation failed\n");
            free(task.description);
            free(task.createdAt);
            fclose(fp);
            fclose(temp_fp);
            return;
        }
        fread(task.updatedAt, sizeof(char), updatedAtLength, fp);

        if (task.id == *id) {
            found = 1;
        } else {
            fwrite(&task.id, sizeof(int), 1, temp_fp);
            fwrite(&task.status, sizeof(int), 1, temp_fp);
            fwrite(&descriptionLength, sizeof(int), 1, temp_fp);
            fwrite(task.description, sizeof(char), descriptionLength, temp_fp);
            fwrite(&createdAtLength, sizeof(int), 1, temp_fp);
            fwrite(task.createdAt, sizeof(char), createdAtLength, temp_fp);
            fwrite(&updatedAtLength, sizeof(int), 1, temp_fp);
            fwrite(task.updatedAt, sizeof(char), updatedAtLength, temp_fp);
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
    } else {
        printf("Task with ID %d not found.\n", *id);
        remove("temp.dat");
    }
}

void updateTask(int *id, char *description) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Error opening file for reading\n");
        return;
    }

    // Temporary file for writing updated tasks
    FILE *temp_fp = fopen("temp.dat", "wb");
    if (!temp_fp) {
        fprintf(stderr, "Error opening temporary file for writing\n");
        fclose(fp);
        return;
    }

    while (!feof(fp)) {
        Task task;
        int descriptionLength, createdAtLength, updatedAtLength;

        long pos = ftell(fp);

        if (fread(&task.id, sizeof(int), 1, fp) != 1) break;
        fread(&task.status, sizeof(int), 1, fp);

        if (fread(&descriptionLength, sizeof(int), 1, fp) != 1) break;
        task.description = (char *)malloc(descriptionLength);
        if (!task.description) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(fp);
            fclose(temp_fp);
            return;
        }
        fread(task.description, sizeof(char), descriptionLength, fp);

        if (fread(&createdAtLength, sizeof(int), 1, fp) != 1) break;
        task.createdAt = (char *)malloc(createdAtLength);
        if (!task.createdAt) {
            fprintf(stderr, "Memory allocation failed\n");
            free(task.description);
            fclose(fp);
            fclose(temp_fp);
            return;
        }
        fread(task.createdAt, sizeof(char), createdAtLength, fp);

        if (fread(&updatedAtLength, sizeof(int), 1, fp) != 1) break;
        task.updatedAt = (char *)malloc(updatedAtLength);
        if (!task.updatedAt) {
            fprintf(stderr, "Memory allocation failed\n");
            free(task.description);
            free(task.createdAt);
            fclose(fp);
            fclose(temp_fp);
            return;
        }
        fread(task.updatedAt, sizeof(char), updatedAtLength, fp);

        // If the task ID matches, update the task
        if (task.id == *id) {
            free(task.description);
            task.description = strdup(description);

            descriptionLength = strlen(task.description) + 1;
            fwrite(&task.id, sizeof(int), 1, temp_fp);
            fwrite(&task.status, sizeof(int), 1, temp_fp);

            fwrite(&descriptionLength, sizeof(int), 1, temp_fp);
            fwrite(task.description, sizeof(char), descriptionLength, temp_fp);

            createdAtLength = strlen(task.createdAt) + 1;
            fwrite(&createdAtLength, sizeof(int), 1, temp_fp);
            fwrite(task.createdAt, sizeof(char), createdAtLength, temp_fp);

            updatedAtLength = strlen(task.updatedAt) + 1;
            fwrite(&updatedAtLength, sizeof(int), 1, temp_fp);
            fwrite(task.updatedAt, sizeof(char), updatedAtLength, temp_fp);

            free(task.description);
            free(task.createdAt);
            free(task.updatedAt);
        } else {
            // Write the task as is to the temporary file
            fwrite(&task.id, sizeof(int), 1, temp_fp);
            fwrite(&task.status, sizeof(int), 1, temp_fp);

            descriptionLength = strlen(task.description) + 1;
            fwrite(&descriptionLength, sizeof(int), 1, temp_fp);
            fwrite(task.description, sizeof(char), descriptionLength, temp_fp);

            createdAtLength = strlen(task.createdAt) + 1;
            fwrite(&createdAtLength, sizeof(int), 1, temp_fp);
            fwrite(task.createdAt, sizeof(char), createdAtLength, temp_fp);

            updatedAtLength = strlen(task.updatedAt) + 1;
            fwrite(&updatedAtLength, sizeof(int), 1, temp_fp);
            fwrite(task.updatedAt, sizeof(char), updatedAtLength, temp_fp);

            free(task.description);
            free(task.createdAt);
            free(task.updatedAt);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    // Replace the original file with the temporary file
    remove(filename);
    rename("temp.dat", filename);
}

void setTaskTo(int *id, Status *status) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Error opening file for reading\n");
        return;
    }

    // Temporary file for writing updated tasks
    FILE *temp_fp = fopen("temp.dat", "wb");
    if (!temp_fp) {
        fprintf(stderr, "Error opening temporary file for writing\n");
        fclose(fp);
        return;
    }

    while (!feof(fp)) {
        Task task;
        int descriptionLength, createdAtLength, updatedAtLength;

        long pos = ftell(fp);

        if (fread(&task.id, sizeof(int), 1, fp) != 1) break;
        fread(&task.status, sizeof(int), 1, fp);

        if (fread(&descriptionLength, sizeof(int), 1, fp) != 1) break;
        task.description = (char *)malloc(descriptionLength);
        if (!task.description) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(fp);
            fclose(temp_fp);
            return;
        }
        fread(task.description, sizeof(char), descriptionLength, fp);

        if (fread(&createdAtLength, sizeof(int), 1, fp) != 1) break;
        task.createdAt = (char *)malloc(createdAtLength);
        if (!task.createdAt) {
            fprintf(stderr, "Memory allocation failed\n");
            free(task.description);
            fclose(fp);
            fclose(temp_fp);
            return;
        }
        fread(task.createdAt, sizeof(char), createdAtLength, fp);

        if (fread(&updatedAtLength, sizeof(int), 1, fp) != 1) break;
        task.updatedAt = (char *)malloc(updatedAtLength);
        if (!task.updatedAt) {
            fprintf(stderr, "Memory allocation failed\n");
            free(task.description);
            free(task.createdAt);
            fclose(fp);
            fclose(temp_fp);
            return;
        }
        fread(task.updatedAt, sizeof(char), updatedAtLength, fp);

        // If the task ID matches, update the task
        if (task.id == *id) {
            task.status = status->id;

            descriptionLength = strlen(task.description) + 1;
            fwrite(&task.id, sizeof(int), 1, temp_fp);
            fwrite(&task.status, sizeof(int), 1, temp_fp);

            fwrite(&descriptionLength, sizeof(int), 1, temp_fp);
            fwrite(task.description, sizeof(char), descriptionLength, temp_fp);

            createdAtLength = strlen(task.createdAt) + 1;
            fwrite(&createdAtLength, sizeof(int), 1, temp_fp);
            fwrite(task.createdAt, sizeof(char), createdAtLength, temp_fp);

            if (strcmp(status->status,"done") == 0)
            {
                free(task.updatedAt);
                task.updatedAt = getTodayDate();
            }

            updatedAtLength = strlen(task.updatedAt) + 1;
            fwrite(&updatedAtLength, sizeof(int), 1, temp_fp);
            fwrite(task.updatedAt, sizeof(char), updatedAtLength, temp_fp);

            free(task.description);
            free(task.createdAt);
            free(task.updatedAt);
        } else {
            // Write the task as is to the temporary file
            fwrite(&task.id, sizeof(int), 1, temp_fp);
            fwrite(&task.status, sizeof(int), 1, temp_fp);

            descriptionLength = strlen(task.description) + 1;
            fwrite(&descriptionLength, sizeof(int), 1, temp_fp);
            fwrite(task.description, sizeof(char), descriptionLength, temp_fp);

            createdAtLength = strlen(task.createdAt) + 1;
            fwrite(&createdAtLength, sizeof(int), 1, temp_fp);
            fwrite(task.createdAt, sizeof(char), createdAtLength, temp_fp);

            updatedAtLength = strlen(task.updatedAt) + 1;
            fwrite(&updatedAtLength, sizeof(int), 1, temp_fp);
            fwrite(task.updatedAt, sizeof(char), updatedAtLength, temp_fp);

            free(task.description);
            free(task.createdAt);
            free(task.updatedAt);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    // Replace the original file with the temporary file
    remove(filename);
    rename("temp.dat", filename);
}
