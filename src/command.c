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
        if (strcmp(argv[0], "update") == 0) return CMD_UPDATE;
    } else if (argc == 2) {
        if (strcmp(argv[0], "add") == 0) return CMD_ADD;
        if (strcmp(argv[0], "delete") == 0 && strcmp(argv[1], "all") == 0) {
            return CMD_DELETE_ALL;
        }else if (strcmp(argv[0], "delete") == 0) return CMD_DELETE;
        if (strcmp(argv[0], "mark-in-progress") == 0) return CMD_MARK_IN_PROGRESS;
        if (strcmp(argv[0], "mark-done") == 0) return CMD_MARK_DONE;
        if (strcmp(argv[0], "list") == 0) {
            if (strcmp(argv[1], "done") == 0) return CMD_LIST_DONE;
            if (strcmp(argv[1], "todo") == 0) return CMD_LIST_TODO;
            if (strcmp(argv[1], "in-progress") == 0) return CMD_LIST_IN_PROGRESS;
        }
    } else if (argc == 1) {
        if (strcmp(argv[0], "list") == 0) return CMD_LIST;
        if (strcmp(argv[0], "exit") == 0) return CMD_EXIT;
        if (strcmp(argv[0], "help") == 0) return CMD_HELP;
        if (strcmp(argv[0], "clear") == 0) return CMD_CLEAR;
    }
    return CMD_UNKNOWN;
}

void executeCommand(CommandType cmdType, int argc, char *argv[]) {
    int taskId;

    switch (cmdType) {
        case CMD_ADD:
            if (argc < 2) {
                fprintf(stderr, "Usage: add <description>\n");
                return;
            }
            createTask(argv[1]);
            break;
        case CMD_UPDATE:
            if (argc < 3) {
                fprintf(stderr, "Usage: update <id> <description>\n");
                return;
            }
            taskId = atoi(argv[1]);
            updateTask(&taskId, argv[2]);
            break;
        case CMD_DELETE:
            if (argc < 2) {
                fprintf(stderr, "Usage: delete <id>\n");
                return;
            }
            taskId = atoi(argv[1]);
            deleteTask(&taskId);
            break;
        case CMD_DELETE_ALL:
            if (argc > 2) {
                fprintf(stderr, "Usage: delete all\n");
                return;
            }
            deleteAllTasks();
            break;
        case CMD_LIST:
            if (argc > 2) {
                fprintf(stderr, "Usage: list <status>\n");
                return;
            }
            listTasksWhere(NULL);
            break;
        case CMD_MARK_IN_PROGRESS:
            if (argc < 2) {
                fprintf(stderr, "Usage: mark-in-progress <id>\n");
                return;
            }
            taskId = atoi(argv[1]);
            setTaskTo(&taskId, &statuses[1]);
            break;
        case CMD_MARK_DONE:
            if (argc < 2) {
                fprintf(stderr, "Usage: mark-done <id>\n");
                return;
            }
            taskId = atoi(argv[1]);
            setTaskTo(&taskId, &statuses[2]);
            break;
        case CMD_LIST_DONE:
            if (argc != 2) {
                fprintf(stderr, "Usage: list <status>\n");
                return;
            }
            listTasksWhere(&statuses[2]);
            break;
        case CMD_LIST_TODO:
            if (argc != 2) {
                fprintf(stderr, "Usage: list <status>\n");
                return;
            }
            listTasksWhere(&statuses[0]);
            break;
        case CMD_LIST_IN_PROGRESS:
            if (argc != 2) {
                fprintf(stderr, "Usage: list <status>\n");
                return;
            }
            listTasksWhere(&statuses[1]);
            break;
        case CMD_HELP:
            printHelp();
            break;
        case CMD_CLEAR:
            system("clear");
            break;
        case CMD_UNKNOWN:
        default:
            fprintf(stderr, "Unknown command. Type 'help' for a list of commands.\n");
            break;
    }
}