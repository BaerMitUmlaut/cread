#include <stdio.h>
#include <string.h>
#include "client.h"
#include "server.h"

int main(int argc, char const *argv[]) {
    if (argc == 2 && strcmp("-s", argv[1]) == 0) {
        printf("Server mode\n");
        return serverMode();
    } else if (argc > 3) {
        printf("Client mode\n");
        return clientMode(argv[1], argv[2], argc - 3, &argv[3]);
    } else {
        printf("Args: -s | IP SIZE FILE...\n");
        return 0;
    }
}
