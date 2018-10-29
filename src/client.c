#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "request.h"

int clientMode(const char* ip, const char* size, char* file) {
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

    /* send request */
    struct request req;
    req.file = file;
    req.length = atoi(size);
    unsigned char reqbuf[strlen(file) + 5];
    reqtobuf(&req, &reqbuf, sizeof(reqbuf));
    send(sock, &reqbuf, sizeof(reqbuf), 0);

    /* receive data */
    char filebuf[1024];
    int received;
    while ((received = recv(sock, &filebuf, sizeof(filebuf) - 1, 0)) > 0) {
        filebuf[received] = '\0';
        printf("%s", filebuf);
    }

    printf("\n");
    close(sock);
    return 0;
}