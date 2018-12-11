#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "request.h"

int clientMode(const char* ip, const char* size, const int fileNum, char** files) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) return 1;

    struct sockaddr_in server;
    inet_pton(AF_INET, ip, &server.sin_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(16000);

    /* wait for server */
    while (1) {
        if (connect(sock, (struct sockaddr *) &server, sizeof(server)) > -1)
            break;
        sleep(1);
    }

    for (int i = 0; i < fileNum; i++) {
        printf("Requesting %s:\n", files[i]);

        /* send request */
        struct request req;
        req.file = files[i];
        req.length = atoi(size);
        unsigned char reqbuf[strlen(files[i]) + 5];
        reqtobuf(&req, &reqbuf, sizeof(reqbuf));
        send(sock, &reqbuf, sizeof(reqbuf), 0);

        /* receive data */
        char filebuf[1025]; /* extra char for \0 */
        int received;
        while ((received = recv(sock, &filebuf, 1024, 0)) > 0) {
            if (filebuf[0] == '\0') break;
            filebuf[received] = '\0';
            printf("%s", filebuf);
        }
        printf("\n");
    }

    close(sock);
    return 0;
}