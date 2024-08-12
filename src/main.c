#include <stdio.h>
#include <stdlib.h>
#include "../include/command.h"
#include "../include/file_ops.h"
#include "../include/utils.h"

int main(int argc, char *argv[]) {
    CommandType cmd = parseCommandLineArguments(argc, argv);
    executeCommand(cmd, argc, argv);
    return 0;
}
