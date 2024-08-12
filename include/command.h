#ifndef COMMAND_H
#define COMMAND_H

typedef enum {
    CMD_UNKNOWN,
    CMD_ADD,
    CMD_UPDATE,
    CMD_DELETE,
    CMD_LIST,
    CMD_MARK_IN_PROGRESS,
    CMD_MARK_DONE,
    CMD_LIST_DONE,
    CMD_LIST_TODO,
    CMD_LIST_IN_PROGRESS
} CommandType;

CommandType parseCommandLineArguments(int argc, char *argv[]);
void executeCommand(CommandType cmdType, int argc, char *argv[]);

#endif // COMMAND_H
