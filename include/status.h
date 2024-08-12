#ifndef STATUS_H
#define STATUS_H

#define STATUS_COUNT 3

typedef struct {
    int id;
    const char *status;
} Status;

extern Status statuses[STATUS_COUNT];
const char* getStatus(int statusCode);

#endif // STATUS_H
