#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/command.h"
#include "../include/task.h"
#include "../include/status.h"
#include "../include/file_ops.h"
#include "../include/utils.h"

CommandType parseCommandLineArguments(int argc, char *argv[]) {
    if (argc == 3) {
        if (strcmp(argv[1], "add") == 0) return CMD_ADD;
        if (strcmp(argv[1], "delete") == 0) return CMD_DELETE;
        if (strcmp(argv[1], "mark-in-progress") == 0) return CMD_MARK_IN_PROGRESS;
        if (strcmp(argv[1], "mark-done") == 0) return CMD_MARK_DONE;
        if (strcmp(argv[1], "list") == 0) {
            if (strcmp(argv[2], "done") == 0) return CMD_LIST_DONE;
            if (strcmp(argv[2], "todo") == 0) return CMD_LIST_TODO;
            if (strcmp(argv[2], "in-progress") == 0) return CMD_LIST_IN_PROGRESS;
        }
    } else if (argc == 2) {
        if (strcmp(argv[1], "list") == 0) return CMD_LIST;
    } else if (argc == 4) {
        if (strcmp(argv[1], "update") == 0) return CMD_UPDATE;
    }
    return CMD_UNKNOWN;
}

void executeCommand(CommandType cmdType, int argc, char *argv[]) {

    int taskId;

    switch (cmdType) {
        case CMD_ADD:
            createTask(argv[2]);
            if (argc < 3)
            {
                fprintf(stderr, "Usage: add <description>\n");
                return;
            }
            break;
        case CMD_UPDATE:
            if (argc < 4) {
                fprintf(stderr, "Usage: update <id> <description>\n");
                return;
            }
            taskId = atoi(argv[2]); 
            updateTask(&taskId, argv[3]);
            break;
        case CMD_DELETE:
            if (argc < 3) {
                fprintf(stderr, "Usage: delete <id>\n");
                return;
            }
            taskId = atoi(argv[2]); 
            deleteTask(&taskId); 
            break;
        case CMD_LIST:
            listTasksWhere(NULL);
            if (argc > 3)
            {
                fprintf(stderr, "Usage: list <status>\n");
                return;
            }
            break;
        case CMD_MARK_IN_PROGRESS:
            if (argc < 3) {
                fprintf(stderr, "Usage: mark-in-progress <id>\n");
                return;
            }
            taskId = atoi(argv[2]); 
            setTaskTo(&taskId, &statuses[1]);
            break;
        case CMD_MARK_DONE:
            if (argc < 3) {
                fprintf(stderr, "Usage: mark-done <id>\n");
                return;
            }
            taskId = atoi(argv[2]); 
            setTaskTo(&taskId, &statuses[2]);
            break;
        case CMD_LIST_DONE:
            if (argc != 3)
            {
                fprintf(stderr, "Usage: list <status>\n");
                return;
            }
            listTasksWhere(&statuses[2]);
            break;
        case CMD_LIST_TODO:
            if (argc != 3)
            {
                fprintf(stderr, "Usage: list <status>\n");
                return;
            }
            listTasksWhere(&statuses[0]);
            break;
        case CMD_LIST_IN_PROGRESS:
            if (argc != 3)
            {
                fprintf(stderr, "Usage: list <status>\n");
                return;
            }
            listTasksWhere(&statuses[1]);
            break;
        case CMD_UNKNOWN:
        default:
            printHelp();
            break;
    }
}
