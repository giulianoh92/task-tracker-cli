#include "../include/status.h"

Status statuses[STATUS_COUNT] = {
    {1, "todo"},
    {2, "in-progress"},
    {3, "done"}
};

const char* getStatus(int statusCode) {
    for (int i = 0; i < STATUS_COUNT; i++) {
        if (statuses[i].id == statusCode) return statuses[i].status;
    }
    return "unknown";
}
