#include <stdio.h>
#include <string.h>
#include "client.h"
#include "server.h"

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        printf("Args: -s | IP SIZE FILE\n");
        return 0;
    } else if (strcmp("-s", argv[1]) == 0) {
        printf("Server mode\n");
        return serverMode();
    } else if (argc >= 3) {
        printf("Client mode\n");
        return clientMode(argv[1], argv[2], argv[3]);
    }
    return 0;
}
