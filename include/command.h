#ifndef COMMAND_H
#define COMMAND_H

typedef enum {
    CMD_ADD,
    CMD_UPDATE,
    CMD_DELETE,
    CMD_MARK_IN_PROGRESS,
    CMD_MARK_DONE,
    CMD_LIST,
    CMD_LIST_DONE,
    CMD_LIST_TODO,
    CMD_LIST_IN_PROGRESS,
    CMD_HELP,
    CMD_EXIT,
    CMD_UNKNOWN
} CommandType;

CommandType parseCommandLineArguments(int argc, char *argv[]);
void executeCommand(CommandType cmdType, int argc, char *argv[]);

#endif // COMMAND_H
