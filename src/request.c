#include <stddef.h>
#include <string.h>
#include "request.h"

void reqtobuf(const struct request *req, void *buffer, size_t n) {
    memcpy(buffer, &req->length, 4);
    memcpy((char *) buffer + 4, req->file, n - 4);
}

void buftoreq(const void *buffer, struct request *req, size_t n) {
    memcpy(&req->length, buffer, 4);
    req->file = (char *) buffer + 4;
}