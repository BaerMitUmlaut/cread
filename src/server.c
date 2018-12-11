#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include "request.h"

void* handleClient(void *client);

int serverMode() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) return 1;
    const int y = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y));

    struct sockaddr_in client;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_family = AF_INET;
    client.sin_port = htons(16000);

    /* listen for incoming connections */
    if (bind(sock, (struct sockaddr *) &client, sizeof(client)) == -1)
        return 2;
    listen(sock, 1);

    socklen_t len = sizeof(client);
    while (1) {
        /* accept incoming connection */
        int clientSocket = accept(sock, (struct sockaddr *) &client, &len);

        /* start new thread for client */
        pthread_t thread;
        pthread_create(&thread, NULL, handleClient, &clientSocket);
    }
    close(sock);

    return 0;
}

void* handleClient(void *client) {
    int clientSocket = * (int *)client;
    char buf[1024];

    while (recv(clientSocket, &buf, sizeof(buf), 0) > 0) {
        /* read request */
        struct request req = { .length = 0, .file = NULL };
        buftoreq(&buf, &req, sizeof(buf));

        printf("Client requested %u bytes of %s\n", req.length, req.file);

        /* read and send file or error message */
        /* packets are all 1024 bytes for simplicity */
        FILE *handle = fopen(req.file, "r");
        if (handle) {
            char filebuf[1024] = {0};
            size_t readBytesTotal = 0;
            size_t readBytes = 0;

            /* send at least one package, so even if 0 bytes are requested, empty package is sent */
            do {
                /* read (another) 1024 bytes or less if requested length is reached */
                int bytesToRead = req.length - readBytesTotal > 1024 ? 1024 : req.length;
                readBytes = fread(filebuf, 1, bytesToRead, handle);
                readBytesTotal += readBytes;

                /* terminate string if EOF was reached */
                if (readBytes < 1024)
                    filebuf[readBytes] = '\0';

                send(clientSocket, &filebuf, 1024, 0);
            /* continue until either requested length was read or end of file was reached */
            } while (readBytesTotal < req.length && readBytes == 1024);
            fclose(handle);
        } else {
            /* error message must also be 1024 bytes */
            char error[1024];
            snprintf(error, 1024, "File %s not found.", req.file);
            send(clientSocket, &error, sizeof(error), 0);
        }

        /* send empty packet to indicate end of current file */
        char endSignal[1024] = {0};
        send(clientSocket, &endSignal, 1024, 0);
    }

    close(clientSocket);
    return NULL;
}