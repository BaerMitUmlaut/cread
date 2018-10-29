#include <stddef.h>
#include <stdint.h>

struct request {
    char *file;
    uint32_t length;
};

void reqtobuf(const struct request *req, void *buffer, size_t n);
void buftoreq(const void *buffer, struct request *req, size_t n);
